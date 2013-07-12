/* this is a program to grab a bunch of memory over time to test the memory management of SGE */
/*
 This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//[TODO] we must find the where the breake is later
void* myMemory = NULL; //can I do void pointers?
int Time=0;
size_t memsize=0;
time_t now ,then;
int randomGen;

int timePast(){
    time(&now);
    //return (then.time - now.time);  
    return ((int)now -(int)then);
}

void usage(){
    printf("this program allocates memory in INTERVAL second intervals, increasing by SIZE megabytes of memory,\n");
    printf("until the machine runs out or you quit it with ctrl-c.\n");
    exit(0);
}
// we will intercept SIGINT to catch ctrl-d

void interruptHandler( int sig) {
    printf("%li memory allocated,\n ran for %d seconds,\n",(long)memsize, Time );  //we'll do the human readable later[TODO]
    close(randomGen);
    _exit(0); //diferent exit
}

void grab( size_t size){
    /*if (myMemory != NULL)
        free(myMemory);
    myMemory = malloc(size);
    */ 
    myMemory = realloc(myMemory, size);
    //bzero(myMemory,memsize); 
    //memset (myMemory,1,memsize); 
    if (errno == ENOMEM ) {
      free(myMemory); //if we do this, we free a null pointer right?
      close(randomGen);
      perror("out of memory , exiting");
      exit(errno);
    }
    printf("grabbed %li  memory\n",(long)memsize);
    /*lets read from urandom */
    //read memsize bytes into myMemory from randomGen
    read (randomGen,myMemory,memsize);
    //write(trashcan, myMemory, memsize);
}


int main(int argc, char** argv){
    if (argc < 2 ){ 
        printf("%s interval step [max] [timeout] \n" , argv[0]);
        usage();
    }

    //(void)signal (SIGINT ,interruptHandler); //catch C-c
    //use sigaction instead of signal
    struct sigaction act, oact;
    act.sa_handler = interruptHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, &oact);

    //register signals
    int max , timeout=0;
    randomGen = open("/dev/urandom",O_RDONLY); //a file discriptor
    //trashcan= open("/dev/null",O_WRONLY);
    if (argc >=4 ) max=(size_t) atoi(argv[3]) *1000000;
    if (argc ==5 ) { 
        timeout= atoi(argv[4]);    
    }
    int interval = atoi(argv[1]);
    size_t step =(size_t) atoi(argv[2]) * 1000000 ; //times a megabyte
    time(&then);
    
    while(1) {
        memsize+=step;
        grab(memsize);
        sleep(interval);
        Time =timePast();
        if ((max>0)&&( memsize >= max)) {
            printf("reached maximum memory in %d seconds\n",Time);
            close(randomGen);
            exit(0);
        }
        if ((timeout>0)&& (Time >= timeout)){
            printf("reached timeout period with %d memory grabbed\n",(int)memsize);
            close(randomGen);
            exit(0);
        }
    }
    
    printf("we got all the way here? \n");
    return 0;
}
