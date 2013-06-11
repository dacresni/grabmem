// a program to grab memory over time

package main
import ( "fmt"
 "io"
 //"bytes"
 "flag"
 "time"
 "crypto/rand"
)
var  memsize, step int
var interval, now, then time.Duration

func usage() {
    fmt.Println("a program to suck up memory")
}
func init() {
    flag.IntVar(&memsize, "step",0, "step size for memory increases")
    flag.DurationVar(&interval, "interval", 0, "time between memory steps")
    flag.Parse()
}
func main () {
    myMemory := make([]byte, step)
    bytesWritten, err := io.ReadFull(rand.Reader, myMemory)
    if bytesWritten != len(myMemory) || err != nil { 
        fmt.Println("error:",err)
        return
    }
}
