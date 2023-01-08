use std::collections::HashMap;
use std::net::SocketAddr;
use std::sync::Arc;

use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpListener as AsyncTcpListener;
use tokio::net::TcpStream as AsyncTcpStream;
use tokio::sync::Mutex;

struct GameServer {
    rooms: HashMap<String, Room>,
}

#[derive(Clone)]
struct Room {
    player1: Player,
    player2: Option<Player>,
}

#[derive(Clone)]
struct Player {
    addr: SocketAddr,
    stream: Arc<Mutex<AsyncTcpStream>>,
}

impl GameServer {
    fn new() -> Self {
        GameServer { rooms: HashMap::new() }
    }

    async fn create_room(&self, player: Player) -> Room {
        Room {
            player1: Player {
                addr: player.addr,
                stream: player.stream,
            },
            player2: None,
        }
    }

    async fn find_free_room(&self, player: Player) -> Option<Room> {
        for room in self.rooms.values() {
            if room.player2.is_none() {
                return Some(Room {
                    player1: room.player1.clone(),
                    player2: Some(Player {
                        addr: player.addr,
                        stream: player.stream,
                    }),
                });
            }
        }
        None
    }

    async fn send_message_to_player(&self, player: Player, message: &str) {
        self.send_message(&player.stream, message).await.expect("Failed to send message");
    }

    async fn handle_connection(&mut self, stream: AsyncTcpStream, addr: SocketAddr) {
        println!("Handling connection from {}", addr);
        let player = Player {
            addr,
            stream: Arc::new(Mutex::new(stream)),
        };
        let room = match self.find_free_room(player.clone()).await {
            Some(room) => {
                self.rooms.insert(room.player1.addr.to_string(), room.clone());
                room
            }
            None => {
                let room = self.create_room(player.clone()).await;
                self.rooms.insert(room.player1.addr.to_string(), room.clone());
                room
            }
        };

        if player.addr == room.player1.addr {
            println!("Player 1 connected");
            self.send_message_to_player(player.clone(), "Waiting for another player...").await;
        } else {
            println!("Player 2 connected");
            self.send_message_to_player(player.clone(), "You are connected to the server").await;
        }

        loop {
            let mut buf = [0; 1024];
            let n = match room.player1.stream.lock().await.read(&mut buf).await {
                // connection closed
                Ok(0) => {
                    println!("Connection closed with {}", addr);
                    return;
                }
                Ok(n) => n,
                Err(e) => {
                    println!("Error reading from stream: {}", e);
                    return;
                }
            };
            let msg = String::from_utf8_lossy(&buf[..n]);
            println!("{}: {}", addr, msg);
            if room.player1.addr == addr {
                if let Some(player2) = &room.player2 {
                    self.send_message_to_player(player2.clone(), &msg).await;
                }
            } else {
                self.send_message_to_player(room.player1.clone(), &msg).await;
            }
        }
    }

    async fn send_message(
        &self,
        stream: &Arc<Mutex<AsyncTcpStream>>,
        message: &str,
    ) -> std::io::Result<()> {
        let mut stream = stream.lock().await;
        stream.write(message.as_bytes()).await?;
        stream.flush().await?;
        Ok(())
    }
}

#[tokio::main]
async fn main() {
    let server = Arc::new(Mutex::new(GameServer::new()));
    let listener = AsyncTcpListener::bind("127.0.0.1:1234").await.unwrap();

    loop {
        let (stream, addr) = listener.accept().await.unwrap();
        println!("New connection from {}", addr);
        let server = Arc::clone(&server);
        tokio::spawn(async move {
            println!("New thread for {}", addr);
            server.lock().await.handle_connection(stream, addr).await;
        });
    }
}