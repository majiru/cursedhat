package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"net"
)

func main() {
	const (
		network  = "tcp"
		hostname = "localhost"
		port     = "8000"
	)

	c, err := net.Dial(network, hostname+":"+port)

	if err != nil {
		log.Fatal(err)
	}

	r := bufio.NewReader(c)

	for {
		m, err := r.ReadString('\n')

		if err != nil && err != io.EOF {
			log.Fatal(err)
		}

		if len(m) > 0 {
			fmt.Println(m)
		}
	}
}
