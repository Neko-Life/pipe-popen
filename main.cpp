/*
    Pipe Exercise
    Neko Life
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE BUFSIZ * 2
#define MUSIC_FILE "'my music.opus'"

void
printbuf (char *buf, ssize_t siz)
{
    for (ssize_t i = 0; i < siz; i++)
        {
            putchar(buf[i]);
        }
}

int
main ()
{
    // fixed length buffer works just fine and secure
    char buf[BUFFER_SIZE];
    // make sure to clean all the garbage in `buf`
    memset(buf, '\0', sizeof(buf) / sizeof(buf[0]));

    /**
     * The popen function run command in specified mode like
     * running cmd in the terminal. We(I) cannot determine
     * if the pipe actually open and running
     * as expected unless maybe run the command twice to check
     * the first popen call status code by closing it and
     * proceed with the second call if the first call is not
     * an error status code (0). Kinda tricky and unreliable.
     */

    // open the command to read opus music format in read mode
    FILE *decode_cmd = popen("opusdec " MUSIC_FILE " -" /* " --packet-loss 20" */, "r");

    // check if command open
    if (!decode_cmd)
        {
            pclose(decode_cmd);
            fprintf(stderr, "popen(): decode_cmd\n");
            return 1;
        }

    // open the command to play pcm data to speaker in write mode
    FILE *play_cmd = popen("aplay -f dat -", "w");

    // check if command open
    if (!play_cmd)
        {
            pclose(decode_cmd);
            pclose(play_cmd);

            fprintf(stderr, "popen(): play_cmd\n");
            return 1;
        }

    /**
     * Stores the read amount, we should process this
     * many byte exactly since the data stored in
     * `buf` would be exactly this length
     */
    ssize_t result = 0;

    //          read the data from decode command `BUFFER_SIZE`
    //          byte at a time and store it in `buf`
    while ((result = read(fileno(decode_cmd), (void *)buf, BUFFER_SIZE)) > 0)
        {
            // write the data from `buf` to `play_cmd` exactly `result` bytes
            write(fileno(play_cmd), (void *)buf, result);

            // printbuf(buf, result);
        }

    // clean ups
    pclose(decode_cmd);
    pclose(play_cmd);

    return 0;
}
