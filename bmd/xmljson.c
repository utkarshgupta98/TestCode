#include<stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create a Payload file for a sender */
void xml2json(char Pay[], char Sender[]) {
    char * filename;
    strcpy(filename, Sender);
    strcat(filename, "Payload.json");
    FILE * file;
    file = fopen(filename, "w");

    if (file == NULL) {
        printf("file opening failed");
        exit(0);
    }

    /* Writes into json file */
    fprintf(file, "{\n \"Payload\":\"%s\"\n}", Pay);

    printf("\nPayload json File created\n");

    /* Closes file */
    fclose(file);
}

/* Get the filesize for a given filename */
int get_filesize(char filename[]) {
    /*File size validation*/
    FILE * fp;
    int size = 0;

    fp = fopen(filename, "r");
    if (fp == NULL)
        printf("\nFile unable to open ");
    else
        printf("\nFile opened ");
    fseek(fp, 0, 2); /* file pointer at the end of file */
    size = ftell(fp); /* take a position of file pointer un size variable */

    /*Check if Filesize is larger than 5MB*/
    if (size > 5000000)
        printf("\nFile size larger than 5MB not allowed!\n");

    /*close the file*/
    fclose(fp);
    /*return filesize*/
    return size;
}

/* Get the filecontents for a given filename */
char * get_filecontents(char filename[]) {
    long f_size;
    char * str_data;
    size_t file_size, result;
    FILE * fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);

    /* Reads the size of the file */
    f_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    file_size = sizeof(char) * f_size;
    str_data = malloc(file_size);

    /* fread returns number of items actually read. */
    result = fread(str_data, 1, f_size, fp);
    return strdup(str_data);

}

/* This function is defined for testing */
char * xmltojson(char Pay[]) {
    char filename[] = "Payload.json";
    FILE * file;
    file = fopen(filename, "w");

    if (file == NULL) {
        printf("file opening filed");
        exit(0);
    }

    /* Writes into json file */
    fprintf(file, "{\n \"Payload\":\"%s\"\n}", Pay);

    printf("\nPayload json File created\n");

    /* Closes file */
    fclose(file);

    /* returns json filename */
    return strdup(filename);
}
