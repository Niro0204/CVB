#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printHelp();
void printVersion();

struct format{
    
}

int main(int argc, char* argv[]){

    int startLine = 0;
    int endLine = 0;

    FILE* file = NULL;

    //when the last argument is "-", the input is set to stdin
    if(strcmp(argv[argc-1],"-")==0){
        file = stdin;
    }
    else{
        file = fopen(argv[argc-1],"r");
        if(file == NULL){
            fprintf(stderr,"failed to open file!");
        }
    }

    //process other arguments
    for(int i = 0;i<argc;i++){
        
        if(strcmp(argv[i],"--help")==0){
            printHelp();
        }
        else if(strcmp(argv[i],"-s")==0){
            startLine = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i],"-e")==0){
            endLine = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i],"-v")){
            //coming soon 
        }
        else if(strcmp(argv[i],"-q")==0){
            //coming soon
        }
        else if(strcmp(argv[i],"--version")==0){
            printVersion();
        }
        else if(strcmp(argv[i],"-n")==0){
            //wip format handling
        }
    }

    if(strcmp(argv[argc-1],"-")!=0){
        fclose(file);
    }

    return 0;
}

void printHelp(){

    printf("Help information:\n"
           "--help - Help information is output; program terminates\n"
           "-s n - Specify the first (nth) line to be output\n"
           "-e n - Specify the last (mth) line to be output\n"
           "-n [format] output with leading line numbers\n"
           "Optional format parameters:\n"
           "    uint - Width of line number field (numeric)\n"
           "    R - right aligned (default)\n"
           "    0 - right-aligned with leading zeros\n"
           "    L - left aligned\n"
           "    N - the line number starts with zero\n"
           "-v - program prints any additional status and file information during processing\n"
           "-q - ​program only prints any additional status and file information, but does not print the contents of the file\n"
           "--version - version information of the program is output\n");

}

void printVersion(){

    printf("cvb version: 1.0, 30.03.2024");
}