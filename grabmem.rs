//we'll figure out cli parsing later but its 'extern crate getopts;
use std::{thread, time};
let mut memsizee = 0i64;// this seemed risky back in 2010
let mut myMemory = [0;0];
fn usage( ){
    println!("usage: grabmem max rate step");
    println!("this program allocates memory in INTERVAL second intervals, increasing by SIZE megabytes of memory,\n");
}
fn grab(size: i64) {
    myMemory = [0;size];//will this get optimized away if we never use it?
    memsize += size;
}


fn main(){  
    usage();
    grab(1);
    let one_sec = time::Duration::new(1);
    thread::sleep(one_sec);
}

