#include <stdio.h>

int main() {
    int error;
    FILE *scores_file;
    /*
    fprintf writes formatted text to the output stream you specify.
    printf is equivalent to writing fprintf(stdout, ...) and writes formatted 
    text to wherever the standard output stream is currently pointing.
    */

    /*
    fopen(char * file_name,char * mode)
    mode can be :
    "r"     read
    "w"     empty the file, write from the first byte
    "a"     stands for appending a file
    "rb"    read a binary file
    */

    scores_file = fopen("top10.txt", "r");
    if (scores_file == NULL) {//check if fopen runs without an error
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    printf("File opened: we can use it here\n");

    error = fclose(scores_file);
    if (error != 0) {//check if the file is closed sucessfully
        fprintf(stderr, "fclose failed\n");
        return 1;
    }

    return 0;
}