package main

import (
	"bufio"
	"fmt"
	"net"
	"strings"
	"time"
	"log"
)

func echo(c net.Conn, shout string, delay time.Duration) {
	fmt.Fprintf(c, strings.ToUpper(shout))
	time.Sleep(delay)
	fmt.Fprintf(c, shout)
	time.Sleep(delay)
	fmt.Fprintf(c,strings.ToLower(shout))
}
func handleConn(c net.Conn) {
	input := bufio.NewScanner(c)
	for input.Scan() {
		echo(c, input.Text(), 1*time.Second)
	}
	c.Close()
}
func main() {
	listener, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) //connection aborted
			continue
		}
		go handleConn(conn)
	}
}
