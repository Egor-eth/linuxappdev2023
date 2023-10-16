#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

struct Range {
    int start;
    int stop;
    int step;
    int _pos;
};
typedef struct Range range;

void argparse(int argc, char **argv, int *start, int *end, int *step)
{
    *step = 1;
    switch(argc) {
    case 4:
        *step = atoi(argv[3]);
    case 3:
        *end = atoi(argv[2]);
        *start = atoi(argv[1]);
        break;
    case 2:
        *step = 1;
        *end = atoi(argv[1]);
        *start = 0;
        break;
    default:
        printf(
            "Usage:\t./range N - prints [0, ... N-1]\n"
            "\t./range M N - prints [M, ..., N-1]\n"
            "\t./range M N S - prints [M, M + S, M + 2S, ..., N-1]\n"
            );
        exit(0);
    }
}

void range_init(range *r)
{
    r->_pos = r->start;
}

bool range_run(range *r)
{
    return r->_pos < r->stop;
}

void range_next(range *r)
{
    r->_pos += r->step;
}

int range_get(range *r)
{
    return r->_pos;
}

int main(int argc, char *argv[]) {
    range I;
    argparse(argc, argv, &I.start, &I.stop, &I.step);
    for(range_init(&I); range_run(&I); range_next(&I))
        printf("%d\n", range_get(&I));
    return 0;
}
