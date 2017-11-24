// +build ignore

package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"sync"

	"ch8/thumbnail"
)
func main(){

}
func main1() {
	input := bufio.NewScanner(os.Stdin)
	for input.Scan() {
		thumb, err := thumbnail.ImageFile(input.Text())
		if err != nil {
			log.Print(err)
			continue

		}
		fmt.Println(thumb)

	}
	if err := input.Err(); err != nil {
		log.Fatal(err)

	}

}
//无并行的操作
func makeThumbnails(filenames []string){
	for _, filename := range filenames {
		if _,err := thumbnail.ImageFile(f);err != nil {
			log.Println(err)
		}
	}
}
//并行操作，但是工作并不能被完成
func makeThumbnails2(filenames []string){
	for _, filename := range filenames {
		go thumbnail.ImageFile(f) //NOTE:ignoring errors
}
//使用channel发送事件
//注意我们将f的值作为一个显式的变量传给了函数，而不是在循环的闭包中声明。
func makeThumbnail3(filenames []string){
	ch := make(chan struct{})
	for _ ,f := range filenames {
		go func(f string){
			thumbnail.ImageFile(f) //NOTE :ignoring errors
			ch <- struct{}{}
		}(f)
	}
	//等待 goroutines 完成
	for range filenames {
		<-ch
	}
}
func makeThumbnail4(filenames []string)error{
	errors := make(chan error)
	for _,f := range filenames {
		go func(f string){
			_,err := thumbnail.ImageFile(f)
			errors <- err
		}(f)
	}

	for range filenames {
		if err := <-errors;err != nil {
			return err //NOTE：会造成goroutine leak(线程泄露),可能会导致整个程序卡主或者out of memory
		}
	}
}
//使用线程缓存解决该问题
func makeThumbnail5(filenames []string)(thumbfiles []string,err error){
	type item struct {
		thumbfile string
		err error
	}
	ch := make(chan item,len(filenames))
	for _,file := range filenames {
		go func(f string){
			var it item
			it.thumbfile,it.err = thumbnail.ImageFile(f)
			ch <- it
		}(f)
	}
	for range filenames {
		it := <-ch
		if it.err != nil {
			return nil,it.err
		}
		thumbfiles = append(thumbfiles, it.thumbfile)
	}
	return thumbfiles,nil
}
//filenames channel 里面可以有多个filename
func makeThumbnail6(filenames <-chan string)int64{
	sizes := make(chan int64)
	var wg sync.WaitGroup // number of working goroutines
	for f := range filenames{
		wg.Add(1)//增加一个goroutines
		//worker
		go func(f string){
			defer wg.Done()
			thumb,err := thumbnail.ImageFile(f)
			if err != nil {
				log.Println(err)
				return
			}
			info,_ := os.Stat(thumb) //OK to ignore error
			sizes <- info.Size()
		}(f)
	}
	//closer
	go func(){
		wg.Wait()
		close(sizes)
	}()

	var total int64
	for size := sizes {
		total += size
	}
	return total
}
