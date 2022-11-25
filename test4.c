#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>


#define READ_END 0
#define WRITE_END 1

/*
char *NameOfPipe(int i, int j){
    char *pipeName = malloc(20);
    char int_string[20];
    char int_string2[20];
    itoa(i, int_string, 10);
    itoa(j, int_string2, 10);
    strcat(pipeName, "pipe");
    strcat(pipeName, int_string);
    strcat(pipeName, "to");
    strcat(pipeName, int_string2);
    return pipeName;
}
*/

int main(int argc, char *argv[]) {

    
    int n = atoi(argv[1]);
    float p = atof(argv[2]);
    int t = atoi(argv[3]);

    
    
    int token = 0;
    //int tokenToSend = 0;
    

    

    pid_t pid;

    int i;

    

    // criamos n pipes e n > 1
    int j;
    for (j = 1; j <= n; j++) {
        char NameOfPipe[10];
        sprintf(NameOfPipe, "pipe%dto%d", j, (j % n)+ 1 );
        mkfifo(NameOfPipe, 0666);   
    }
            

    for (i = 1; i <= n; i++) {

        if (( pid = fork()) <  0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        

        if ( pid == 0){

            srand(getpid());

            while(1){
                int f_read;
                int f_write;

                char NameOfPipeRead[10];
                sprintf(NameOfPipeRead, "pipe%dto%d", i, (i % n) + 1);
                if((f_read = open(NameOfPipeRead, O_RDONLY )) < 0){
                    printf("Error reading file 87\n\r");
                    return EXIT_FAILURE;
                }

                read(f_read, &token, sizeof(token));
                close(f_read);

                token++;
                
                float random = (float)rand() / (float)RAND_MAX;
                
                if ( random < p) {
                    printf("[p%d] lock on token (val = %d)\n", i, token);
                    printf("[p%d] unlock token \n", i);
                    sleep(t);
                }



               


                

                char NameOfPipeWrite[10];
                sprintf(NameOfPipeWrite, "pipe%dto%d", (i % n + 1) , ((i % n + 1) % n  + 1) );
                //printf("%s", NameOfPipeWrite);
                if((f_write = open(NameOfPipeWrite, O_WRONLY  )) < 0){
                    printf("Error reading file 96\n\r");
                    //printf("%s", NameOfPipeWrite);
                    return EXIT_FAILURE;
                }
                //printf("helloe");
                write(f_write, &token, sizeof(token));
            }
        }


        if(pid > 0){
            if(i == 1){
                int f_write = open("pipe1to2", O_WRONLY );
                write(f_write, &token, sizeof(token));
            }
            
        }
    }

    // kill all processes  mkfifo. By Ctrl C
    
    return 0;
}

