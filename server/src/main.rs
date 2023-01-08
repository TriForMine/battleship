use env_logger::WriteStyle;
use log::{debug, info, LevelFilter};
use tokio::net::TcpListener as AsyncTcpListener;

use crate::server::GameServer;

mod types;
mod server;

#[tokio::main]
async fn main() {
    env_logger::Builder::new()
        .write_style(WriteStyle::Always)
        .filter(None, LevelFilter::Debug)
        .init();

    let server = GameServer::new();
    let listener = AsyncTcpListener::bind("127.0.0.1:1234").await.unwrap();
    info!("Server started on {}", listener.local_addr().unwrap());

    loop {
        let (stream, addr) = listener.accept().await.unwrap();
        debug!("New connection from {}", addr);
        let mut server = server.clone();
        tokio::spawn(async move {
            server.handle_connection(stream, addr).await;
        });
    }
}