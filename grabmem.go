// a program to grab memory over time

package main
import (
 "fmt"
 "io"
 //"bytes"
 "flag"
 "time"
 "crypto/rand"
)
var  memsize, step, max int
var interval, now, then time.Duration
var myMemory = make([]byte, step)
//var myMemory  bytes.Buffer

func usage() {
    fmt.Println("a program to suck up memory")
}

func init() {
    flag.IntVar(&memsize, "step",1, "step size for memory increases")
    flag.IntVar(&max, "max",5, "maximum memory to allcate")
    flag.DurationVar(&interval, "interval", 1, "time between memory steps")
    flag.Parse()
}

func fill() {
    //myMemory.Grow(step)
    bytesWritten, err := io.ReadFull(rand.Reader, myMemory)
    if bytesWritten != len(myMemory) || err != nil {
        fmt.Println("error:",err)
        return
    }
    time.Sleep(interval)
}

func main () {
    myMemory = make([]byte, step)
    for memsize <= max {
      myMemory = append(myMemory , [step]byte  )
      fill()
    }
    usage()
}
