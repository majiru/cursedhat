package main

import (
	"bufio"
	"fmt"
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

	for {
		m, _ := bufio.NewReader(c).ReadString('\n')

		fmt.Print(m)
	}
}
