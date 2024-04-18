#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_LINE_LENGTH 1000

//struct for better format handling
typedef struct formating{

    int width;
    bool align_right;
    bool align_left;
    bool fillZeros;
    bool line_startZero;

}formating;

void printHelp(){

//for more information about the program read the README 
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

//printing latest 
void printVersion(){

    fprintf(stdout,"cvb version: 1.0, 08.04.2024");
}

//printing information about the source file
void fileInfo(int argc, char* argv[]){

    struct stat filestat;
            stat(argv[argc - 1], &filestat);
            fprintf(stdout,"File access time %s", ctime(&filestat.st_atime));
            fprintf(stdout,"File modify time %s", ctime(&filestat.st_mtime));
            fprintf(stdout,"File changed time %s", ctime(&filestat.st_ctime));

}

//checks if all the given parameters are also in the allowed parameters list
bool allowedParameter(const char* parameter, const char* paramList[], int sizeParamList){

   for (size_t i = 0; i < sizeParamList; i++) {
        if (strcmp(parameter, paramList[i]) == 0) {
            return true;
        }
    }
    return false;
}


void printLines(FILE* file, int startLine, int endLine,formating options) {

    char line[MAX_LINE_LENGTH]; //buffer for read lines
    int lineCount = options.line_startZero ? 0 : 1;
    int printedLines = 0; 
        
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {

        // deletes endline symbol
        line[strcspn(line, "\n")] = '\0';

        if (lineCount >= startLine && (endLine == -1 || lineCount <= endLine)) {
            char line_number_str[10]; // Buffer for formated line number

            // formating line number 
            if (options.fillZeros && options.align_right) {
                snprintf(line_number_str, sizeof(line_number_str), "%0*d", options.width, lineCount); //prints lines with given format in buffer
             
            } else if(options.fillZeros && options.align_left){
                fprintf(stderr,"Error: Can't use fill zeros and align left at the same time");
                exit(1);
            }
            else if(options.align_left && options.align_right){
                fprintf(stderr,"Error: Can't align left and right at the same time");
                exit(1);
            }
            else if(!options.fillZeros && options.align_right){
                snprintf(line_number_str, sizeof(line_number_str), "%*d", options.width, lineCount);
            }
            else if(!options.fillZeros && options.align_left){
                snprintf(line_number_str, sizeof(line_number_str), "%-*d", options.width, lineCount);
            }
            else{
                fprintf(stderr,"Error: Printing format failed\n");
            }

            fprintf(stdout, "%s: %s\n",line_number_str, line); //printing lines from buffer
           
            printedLines++; // increment printed lines to keep track
        }
        
        // checking of the endline is printed or if the last line is reached
        if (lineCount == endLine || printedLines == endLine - startLine + 1) {
            break;
        }

        lineCount++; //incrementing lineCount to continue loop
    }
}

formating formatHandling(const char* format){

    formating options = {3,true,false,false,false}; //default options 

    //checking string for formating options
    if(format != NULL){

        //if there is an unknown character it gets ignored
        for(int i =0; format[i] != '\0';i++){
            switch (format[i]){
                case 'R':
                    options.align_right = true;
                    break;
                case 'L':
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
    const char* paramList[] = {"-s","-e","-v","-q","-n","-"}; //allowed parameters
    int sizeParamList = sizeof(paramList) / sizeof(paramList[0]);

    //checking if there are arguments in argv
    if(argc < 2){
        fprintf(stderr,"Error: not enough arguments\n");
        exit(1);
    }

    //checking for help or version argument
    if(strcmp(argv[argc-1],"--help")==0){
        printHelp();
        exit(1);
    }

    else if(strcmp(argv[argc-1],"--version")==0){
        printVersion();
        exit(1);
    }

    //creating file pointer for inputstream
    FILE* instream = NULL;
    

    if(strcmp(argv[argc-1],"-")==0){
        instream = stdin; //input gets read from stdin
    }
    else{
        instream = fopen(argv[argc-1],"r"); //filepointer is set to last given argument
        if(instream == NULL){
            fprintf(stderr,"Error: Failed to open file!\n");
            exit(1);
        }
    }

    //check if there are not allowed parameters 
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && !allowedParameter(argv[i],paramList,sizeParamList)) {
            fprintf(stderr, "Error: Unknown argument '%s'.\n", argv[i]);
            exit(1);
        }
    }

    //loops trough parameters 
    for(int i = 0;i<argc;i++){
        
        if(strcmp(argv[i],"-s")==0 && i+1 < argc){
         
            startLine = atoi(argv[++i]); //incrementing bevor reading parameter to get the input number

            if(atoi(argv[i])==0){ //checking if there is a number behind the -s 
                fprintf(stderr,"Error: Need a value for -s\n"); 
                exit(1);
            }
        }
        else if(strcmp(argv[i],"-e")==0 && i+1 < argc){

            endLine = atoi(argv[++i]);

            if(atoi(argv[i])==0){
                fprintf(stderr,"Error: Need a value for -e\n"); 
                exit(1);
            }
        }
        else if(strcmp(argv[i],"-v")==0){
            verbose_mode = true;
        }
        else if(strcmp(argv[i],"-q")==0){
            quiet_mode = true;
        }
        else if(strcmp(argv[i],"-n")==0 && i+1 < argc){
           format=formatHandling(argv[++i]); //giving the format handling the parameter string
        }
       
    
    }


    if (quiet_mode && verbose_mode) {
        fprintf(stderr, "Error: Cannot use both -q and -v options at the same time.\n");
        exit(1);
    }

    //detailed mode with output
    if (verbose_mode) {
        fprintf(stdout,"Verbose mode is enabled.\n");
        fileInfo(argc,argv);
        printLines(instream, startLine, endLine, format);
    }
    //detailed mode without output
    else if (quiet_mode) {
        fprintf(stdout,"Quiet mode is enabled.\n");
        fileInfo(argc,argv);
    } 
    //normal output mode
    else{
        printLines(instream, startLine, endLine,format);
    }

    return 0;
}