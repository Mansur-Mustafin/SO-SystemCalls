#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h> 

// ------ uma possivel solucao ---------
/*
int count_file(const char *fname){
    int n;
    //char ch;
    char buffer[100];
    int count = 0;
    FILE *fp = fopen(fname, "rt");
    if (fp == NULL) return 0;
    while ((n = fread(buffer, 1, sizeof buffer, fp)) > 0) {
        count += n;
    }
    fclose(fp);
    return count;
}
*/


// write(STDOUT_FILENO, buffer, n_bytesread); // write to STDOUT

int main(int argc, char* argv[]) {
    struct stat info;

    // teste de numero de argumentos
    if(argc != 4){
		printf("Erro numero de argumentos");
		return EXIT_FAILURE;
	}

    // testamos a abrir file para ler
    FILE * fp = fopen(argv[1],"rt");
    FILE * fp_origin = fp;
    if(fp == NULL){
		printf("File not in working directory %s\n", argv[1]);
		return EXIT_FAILURE;
	}

    // inicializamos o buffer com 
    int buffer_size = atoi(argv[3]);    // numero de char numa string
    char buffer[buffer_size];           // os string com resultado
    char onechar[1];
    int times = atoi(argv[2]);          // numero de vezes que temos que imprimir
    
    // testamos a stat info de file
    if(stat(argv[1], &info) == -1){
       fprintf(stderr, "fsize: Cannot stat %s\n", argv[1]);
      return EXIT_FAILURE;
    } 
    int max_random_number = (int)info.st_size - buffer_size; // o numero maximo de onde podemos comecar a ler 
    
    //int max_random_number = count_file(argv[1]) - buffer_size; 

    time_t t;
    int flag;
    srandom((unsigned) time(&t));
    
    
    while(times){

        int index = random() % max_random_number;       // gera um numero entre [0; max_random_number]   
        fseek(fp, index, SEEK_SET);                     
        fread(buffer, buffer_size, 1, fp);
        if(buffer[0] > 127 || buffer[0] < 0 || buffer[buffer_size - 1] > 127 || buffer[buffer_size - 1] < 0){
            fp = fp_origin;
            continue;
        }

        //write(STDOUT_FILENO, bufferMenor, sizeof(char));
        printf("%c",'>');
        //putchar('>');
        flag = 0;
        for(int l = 0; l < buffer_size; l++){
            if(buffer[l] == '\n'){
                flag++;
                continue;
            }
            if(buffer[l] > 127 || buffer[l] < 0 ){
                printf("%c",buffer[l]);
                //putchar(buffer[l]);
                //write(STDOUT_FILENO, buffer[l], sizeof(char)); // write to STDOUT
                l++;
                printf("%c",buffer[l]);
                //putchar(buffer[l]);
                l++;
                printf("%c",buffer[l]);
                //putchar(buffer[l]);
                flag += 2;
            }else{
                //putchar(buffer[l]);
                //write(STDOUT_FILENO, buffer[l], sizeof(char));
                printf("%c",buffer[l]);
            }
        }

        // caso de houver char nao de assci table
        while(flag){

            fread(onechar, 1 , 1, fp);

            if(onechar[0] == '\n'){
                continue;
            }

            if(onechar[0] > 127 || onechar[0] < 0){
                //write(STDOUT_FILENO, onechar, sizeof(char));
                printf("%c",onechar[0]);
                //putchar(onechar[0]);
                fread(onechar, 1 , 1, fp);
                //write(STDOUT_FILENO, onechar, sizeof(char));
                printf("%c",onechar[0]);
                //putchar(onechar[0]);
                //write(STDOUT_FILENO, onechar, sizeof(char));
                fread(onechar, 1 , 1, fp);
                printf("%c",onechar[0]);
                //putchar(onechar[0]);
            }else{
                //write(STDOUT_FILENO, onechar, sizeof(char));
                printf("%c",onechar[0]);
            }

            flag--;
        }
        //putchar('<');
        //putchar('\n');
        printf("%s","<\n");
        fp = fp_origin;
        times--;

    }

    return EXIT_SUCCESS;
}
