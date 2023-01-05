use dashmap::DashMap;

use tokio::net::{TcpListener, TcpStream};

const MAX_ROOMS: usize = 100;

#[derive(Debug)]
struct Room {
    player1: Option<TcpStream>,
    player2: Option<TcpStream>,
}


impl Room {
    fn new() -> Room {
        Room {
            player1: None,
            player2: None,
        }
    }

    fn add_player(&mut self, stream: TcpStream) -> Option<usize> {
        if self.player1.is_none() {
            self.player1 = Some(stream);
            Some(1)
        } else if self.player2.is_none() {
            self.player2 = Some(stream);
            Some(2)
        } else {
            None
        }
    }

    fn remove_player(&mut self, player: usize) {
        if player == 1 {
            self.player1 = None;
        } else if player == 2 {
            self.player2 = None;
        }
    }

    fn send(&mut self, player: usize, msg: &[u8]) {
        if player == 1 && self.player1.is_some() {
            self.player1.unwrap().send(msg);
        } else if player == 2 && self.player2.is_some() {
            self.player2.unwrap().send(msg);
        }
    }
}

async fn handle_client(mut stream: TcpStream, mut rooms: &DashMap<usize, Room>) {
    let mut buf = [0; 4096];

    loop {
        stream.readable().await;
        let n = stream.try_read(&mut buf).unwrap();
        if n == 0 {
            break;
        }

        let s = String::from_utf8_lossy(&buf[..n]);
        println!("Received: {}", s);

        // Find an available room and add the client to it
        let mut room = None;
        for mut r in rooms.iter_mut() {
            if r.player1.is_none() {
                r.player1 = Some(stream.clone());
                room = Some(r.value_mut());
                break;
            } else if r.player2.is_none() {
                r.player2 = Some(stream.clone());
                room = Some(r.value_mut());
                break;
            }
        }

        // If no room is available, create a new one
        if room.is_none() {
            let mut r = Room {
                player1: Some(stream.clone()),
                player2: None,
            };
            let key = rooms.len();
            rooms.insert(rooms.len(), r);
            room = Some(rooms.get_mut(&key).unwrap().value_mut());
        }

        let mut room = room.unwrap();
        if room.player2.is_some() {
            println!("Starting game in room {:?}", room.value());
            // Start the game in a new task
            tokio::spawn(async move {
                play_game(room.value_mut()).await;
            });
        }
    }
}

async fn play_game(room: &mut Room) {
    // Wait for player actions and broadcast them to the other player
    let mut bufs = [
        [0; 4096],
        [0; 4096],
    ];
    let mut streams = [
        room.player1.as_mut().unwrap(),
        room.player2.as_mut().unwrap(),
    ];

    loop {
        for i in 0..2 {
            let n = streams[i].read(&mut bufs[i]).await.unwrap();
            if n == 0 {
                break;
            }

            let s = String::from_utf8_lossy(&bufs[i][..n]);
            println!("Player {} sent: {}", i + 1, s);
            let _ = streams[1 - i].write_all(&bufs[i][..n]).await;
        }
    }
}

#[tokio::main]
async fn main() {
    let listener = TcpListener::bind("0.0.0.0:1234").await.unwrap();
    let mut rooms = DashMap::new();

    loop {
        let (stream, _) = listener.accept().await.unwrap();
        let rooms = &mut rooms;
        tokio::spawn(async move {
            handle_client(stream, rooms).await;
        });
    }
}