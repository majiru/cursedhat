client:
	go build client.go

server:
	rustc server.rs

clean:
	rm -f client server
