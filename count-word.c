/*
    Word count challenge
    Neko Life
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 2048
#define MAX_WORD_LENGTH 256

#define CLEAR(x) memset(x, '\0', sizeof(x) / sizeof(x[0]))

struct result_t
{
    char word[MAX_WORD_LENGTH];
    ssize_t count;
};

static const char PUNCTUATION[] =
{
    ';',
    ',',
    '?',
    ':',
    '\'',
    '!',
    '-',
    '~',
    '.',
    '"',
    '(',
    ')',
    '{',
    '}',
    '[',
    ']',
    '/',
    '\\',
    '&',
};

static const char TOKENS[] =
{
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
};

void
print_separator ()
{
    printf("\n");
    for (int i = 0; i < 50; i++)
        printf("=");
    printf("\n");
}

int
is_punct (char token)
{
    for (int i = 0; i < (sizeof(PUNCTUATION) / sizeof(PUNCTUATION[0])); i++)
        {
            if (PUNCTUATION[i] == token) return 1;
        }

    return 0;
}

int
valid_token (char token)
{
    for (int i = 0; i < (sizeof(TOKENS) / sizeof(TOKENS[0])); i++)
        {
            if (TOKENS[i] == token) return 1;
        }

    if (is_punct(token))
        return 1;

    return 0;
}

ssize_t
find_result_index (char *word, struct result_t results[])
{
    ssize_t i = 0;
    while (strlen(results[i].word) > 0)
        {
            if (!strcmp(word, results[i].word))
                {
                    return i;
                }
            i++;
        }
    
    return -1;
}

ssize_t
remove_puncts (char *buf_temp, ssize_t buf_temp_index)
{
    char new_buf_temp[BUFFER_SIZE + 1] = {0};
    ssize_t new_buf_temp_index = 0;

    for (ssize_t i = 0; i < buf_temp_index; i++)
        {
            if (is_punct(buf_temp[i])) continue;

            new_buf_temp[new_buf_temp_index++] = buf_temp[i];
        }

    memcpy(buf_temp, new_buf_temp, new_buf_temp_index);
    buf_temp[new_buf_temp_index] = '\0';

    return new_buf_temp_index;
}

int
main (const int argc, const char *argv[])
{
    FILE *input_file = {0};
    int has_input_file = 0;

    if (argc == 2)
        {
            has_input_file = 1;
            input_file = fopen(argv[1], "r");

            if (!input_file)
                {
                    fprintf(stderr, "No such file\n");
                    return 1;
                }
        }

    struct result_t results[BUFFER_SIZE] = {0};
    ssize_t results_len = 0;

    char buf[BUFFER_SIZE] = {0};

    char buf_last_temp[BUFFER_SIZE + 1] = {0};
    ssize_t last_buf_len = 0;


    ssize_t result = 0;

    while ((result = read(has_input_file ? fileno(input_file) : STDIN_FILENO, (void *)buf, BUFFER_SIZE)) > 0)
        {
            ssize_t index = 0;

            while (index < result)
                {
                    if (results_len == BUFFER_SIZE) break;

                    ssize_t buf_temp_index = 0;

                    char buf_temp[BUFFER_SIZE + 1] = {0};

                    if (last_buf_len)
                        {
                            memcpy(buf_temp, buf_last_temp, last_buf_len);
                            CLEAR(buf_last_temp);

                            buf_temp_index = last_buf_len;
                            last_buf_len = 0;
                        }

                    for (; index < result; index++)
                        {
                            char tkn = tolower(buf[index]);
                            if (valid_token(tkn))
                                {
                                    buf_temp[buf_temp_index++] = tkn;
                                    continue;
                                }

                            if (!buf_temp_index) continue;

                            break;
                        }

                    buf_temp_index = remove_puncts(buf_temp, buf_temp_index);

                    if (!buf_temp_index) continue;
                        
                    if (index == BUFFER_SIZE)
                        {
                            memcpy(buf_last_temp, buf_temp, buf_temp_index);
                            last_buf_len = buf_temp_index;
                            continue;
                        }

                    ssize_t result_index = find_result_index(buf_temp, results);

                    if (result_index == -1)
                        {
                            memcpy(results[results_len].word, buf_temp, sizeof(results[results_len].word) / sizeof(results[results_len].word[0]));
                            results[results_len].count = 1;
                            results_len++;
                            continue;
                        }

                    results[result_index].count++;
                }
        }

    if (has_input_file) fclose(input_file);

    for (ssize_t i = 0; i < results_len; i++)
        {
            printf("%s: %ld\n", results[i].word, results[i].count);
        }

    print_separator();
    printf("%ld words indexed\n", results_len);

    return 0;
}
