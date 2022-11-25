#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

    if(argc < 2){
        printf("ERROR -> usage: %s <source_filename1> ... \n\r", argv[0]);
		return EXIT_FAILURE;
    }
    
    int i;
    pid_t pid;
    int status;
   

    char* args[] = {"zip", "epubs.zip", ".epub"};
    int index = 3;
    
    
    for (i = 1; i < argc; i++) {

        char *file = argv[i];
        char *epub = malloc(strlen(file) + 1);
        strcpy(epub, file);
        epub[strlen(epub) - 4] = '\0';
        strcat(epub, ".epub");

        
        
        args[index] = epub;
        index++;


        if ((pid = fork()) == -1){     // criar um processo
            perror("fork");
            return EXIT_FAILURE;
        }



        if (pid == 0) {
            printf("pid[%d] converting %s ...\n", getpid(), file);
            if(execlp("pandoc", "pandoc", file, "-o", epub, NULL) == -1){
                perror("execlp");
                return EXIT_FAILURE;
            }
        }
    }

    
    for (i = 1; i < argc; i++) {
        if (wait(&status) == -1){       // parent waits child (pid=0) to finish
            perror("wait");
            return EXIT_FAILURE;
        }
    }

    args[index] = NULL;
    
    

    if(execvp("zip", args) == -1){
        perror("execvp");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
