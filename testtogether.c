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


void printLines(int argc, char* argv[], int start_line, int end_line, formating options, bool verbose_mode) {

    char line[MAX_LINE_LENGTH];
    int line_count = options.line_startZero ? 0 : 1;
    int printed_lines = 0;

    FILE* file = NULL;
    file = stdin;

    fseek(file, 0, SEEK_SET); // Reset file pointer to the beginning

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (line_count >= start_line && (end_line == 0 || line_count <= end_line)) {
            char line_number_str[10];

            if (options.fillZeros) {
                snprintf(line_number_str, sizeof(line_number_str), "%0*d", options.width, line_count);
            } else {
                snprintf(line_number_str, sizeof(line_number_str), "%*d", options.width, line_count);
            }

            if (options.align_right) {
                fprintf(stdout, "%*s: %s\n", options.width, line_number_str, line);
            } else {
                fprintf(stdout, "%s: %*s\n", line_number_str, options.width, line);
            }

            printed_lines++; 
        }

        if (end_line != 0 && line_count == end_line) {
            break;
        }

        line_count++;
    }
}

int main(int argc, char* argv[]){

    if(argc < 2){
        return 0;
    }

    int startLine = 1;
    int endLine = 8;
    bool verbose_mode = false;
    formating format={3,true,false,false,false}; //default format

    


    printLines(argc,argv, startLine, endLine, format, verbose_mode);

    return 0;
}