#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct formating{

    int width;
    bool align_right;
    bool align_left;
    bool fillZeros;
    bool line_startZero;

}formating;

void printLines(FILE* file, int startLine, int endLine, formating options) {
    char line[1000];
    int lineCount = options.line_startZero ? 0 : 1;
    int printedLines = 0; // Anzahl der gedruckten Zeilen

    while (fgets(line, 1000, file) != NULL) {
        // Entferne das Zeilenende von 'line'
        line[strcspn(line, "\n")] = '\0';

       

        if (lineCount >= startLine && lineCount <= endLine) {
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
                case 'O':
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


int main(){

    FILE* file = NULL;

        file = fopen("test.txt","r");
        if(file == NULL){
            fprintf(stderr,"failed to open file!");
            exit(1);
        }
    char testString[5]="4O";
    formating format = formatHandling(testString);
    printLines(file,1,5,format);

    return 0;
}
