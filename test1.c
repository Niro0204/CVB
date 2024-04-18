/*
=======================================================================
program name: cvb.c
author: Anel Sivic und Sebastian Maier
program content: Schreiben Sie ein C-Konsolenprogramm, das eine mini
shell Anwendung darstellt
date: 22.03.2024
=======================================================================
*/

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

// check on which operating system the user is working
#ifdef __linux__
#define OS_LINUX 1
#endif

#ifdef __WIN32__
#define OS_LINUX 0
#endif

// IMPORTANT: we are using argc and argv
int end = 0;
#define MAXCMD 128
char cmd[MAXCMD]; /* enthaelt eingelesenes Kommando */

// Functions
void print_help();
void printVersion();
void macher(int argc, char *argv[], int boolean);
void printFileInfo(FILE *sourcefile);

int main(int argc, char *argv[])
{
    // getting user input
    // Checks if enough arguments are passed
    if (argc < 2)
    {
        fprintf(stderr, "--ERROR: Too few arguments--\n");
        return 1;
    }

    // boolean to check if '-' was entered
    int boolean = 0;
    // char array to store the options from the user
    // char option[5];
    // int count = 0;

    macher(argc, argv, boolean);

    return 0;
}

// this function gets the number of element passed by the user == argc
// the whole input == argv[]
// a boolean if the user used OPTIONs
void macher(int argc, char *argv[], int boolean)
{
    FILE *sourcefile = NULL;
    int startLine = 1, endLine = -1, lineNumberWidth = 0, q = 0, v = 0, n_R = 0, n_0 = 0, n_L = 0, n_N = 0, currentLine = 1, ui = 3;
    char temp[1000];

    // looping through the input
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            print_help();
        }
        else if (strcmp(argv[i], "--version") == 0)
        {
            printVersion();
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            startLine = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            endLine = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            i++;
            for (int j = 0; argv[i + j] != NULL; j++)
            {
                if (argv[i + j][0] == 'R')
                {
                    n_R = 1; // set var for formatting later
                    if (isdigit(argv[i + j][1]))
                    {
                        ui = atoi(argv[i + j] + 1);
                        if (ui <= 3 || ui >= 9)
                        {
                            fprintf(stderr, "Format size is either too big or too small!\n");
                            exit(1);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Format specifier not correct!\n");
                        exit(1);
                    }
                }
                else if (argv[i + j][0] == '0')
                {
                    n_0 = 1; // set var for formatting later
                    if (isdigit(argv[i + j][1]))
                    {
                        ui = atoi(argv[i + j] + 1);
                        if (ui <= 3 || ui >= 9)
                        {
                            fprintf(stderr, "Format size is either too big or too small!\n");
                            exit(1);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Format specifier not correct!\n");
                        exit(1);
                    }
                }
                else if (argv[i + j][0] == 'L')
                {
                    n_L = 1; // set var for formatting later
                    if (isdigit(argv[i + j][1]))
                    {
                        ui = atoi(argv[i + j] + 1);
                        if (ui <= 3 || ui >= 9)
                        {
                            fprintf(stderr, "Format size is either too big or too small!\n");
                            exit(1);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Format specifier not correct!\n");
                        exit(1);
                    }
                }
                else if (argv[i + j][0] == 'N')
                {
                    n_N = 1; // set var for formatting later
                    if (isdigit(argv[i + j][1]))
                    {
                        ui = atoi(argv[i + j] + 1);
                        if (ui <= 3 || ui >= 9)
                        {
                            fprintf(stderr, "Format size is either too big or too small!\n");
                            exit(1);
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Format specifier not correct!\n");
                        exit(1);
                    }
                }
            }
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            v = 1;
        }
        else if (strcmp(argv[i], "-q") == 0)
        {
            q = 1;
        }
        else if (strcmp(argv[i], "-") == 0)
        {
            // Read from stdin if '-' is passed
            sourcefile = stdin;
            break;
        }
        else
        {
            sourcefile = fopen(argv[i], "r");
            if (sourcefile == NULL)
            {
                fprintf(stderr, "Error: Cannot open file %s\n", argv[i]);
                exit(1);
            }
        }
    }

    while ((fgets(temp, 1000, sourcefile) != NULL))
    {
        // print fileinfo and break before sourcefileoutput
        if (q == 1)
        {
            printFileInfo(sourcefile);
            break;
        }

        // start from startline till endline
        // or start from startline till EOF => endline==-1
        if (currentLine >= startLine && (endLine == -1 || currentLine <= endLine))
        {
            if (n_R == 1)
            {
                fprintf(stdout, "%*d ", ui, currentLine); // right aligned (default)
            }
            else if (n_0 == 1)
            {
                fprintf(stdout, "%0*d ", ui, currentLine); // right aligned with leading 0
            }
            else if (n_L == 1)
            {
                fprintf(stdout, "%-*d ", ui, currentLine); // left aligned
            }
            else if (n_N == 1)
            {
                fprintf(stdout, "%*d ", ui, currentLine - 1); // line number starts with 0
            }
            fprintf(stdout, "%s", temp);
        }
        currentLine++;
    }

    // print fileinfo after output
    if (v == 1)
    {
        printFileInfo(sourcefile);
    }

    fclose(sourcefile);
}

void printFileInfo(FILE *sourcefile)
{
    // check which os the user is using
    if (OS_LINUX)
    {
        // stat prints info of file
        struct stat sfile;
        if (fstat(fileno(sourcefile), &sfile) == -1)
        {
            fprintf(stderr, "Error occured");
        }

        fprintf(stdout, "\n|File st_uid %d| \n", sfile.st_uid);
        fprintf(stdout, "|File st_size %ld| \n", sfile.st_size);
    }
    else if (!OS_LINUX)
    {
        // get size
        fseek(sourcefile, 0, SEEK_END);
        long int size = ftell(sourcefile);
        fseek(sourcefile, 0, SEEK_SET);

        fprintf(stdout, "\n|File size: %ld|\n", size);
    }
    else
    {
        fprintf(stderr, "Error reading operating system!");
        exit(1);
    }
}

void print_help()
{
    char help[] =
        "\nCVB::USER HELP\n \
--help\t\t............. Hilfeinformation wird ausgegeben; Programm terminiert\n \
-s [n]\t\t............. Angabe der ersten auszugebenen (n-te) Zeile; wenn keine Angabe, dann wird ab erster Zeile gelesen\n \
-e [n]\t\t............. Angabe der letzten auszugebenen (m-te) Zeile; wenn keine Angabe, dann wird bis zur letzten Zeile gelesen\n \
-n [format]\t............. Ausgabe mit fuehrenden Zeilennummern; Formatparameter hierfuer:\n\t\t\t\tuint Breite des Zeilennummernfelds (numerisch)\n\t\t\t\tR rechtsbuendig (default) \n\t\t\t\t0 rechtsbuendig mit fuehrenden Nullen\n\t\t\t\tL linksbuendig\n\t\t\t\tN die Zeilennummer beginnt mit Null\n\t\t\t\tBeispiel: -n 04 - Ausgabe der Zeilennummer rechtsbuendig mit fuehrenden Nullen, z.B. 0017\n \
-v\t\t............. Programm gibt waehrend der Prozessierung etwaige zusaetzliche Status- und Dateiinformationen aus\n \
-q\t\t............. Programm gibt nur etwaige zusaetzliche Status- und Dateiinformationen aus, ohne jedoch den Inhalt der Datei auszugeben\n \
--version\t............. Versionsinformation des Programms wird ausgegeben\n\n";
    fprintf(stdout, "%s", help);
}

void printVersion()
{
    fprintf(stdout, "CVB version 1.0");
    exit(0);
}