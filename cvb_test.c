#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

typedef struct {
    int width;
    bool align_right;
    bool fill_zeros;
    bool line_start_zero;
} formatting;

void print_help();
void print_version();
formatting handle_format(const char *format);

int main(int argc, char *argv[]) {
    int start_line = 0;
    int end_line = 0;
    formatting format = {3, true, false, false}; // Default format
    FILE *file = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [OPTION]... [FILENAME]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Handle command line arguments
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "--version") == 0) {
            print_version();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            start_line = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            end_line = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            format = handle_format(argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            // Additional status and file information during processing (not implemented yet)
        } else if (strcmp(argv[i], "-q") == 0) {
            // Only print additional status and file information without printing file contents (not implemented yet)
        }
    }

    // Open the file or use stdin if "-" is provided as filename
    if (strcmp(argv[argc - 1], "-") == 0) {
        file = stdin;
    } else {
        file = fopen(argv[argc - 1], "r");
        if (file == NULL) {
            fprintf(stderr, "Failed to open file!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Skip lines until start_line is reached
    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && line_count < start_line) {
        line_count++;
    }

    // Print lines from start_line to end_line
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line_count++;
        if (line_count > end_line && end_line != 0) {
            break;
        }
        printf("%s\n", line);
    }

    // Close file if not stdin
    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

void print_help() {
    printf("Help information:\n"
           "--help             Print help information; program terminates\n"
           "--version          Print version information of the program\n"
           "-s n               Specify the first (n-th) line to be output\n"
           "-e n               Specify the last (m-th) line to be output\n"
           "-n [format]        Output with leading line numbers\n"
           "Optional format parameters:\n"
           "    uint           Width of line number field (numeric)\n"
           "    R              Right aligned (default)\n"
           "    0              Right-aligned with leading zeros\n"
           "    L              Left aligned\n"
           "    N              The line number starts with zero\n"
           "-v                 Print any additional status and file information during processing\n"
           "-q                 Print only any additional status and file information, but does not print the contents of the file\n");
}

void print_version() {
    printf("cvb version: 1.0\n");
}

formatting handle_format(const char *format) {
    formatting options = {3, true, false, false}; // Default options

    if (format != NULL) {
        for (int i = 0; format[i] != '\0'; i++) {
            switch (format[i]) {
                case 'R':
                    options.align_right = true;
                    break;
                case 'L':
                    options.align_right = false;
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
