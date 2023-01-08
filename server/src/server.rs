use std::collections::HashMap;
use std::net::SocketAddr;
use std::sync::Arc;

use log::{debug, error, warn};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::tcp::OwnedWriteHalf;
use tokio::net::TcpStream as AsyncTcpStream;
use tokio::sync::Mutex;

use rand::Rng;

use crate::types::{Player, Room};

#[derive(Clone)]
pub struct GameServer {
    rooms: Arc<Mutex<HashMap<String, Room>>>,
}

impl GameServer {
    pub(crate) fn new() -> Self {
        GameServer { rooms: Arc::new(Mutex::new(HashMap::new())) }
    }

    async fn create_room(&self, player: Player) -> Room {
        let mut rng = rand::thread_rng();
        Room {
            player1: Player {
                addr: player.addr,
                read_stream: player.read_stream,
                write_stream: player.write_stream,
            },
            player2: None,
            seed: rng.gen::<u32>(),
        }
    }

    async fn find_free_room(&self, player: Player) -> Option<Room> {
        for room in self.rooms.lock().await.values() {
            if room.player2.is_none() {
                return Some(Room {
                    player1: room.player1.clone(),
                    player2: Some(Player {
                        addr: player.addr,
                        read_stream: player.read_stream,
                        write_stream: player.write_stream,
                    }),
                    seed: room.seed,
                });
            }
        }
        None
    }

    async fn send_message_to_player(&self, player: Player, message: &str) {
        self.send_message(Arc::clone(&player.write_stream), message).await.expect("Failed to send message");
    }

    pub(crate) async fn handle_connection(&mut self, stream: AsyncTcpStream, addr: SocketAddr) {
        let (read_stream, write_stream) = stream.into_split();
        let player = Player {
            addr,
            read_stream: Arc::new(Mutex::new(read_stream)),
            write_stream: Arc::new(Mutex::new(write_stream)),
        };
        let mut room = match self.find_free_room(player.clone()).await {
            Some(room) => {
                self.rooms.lock().await.insert(room.player1.addr.to_string(), room.clone());
                room
            }
            None => {
                let room = self.create_room(player.clone()).await;
                self.rooms.lock().await.insert(room.player1.addr.to_string(), room.clone());
                room
            }
        };

        // send seed to player
        self.send_message_to_player(player.clone(), &format!("seed {}", room.seed)).await;

        if player.addr == room.player1.addr {
            debug!("Player 1 connected on room {}", room.player1.addr);
            self.send_message_to_player(player.clone(), "Waiting for another player...").await;
        } else {
            debug!("Player 2 connected on room {}", room.player1.addr);
            self.send_message_to_player(player.clone(), "You are connected to the server").await;
            self.send_message_to_player(room.player1.clone(), "A player joined, the game will now start").await;
        }

        loop {
            let mut buf = [0; 4096];
            let stream = Arc::clone(&player.read_stream);
            let n = match stream.lock().await.read(&mut buf).await {
                // connection closed
                Ok(0) => {
                    warn!("Connection closed with {}", addr);
                    room = self.rooms.lock().await.get(&*room.player1.addr.to_string()).unwrap().clone();
                    if player.addr == room.player1.addr {
                        if let Some(player2) = &room.player2 {
                            self.send_message_to_player(player2.clone(), "Your opponent left the game").await;
                        }
                    } else {
                        self.send_message_to_player(room.player1.clone(), "Your opponent left the game").await;
                    }

                    self.rooms.lock().await.remove(&*room.player1.addr.to_string());
                    return;
                }
                Ok(n) => n,
                Err(e) => {
                    error!("Error reading from stream: {}", e);
                    return;
                }
            };
            let msg = String::from_utf8_lossy(&buf[..n]);
            room = self.rooms.lock().await.get(&*room.player1.addr.to_string()).unwrap().clone();
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
        stream: Arc<Mutex<OwnedWriteHalf>>,
        message: &str,
    ) -> std::io::Result<()> {
        let mut stream = stream.lock().await;
        stream.write(message.as_bytes()).await?;
        stream.flush().await?;
        Ok(())
    }
}