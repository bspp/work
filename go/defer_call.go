package main

import "fmt"
import "runtime"
import "sync"
import "os"

type Student struct {
	Name string
	Age  int
}

func main() {
	runTime()
}

func defer_call() {
	defer func() { fmt.Println("打印前") }()
	defer func() { fmt.Println("打印中") }()
	defer func() { fmt.Println("打印后") }()
	//	panic("触发异常")
}
func pase_student() {
	m := make(map[string]*Student)
	stus := []Student{
		Student{Name: "zhou", Age: 24},
		Student{Name: "li", Age: 23},
	}
	for _, stu := range stus {
		m[stu.Name] = &stu

	}
	fmt.Println(m)
}
func runTime() {
	runtime.GOMAXPROCS(1)
	wg := sync.WaitGroup{}
	wg.Add(20)
	for i := 0; i < 10; i++ {
		go func() {
			fmt.Println("<", i, ">")
			if i == 1000000 {
				os.Exit(1)
			}
			wg.Done()
		}()
	}
	for i := 0; i < 10; i++ {
		go func(i int) {
			fmt.Println("[", i, "]")
			wg.Done()
		}(i)
	}
	wg.Wait()
}
