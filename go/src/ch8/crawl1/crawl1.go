package main

import (
	"fmt"
	"gopl.io/ch5/links"
)

func crawl(url string) []string {
	fmt.Println(url)
	list, err := links.Extract(url)
	if err != nil {
		log.Print(err)
	}
}
func main() {
	fmt.Println("hello word")
}
