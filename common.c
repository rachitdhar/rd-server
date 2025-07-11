#include <stdio.h>
#include <stdlib.h>
#include "common.h"

STREAM_DATA* get_file_stream(char* filepath)
{
    FILE* fp = fopen(filepath, "rb");
    free(filepath);
    if (fp == NULL) return NULL;

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(filesize);
    if (!buffer) {
	fclose(fp);
	return NULL;
    }

    fread(buffer, 1, filesize, fp);
    fclose(fp);

    STREAM_DATA* sd = malloc(sizeof(STREAM_DATA));
    if (!sd) {
	free(buffer);
	return NULL;
    }
    sd->filesize = filesize;
    sd->buffer = buffer;
    return sd;
}
