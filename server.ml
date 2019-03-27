#!/usr/bin/env ocaml

#load "unix.cma";;

open Unix

let sock_send sock str =
	let len = String.length str in
	send sock (Bytes.of_string str) 0 len []

let sock_recv sock maxlen =
	let str = Bytes.create maxlen in
	let recvlen = recv sock str 0 maxlen [] in
	String.sub (Bytes.to_string str) 0 recvlen ;;

let server_sock = socket PF_INET SOCK_STREAM 0 in
setsockopt server_sock SO_REUSEADDR true ;

let address = (gethostbyname(gethostname())).h_addr_list.(0) in
bind server_sock (ADDR_INET (address, 8000)) ;

listen server_sock 50 ;

while true do 
	let (client_sock, client_addr) = accept server_sock in
	sock_send client_sock "Welcome!!;" ;
	shutdown client_sock SHUTDOWN_ALL
	done ;;