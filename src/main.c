/*
    File: main.c
    Author: Philip Haynes
    This is a program that takes in a binary file
    and embeds it in a c file as a const char array.

    This work is licensed under an MIT license.

    Copyright 2019 Philip Haynes

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>

void help() {
    printf(
        "Usage:\n"
        "file_to_header name_of_binary_file name_of_header.c\n"
    );
}

int main(int argumentCount, char** arguments) {
    if (argumentCount != 3) {
        help();
        return 0;
    }

    FILE *fileIn = fopen(arguments[1], "rb");
    if (!fileIn) {
        printf("Failed to open %s for reading.\n", arguments[1]);
        return 0;
    }
    fseek(fileIn, 0, SEEK_END);
    long fileSize = ftell(fileIn);
    if (fileSize >= 2147483648) {
        printf("File is too large! Please don't exceed 2 gigabytes for the love of god!\n");
        fclose(fileIn);
        return 0;
    }
    fseek(fileIn, 0, SEEK_SET);
    FILE *fileOut = fopen(arguments[2], "w");
    if (!fileOut) {
        printf("Failed to open %s for writing.\n", arguments[2]);
        fclose(fileIn);
        return 0;
    }
    char *buffer = (char*)malloc(fileSize);
    if ((size_t)fileSize != fread(buffer, 1, fileSize, fileIn)) {
        printf("There was a problem reading the file.\n");
        fclose(fileIn);
        fclose(fileOut);
        free(buffer);
        return -1;
    }
    fclose(fileIn);

    // Get rid of any directory listings
    for (int i = 0; arguments[2][i] != 0; i++) {
        if (arguments[2][i] == '/' || arguments[2][i] == '\\') {
            arguments[2] += i+1;
            i = 0;
        }
    }
    // Get rid of the ".c" for the array name
    for (int i = 0; arguments[2][i] != 0; i++) {
        if (arguments[2][i] == '.') {
            arguments[2][i] = 0;
            break;
        }
    }

    int warnSpace = 0;
    for (int i = 0; arguments[2][i] != 0; i++) {
        if (arguments[2][i] == ' ') {
            arguments[2][i] = '_';
            warnSpace = 1;
        }
    }
    if (warnSpace) {
        printf("Warning: Output filename has spaces. "
            "Spaces will be replaced with underscores in the array name.\n");
    }

    fprintf(fileOut,
        "const int %s_length = %i;\n"
        "const char %s[%s_length] = {",
        arguments[2], (int)fileSize, arguments[2], arguments[2]);

    for (int i = 0; i < fileSize; i++) {
        if (i % 16 == 0) {
            fprintf(fileOut, "\n\t%i, ", (int)buffer[i]);
        } else if (i == fileSize-1) {
            fprintf(fileOut, "%i", (int)buffer[i]);
        } else {
            fprintf(fileOut, "%i, ", (int)buffer[i]);
        }
    }

    fprintf(fileOut, "\n};\n");
    fclose(fileOut);
    free(buffer);

    return 0;
}
