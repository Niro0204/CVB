#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

//allows better handling of the formating options
typedef struct formating{

    int width;
    bool align_right;
    bool align_left;
    bool fillZeros;
    bool line_startZero;

}formating;

void printHelp();
void printVersion();
formating formatHandling(const char*);
void printLines(FILE*,int,int,formating);

int main(int argc, char* argv[]){

    int startLine = 0;
    int endLine = 0;
    formating format={3,true,false,false,false}; //default format

    FILE* file = NULL;

    if(strcmp(argv[argc-1],"--help")==0){
        printHelp();
        exit(1);
    }

    else if(strcmp(argv[argc-1],"--version")==0){
        printVersion();
        exit(1);
    }

    //when the last argument is "-", the input is set to stdin
    if(strcmp(argv[argc-1],"-")==0){
        file = stdin;
    }
    else{
        file = fopen(argv[argc-1],"r");
        if(file == NULL){
            fprintf(stderr,"failed to open file!");
            exit(1);
        }
    }

   char testString[5]="04";
    //process other arguments
    for(int i = 0;i<argc;i++){
        
       
        if(strcmp(argv[i],"-s")==0 && i+1 < argc){
            startLine = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-e")==0 && i+1 < argc){
            endLine = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-v")==0){
            //coming soon 
        }
        else if(strcmp(argv[i],"-q")==0){
            //coming soon
        }
        else if(strcmp(argv[i],"-n")==0 && i+1 < argc){
           format=formatHandling(argv[++i]);
        }
    }

    printLines(file,startLine,endLine,format);

    
    //closing file if file is not stdin
    if(strcmp(argv[argc-1],"-")!=0){
        fclose(file);
    }

    return 0;
}

void printHelp(){

    fprintf(stdout,"Help information:\n"
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
           "-q - program only prints any additional status and file information, but does not print the contents of the file\n"
           "--version - version information of the program is output\n");

}

void printVersion(){

    printf("cvb version: 1.0, 30.03.2024");
}

formating formatHandling(const char* format){

   formating options = {3,true,false,false,false}; //default options 
    
    if(format != NULL){

        for(int i =0; format[i] != '\0';i++){
            switch (format[i]){
                case 'R':
                    options.align_right = true;
                    break;
                case 'L':
                    options.align_right = false;
                    options.align_left = true;
                    break;
                case '0':
                    options.fillZeros = true;
                    break;
                case 'N':
                    options.line_startZero =true;
                    break;
                default:
                    if (isdigit(format[i])){
                        options.width = atoi(&format[i]);
                    }
                    break;
            }
        }
    }


    return options;
}

void printLines(FILE* file, int startLine, int endLine, formating options) {

  
    char line[MAX_LINE_LENGTH];
    int lineCount = options.line_startZero ? 0 : 1;
    int printedLines = 0; 

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        
        line[strcspn(line, "\n")] = '\0'; //switches next line witch end string symbol

        if (lineCount >= startLine && lineCount <= endLine) {
            char line_number_str[10]; 

          
            if (options.fillZeros) {
                snprintf(line_number_str, sizeof(line_number_str), "%0*d", options.width, lineCount);
            } else {
                snprintf(line_number_str, sizeof(line_number_str), "%*d", options.width, lineCount);
            }

         
            if (options.align_right) {
                fprintf(stdout, "%*s: %s\n", options.width, line_number_str, line);
            } else {
                fprintf(stdout, "%s: %*s\n", line_number_str, options.width, line);
            }

            printedLines++; 
        }

       
        if (lineCount == endLine || printedLines == endLine - startLine + 1) {
            break;
        }

        lineCount++;
    }
}
