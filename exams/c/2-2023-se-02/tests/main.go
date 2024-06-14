package main

import (
	"fmt"
	"math/rand/v2"
	"time"
)

func main() {
	sentences := []string{
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"foo!",
	}

	stop := make(chan struct{})

	go func() {
		time.Sleep(time.Duration(1000000 * time.Second))
		close(stop)
	}()

	for {
		select {
		case <-stop:
			return
		default:
			fmt.Println(sentences[rand.IntN(len(sentences))])
		}
	}
}
