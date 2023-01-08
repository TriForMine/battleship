use std::fmt::{Debug, Formatter};
use std::net::SocketAddr;
use std::sync::Arc;

use tokio::net::tcp::{OwnedReadHalf, OwnedWriteHalf};
use tokio::sync::Mutex;

#[derive(Clone)]
pub struct Room {
    pub(crate) player1: Player,
    pub(crate) player2: Option<Player>,
}

impl Debug for Room {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Room")
            .field("player1", &self.player1)
            .field("player2", &self.player2)
            .finish()
    }
}

#[derive(Clone)]
pub struct Player {
    pub(crate) addr: SocketAddr,
    pub(crate) read_stream: Arc<Mutex<OwnedReadHalf>>,
    pub(crate) write_stream: Arc<Mutex<OwnedWriteHalf>>,
}

impl Debug for Player {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Player")
            .field("addr", &self.addr)
            .finish()
    }
}