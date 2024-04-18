#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

typedef struct {
    int width;
    bool align_right;
    bool align_left;
    bool fill_zeros;
    bool line_start_zero;
} Formatting;

void print_help();
void print_version();
Formatting handle_format(const char*);
void print_lines(FILE*, int, int, Formatting);

int main(int argc, char* argv[]) {
    int start_line = 0;
    int end_line = 0;
    Formatting format = {3, true, false, false, false}; // Default format
    FILE* file = NULL;
    bool verbose_mode = false;
    bool quiet_mode = false;

    if (strcmp(argv[argc-1], "--help") == 0) {
        print_help();
        exit(1);
    } else if (strcmp(argv[argc-1], "--version") == 0) {
        print_version();
        exit(1);
    }

    if (strcmp(argv[argc-1], "-") == 0) {
        file = stdin;
    } else {
        file = fopen(argv[argc-1], "r");
        if (file == NULL) {
            fprintf(stderr, "failed to open file!");
            exit(1);
        }
    }

    // Process other arguments
    for (int i = 1; i < argc-1; i++) {
        if (strcmp(argv[i], "-s") == 0 && i+1 < argc) {
            start_line = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-e") == 0 && i+1 < argc) {
            end_line = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-n") == 0 && i+1 < argc) {
            format = handle_format(argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose_mode = true;
        } else if (strcmp(argv[i], "-q") == 0) {
            quiet_mode = true;
        }
    }

    if (quiet_mode && verbose_mode) {
        fprintf(stderr, "Cannot use both -q and -v options at the same time.\n");
        exit(1);
    }

    if (!quiet_mode && verbose_mode) {
        printf("Verbose mode is enabled.\n");
    }

    if (!quiet_mode) {
        print_lines(file, start_line, end_line, format);
    } else {
        fseek(file, 0, SEEK_END);
        printf("Total lines: %ld\n", ftell(file) / MAX_LINE_LENGTH);
    }

    if (strcmp(argv[argc-1], "-") != 0) {
        fclose(file);
    }

    return 0;
}

void print_help() {
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
           "--version - version information of the program is output\n");
}

void print_version() {
    printf("cvb version: 1.0, 30.03.2024\n");
}

Formatting handle_format(const char* format) {
    Formatting options = {3, true, false, false, false}; // Default options 
    
    if (format != NULL) {
        for (int i = 0; format[i] != '\0'; i++) {
            switch (format[i]) {
                case 'R':
                    options.align_right = true;
                    break;
                case 'L':
                    options.align_right = false;
                    options.align_left = true;
                    break;
                case '0':
                    options.fill_zeros = true;
                    break;
                case 'N':
                    options.line_start_zero = true;
                    break;
                default:
                    if (isdigit(format[i])) {
                        options.width = atoi(&format[i]);
                    }
                    break;
            }
        }
    }

    return options;
}

void print_lines(FILE* file, int start_line, int end_line, Formatting options) {
    char line[MAX_LINE_LENGTH];
    int line_count = options.line_start_zero ? 0 : 1;
    int printed_lines = 0;

    fseek(file, 0, SEEK_SET); // Reset file pointer to the beginning

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (line_count >= start_line && (end_line == 0 || line_count <= end_line)) {
            char line_number_str[10];

            if (options.fill_zeros) {
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
