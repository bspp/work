package main

import (
	"io"
	"log"
	"net"
	"os"
)

func main() {
	//获取服务器端的地址
	raddr, err := net.ResolveTCPAddr("tcp", "127.0.0.1:8000")
	if err != nil {
		log.Printf("hawk server [%s] resolve error: [%s]", "127.0.0.1:8000", err.Error())
		os.Exit(1)
	}
	//	laddr, err := net.ResolveTCPAddr("tcp", "127.0.0.0:8099")
	//	if err != nil {
	//		log.Println(err)
	//	}
	conn, err := net.DialTCP("tcp", nil, raddr)
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	go mustCopy(os.Stdout, conn)//read from sever
	mustCopy(conn,os.Stdin)//write to server

}
func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
