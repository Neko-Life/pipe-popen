#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE BUFSIZ * 2
#define MUSIC_FILE "'my music.opus'"

void printbuf(char *buf, ssize_t siz) {
    for (ssize_t i = 0; i < siz; i++) {
        putchar(buf[i]);
    }
}

int main() {
    // Fixed length buffer works fine, but use memset to clear it securely
    char buf[BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));

    // Open the command to read opus music format in read mode
    FILE *decode_cmd = popen("opusdec " MUSIC_FILE " -", "r");
    if (!decode_cmd) {
        fprintf(stderr, "Failed to open decode command\n");
        return 1;
    }

    // Open the command to play pcm data to speaker in write mode
    FILE *play_cmd = popen("aplay -f dat -", "w");
    if (!play_cmd) {
        fprintf(stderr, "Failed to open play command\n");
        pclose(decode_cmd);
        return 1;
    }

    ssize_t result = 0;
    while ((result = fread(buf, 1, BUFFER_SIZE, decode_cmd)) > 0) {
        if (fwrite(buf, 1, result, play_cmd) != result) {
            fprintf(stderr, "Failed to write to play command\n");
            break;
        }
    }

    // Check for read errors or incomplete data
    if (ferror(decode_cmd)) {
        fprintf(stderr, "Error occurred while reading from decode command\n");
    }

    // Check for write errors
    if (ferror(play_cmd)) {
        fprintf(stderr, "Error occurred while writing to play command\n");
    }

    // Close the commands and check for errors
    if (pclose(decode_cmd) != 0) {
        fprintf(stderr, "Error occurred while closing decode command\n");
    }

    if (pclose(play_cmd) != 0) {
        fprintf(stderr, "Error occurred while closing play command\n");
    }

    return 0;
}
