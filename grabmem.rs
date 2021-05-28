//we'll figure out cli parsing later but its 'extern crate getopts;
extern crate getopts;
use getopts::Options;
use std::{env, time};
fn usage( ){
    println!("usage: grabmem max rate step");
    println!("this program allocates memory in RATE second intervals, increasing by STEP megabytes of memory, untill memory reaches MAX.\n");
}
fn grab(size: i64) {
    myMemory = size; //will this get optimized away if we never use it?
    memsize += size;
    println!("holding {} memory", memsize);
}


fn main(){  
    let arg: Vec<String> = env::args().collect();

    usage();
    const max  = &arg[1];
    const rate = &arg[2];
    const step = &arg[3].parse::<i64>();
    let duration = time::Duration::from_secs(rate);
    let mut memsize = 0i64 ;
    let mut myMemory = [i64;step] ;

    loop { 
        if memsize >= max {
            println!("hit max memory");
            break;
        }
        grab(step);
        thread::sleep(duration);
    }
}

