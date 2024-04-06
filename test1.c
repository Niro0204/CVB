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

    char testString[5]="4O";
    formating format = formatHandling(testString);

    printf("%d\n %d\n %d\n %d\n %d\n",format.width,format.align_right,format.align_left,format.fillZeros,format.line_startZero);


    return 0;
}