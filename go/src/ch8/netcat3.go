package main
import(
	"net"
	"fmt"
	"io"
	"os"
)
func main(){
	conn,err := net.Dial("tcp","localhost:8000")
	if err != nil {
		fmt.Println(err)
	}
	done := make(chan struct{})
	go func(){
		io.Copy(os.Stdin,conn)//Note:ignoring errors
		fmt.Println("done")
		done <- struct{}{}//signal the main goroutine
	}()
	mustCopy(os,os.Stdin)
	conn.Close()
	<-done//wait for backgroud groutine to finsh
}
func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}


