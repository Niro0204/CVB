#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_LINE_LENGTH 1000

typedef struct formating{

    int width;
    bool align_right;
    bool align_left;
    bool fillZeros;
    bool line_startZero;

}formating;

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

void fileInfo(int argc, char* argv[]){

    struct stat filestat;
            stat(argv[argc - 1], &filestat);
            printf("File access time %s", ctime(&filestat.st_atime));
            printf("File modify time %s", ctime(&filestat.st_mtime));
            printf("File changed time %s", ctime(&filestat.st_ctime));

}


void printLines(FILE* file, int startLine, int endLine,formating options) {

    char line[MAX_LINE_LENGTH];
    int lineCount = options.line_startZero ? 0 : 1;
    int printedLines = 0; // Anzahl der gedruckten Zeilen
        
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {

        // Entferne das Zeilenende von 'line'
        line[strcspn(line, "\n")] = '\0';

        if (lineCount >= startLine && (endLine == -1 || lineCount <= endLine)) {
            char line_number_str[10]; // Buffer für die formatierte Zeilennummer


            // Formatieren der Zeilennummer
            if (options.fillZeros) {
                snprintf(line_number_str, sizeof(line_number_str), "%0*d", options.width, lineCount);
             
            } else {
                snprintf(line_number_str, sizeof(line_number_str), "%*d", options.width, lineCount);
            }

            // Ausgabe der Zeile mit der formatierten Zeilennummer
            if (options.align_right) {
       
                fprintf(stdout, "%*s: %s\n", options.width, line_number_str, line);
            } else {
                fprintf(stdout, "%s: %*s\n", line_number_str, options.width, line);
            }

            printedLines++; // Erhöhe die Anzahl der gedruckten Zeilen
        }
        else{
            fprintf(stderr,"error while printing lines");
        }

        // Überprüfen, ob die letzte Zeile erreicht ist oder ob die maximale Anzahl von Zeilen gedruckt wurde
        if (lineCount == endLine || printedLines == endLine - startLine + 1) {
            break;
        }

        lineCount++;
    }
}

formating formatHandling(const char* format){

    formating options = {3,true,false,false,false}; //default options 

    if(format != NULL){

        for(int i =0; format[i] != '\0';i++){
            switch (format[i]){
                case 'R':
                    options.align_right = true;
                    options.align_left = false;
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

int main(int argc,char* argv[]){

    int startLine = 0;
    int endLine = -1;
    bool verbose_mode = false;
    bool quiet_mode = false;
    formating format={3,true,false,false,false}; //default format

    if(argc < 2){
        fprintf(stderr,"not enough arguments");
        exit(1);
    }

    if(strcmp(argv[argc-1],"--help")==0){
        printHelp();
        exit(1);
    }

    else if(strcmp(argv[argc-1],"--version")==0){
        printVersion();
        exit(1);
    }

    FILE* instream = NULL;
    instream = stdin;

    if(strcmp(argv[argc-1],"-")==0){
        instream = stdin;
    }
    else{
        instream = fopen(argv[argc-1],"r");
        if(instream == NULL){
            fprintf(stderr,"failed to open file!");
            exit(1);
        }
    }

    for(int i = 0;i<argc;i++){
        
       
        if(strcmp(argv[i],"-s")==0 && i+1 < argc){
            startLine = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-e")==0 && i+1 < argc){
            endLine = atoi(argv[++i]);
        }
        else if(strcmp(argv[i],"-v")==0){
            verbose_mode = true;
        }
        else if(strcmp(argv[i],"-q")==0){
            quiet_mode = true;
        }
        else if(strcmp(argv[i],"-n")==0 && i+1 < argc){
           format=formatHandling(argv[++i]);
        }
    }

    if (quiet_mode && verbose_mode) {
        fprintf(stderr, "Cannot use both -q and -v options at the same time.\n");
        exit(1);
    }

    if (verbose_mode) {
        fprintf(stdout,"Verbose mode is enabled.\n");
        fileInfo(argc,argv);
        printLines(instream, startLine, endLine, format);
    }
    else if (quiet_mode) {
        fprintf(stdout,"Quiet mode is enabled.\n");
        fileInfo(argc,argv);
    } 
    else{
        printLines(instream, startLine, endLine,format);
    }

    return 0;
}