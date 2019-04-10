use std::io::Write;
use std::net::TcpListener;
use std::thread;

fn main() {
    let listener = TcpListener::bind("localhost:8000").unwrap();

    for stream in listener.incoming() {
        thread::spawn(|| {
            let mut stream = stream.unwrap();
            stream.write(b"Welcome!").unwrap();
        });
    }
}
