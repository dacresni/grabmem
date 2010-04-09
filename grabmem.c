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
//#include <time.h>
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
int time=0;
size_t memsize=0;

int randomGen ,trashcan;

void usage(){
    printf("this program allocates memory in INTERVAL second intervals, increasing by SIZE megabytes of memory,\n");
    printf("until the machine runs out or you quit it with ctrl-d.\n");
    exit(0);
}
// we will intercept SIGINT to catch ctrl-d

void interruptHandler( int sig) {
    printf("%li memory allocated,\n ran for %i seconds,\n",(long)memsize, time );  //we'll do the human readable later[TODO]
    _exit(0); //different exit
}

void grab( size_t size){
    /*if (myMemory != NULL)
        free(myMemory);
    myMemory = malloc(size);
    */ 
    myMemory = realloc(myMemory, size);
    printf("grabbed %li  memory\n",(long)memsize);
    //bzero(myMemory,memsize); 
    //memset (myMemory,1,memsize); 
    if (errno == ENOMEM ) {
      free(myMemory); //if we do this, we free a null pointer right?
      perror("out of memory , exiting");
      exit(errno);
    }
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
    (void)signal (SIGINT ,interruptHandler); //catch C-c
    //register signals
    int max , timeout=0;
    randomGen = open("/dev/urandom",O_RDONLY); //a file discriptor
    //trashcan= open("/dev/null",O_WRONLY);
    if (argc >=4 ) max=(size_t) atoi(argv[3]) *1000000;
    if (argc ==5 ) timeout= atoi(argv[4]);    
    
    int interval = atoi(argv[1]);
    size_t step =(size_t) atoi(argv[2]) * 1000000 ; //times a megabyte
    
    while(1) {
        memsize+=step;
        grab(memsize);
        sleep(interval);
        time+=interval;
        if ((max>0)&&( memsize >= max)) {
            printf("reached maximum memory in %i of MY seconds\n",time);
            exit(0);
        }
        if ((timeout>0)&& (time >= timeout)){
            printf("reached timeout period with %u memory grabbed\n",(unsigned)memsize);
            exit(0);
        }
    }
    
    printf("we got all the way here? \n");
    return 0;
}
