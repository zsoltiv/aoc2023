#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#define ELEMS(x) (sizeof((x)) / sizeof((x)[0]))

struct mapping {
    char *word, num;
};

unsigned long process_line(char *data)
{
    char *ptr;
    char numstring[3] = {0};
    for(ptr = data; !isdigit((*ptr)); ptr++);
    numstring[0] = *ptr;
    for(ptr = data + strlen(data); !isdigit((*ptr)); ptr--);
    numstring[1] = *ptr;

    return strtoul(numstring, NULL, 10);
}

struct mapping *first_mapping(const char *line, struct mapping *mappings, size_t nmappings, uintptr_t *outpos)
{
    int firsti = -1;
    *outpos = UINTPTR_MAX;
    for(int i = 0; i < nmappings; i++) {
        uintptr_t pos = (uintptr_t)strstr(line, mappings[i].word);
        if(pos && pos < (*outpos)) {
            *outpos = pos;
            firsti = i;
        }
    }

    if(firsti < 0)
        return NULL;
    else
        return &mappings[firsti];
}

struct mapping *last_mapping(const char *line, struct mapping *mappings, size_t nmappings, uintptr_t *outpos)
{
    *outpos = 0;
    for(const char *ptr = line + strlen(line); ptr != line; ptr--) {
        for(int i = 0; i < nmappings; i++) {
            uintptr_t pos = (uintptr_t)strstr(ptr, mappings[i].word);
            if(pos) {
                *outpos = pos;
                return &mappings[i];
            }
        }
    }

    return NULL;
}

unsigned long process_line2(char *data)
{
    static struct mapping word_nums[9] = {
        {"one", '1'},
        {"two", '2'},
        {"three", '3'},
        {"four", '4'},
        {"five", '5'},
        {"six", '6'},
        {"seven", '7'},
        {"eight", '8'},
        {"nine", '9'},
    };
    uintptr_t wordptrs[9] = {0};
    char numstring[3] = {0};
    char *ptr;
    uintptr_t firstpos = 0, lastpos = 0;
    for(ptr = data; *ptr && !isdigit((*ptr)); ptr++);
    struct mapping *first = first_mapping(data, word_nums, ELEMS(word_nums), &firstpos);
    if(first) {
        if(firstpos < (uintptr_t)ptr) {
            numstring[0] = first->num;
        } else {
            numstring[0] = *ptr;
            first = NULL;
            firstpos = 0;
        }
    } else
        numstring[0] = *ptr;
    for(ptr = data + strlen(data) - 1; *ptr && !isdigit((*ptr)); ptr--);
    struct mapping *last = last_mapping(data, word_nums, ELEMS(word_nums), &lastpos);
    if(last && lastpos != firstpos) {
        numstring[1] = lastpos > (uintptr_t)ptr ? last->num : *ptr;
    } else
        numstring[1] = *ptr;

    return strtoul(numstring, NULL, 10);
}

int main(int argc, char **argv)
{
    if(argc != 2) return 1;

    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data = calloc(sz + 1, 1);
    fread(data, 1, sz, f);
    fclose(f);

    char *line = strtok(data, "\n");
    unsigned long sum = 0, sum2 = 0;
    do {
        sum += process_line(line);
        sum2 += process_line2(line);
    } while((line = strtok(NULL, "\n")));
    printf("%lu\n", sum);
    printf("%lu\n", sum2);
}
