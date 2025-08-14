#include <stdio.h>
#include <stdlib.h>
#include "headers/common.h"
#include "libs/sha256.h"


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

void get_sha256(const char *input, char output[65])
{
    BYTE hash[SHA256_HASH_SIZE];
    SHA256_CTX ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, (const BYTE *)input, strlen(input));
    sha256_final(&ctx, hash);

    // convert hash to hex string
    for (int i = 0; i < SHA256_HASH_SIZE; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
}
