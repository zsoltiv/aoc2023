#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#define ELEMS(x) (sizeof((x)) / sizeof((x)[0]))

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

int lowest(uintptr_t *wordpositions, size_t npositions)
{
    int lowi = 0;
    for(int i = 0; i < npositions; i++)
        if((wordpositions[i] && wordpositions[i] < wordpositions[lowi]) || wordpositions[lowi] == 0) {
            lowi = i;
        }
    printf("\t%d\n", lowi);
    return lowi;
}

int highest(uintptr_t *wordpositions, size_t npositions)
{
    int highi = 0;
    for(int i = 0; i < npositions; i++)
        if((wordpositions[i] && wordpositions[i] > wordpositions[highi]) || wordpositions[highi] == 0)
            highi = i;
    printf("\t%d\n", highi);
    return highi;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

unsigned long process_line2(char *data)
{
    static char *words[9] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
    };
    // TODO a legszelso occurence kell ezekbol is; lehet beloluk tobb egy sorban
    static struct {
        char *word, *num;
    } word_nums[9] = {
        {"one", "1"},
        {"two", "2"},
        {"three", "3"},
        {"four", "4"},
        {"five", "5"},
        {"six", "6"},
        {"seven", "7"},
        {"eight", "8"},
        {"nine", "9"},
    };
    uintptr_t wordptrs[9] = {0};
    char numstring[3] = {0};
    puts(data);
    char *ptr;
    for(int i = 0; i < ELEMS(wordptrs); i++) {
        wordptrs[i] = (uintptr_t)strstr(data, words[i]);
    }
    for(ptr = data; !isdigit((*ptr)); ptr++);
    int lowi = lowest(wordptrs, ELEMS(wordptrs));
    uintptr_t lowestptr = wordptrs[lowi];
    printf("\tlow %p %p\n", lowestptr, ptr);
    if(lowestptr)
        numstring[0] = lowestptr < ptr ? word_nums[lowi] : *ptr;
    else
        numstring[0] = *ptr;
    for(ptr = data + strlen(data); !isdigit((*ptr)); ptr--);
    int highi = highest(wordptrs, ELEMS(wordptrs));
    uintptr_t highestptr = wordptrs[highi];
    printf("\thigh %p %p\n", highestptr, ptr);
    if(highestptr)
        numstring[1] = highestptr > ptr ? word_nums[highi] : *ptr;
    else
        numstring[0] = *ptr;

    puts(numstring);

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
    unsigned long sum = 0;
    unsigned long sum2 = 0;
    do {
        sum += process_line(line);
        sum2 += process_line2(line);
    } while((line = strtok(NULL, "\n")));
    printf("%lu\n", sum);
    printf("%lu\n", sum2);
}
