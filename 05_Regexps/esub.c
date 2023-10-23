#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void print_error(int error_code, const regex_t *regex)
{
    size_t buf_size = regerror(error_code, regex, NULL, 0ul);
    char *err_buf = malloc(buf_size);
    regerror(error_code, regex, err_buf, buf_size);

    fprintf(stderr, "Regex error: %s\n", err_buf);
    free(err_buf);
}

enum {
    REALLOC_STEP = 32
};

static const char SUB_REGEX_STR[] = "\\\\[0-9][0-9]*";
static regex_t sub_regex_comp;
static bool _sub_regex_compiled = false;

void compile_sub_regex()
{
    int error_code;
    if((error_code = regcomp(&sub_regex_comp, SUB_REGEX_STR, 0))) {
        print_error(error_code, &sub_regex_comp);
        exit(1);
    }
    _sub_regex_compiled = true;
}

void free_sub_regex()
{
    if(_sub_regex_compiled)
        regfree(&sub_regex_comp);
    _sub_regex_compiled = false;
}

unsigned sub_parse_count(const char *substitution)
{
    regmatch_t  pmatch[1];
    const char *ptr = substitution;

    unsigned counter = 0;

    for(;; ++counter) {
        if(regexec(&sub_regex_comp, ptr, 1, pmatch, 0)) {
            return counter;
        }
      //  printf("%s\n", ptr);
        ptr += pmatch[0].rm_eo;

    }
}

unsigned sub_find_next(const char *substitution, regmatch_t *pmatch)
{
    const char *ptr = substitution;

    if(!regexec(&sub_regex_comp, ptr, 1, pmatch, 0)) {
        const char *ptr = substitution + pmatch->rm_so + 1;
        long idx;
        size_t alloc_size = pmatch->rm_eo - pmatch->rm_so;
        char *tmp = malloc(alloc_size);
        strncpy(tmp, ptr, alloc_size - 1);
        //printf("%s\n", tmp);
        idx = atol(tmp);
        free(tmp);
        if(idx < 0) {
            return 0;
        }
        return idx;
    }
    return 0;
}

size_t substitute(char **dst, size_t dst_size, regoff_t start, regoff_t end, const char *src, size_t n) {
    const size_t diff = n - (end - start);

    if(diff > 0) {
        *dst = realloc(*dst, dst_size += ((diff / REALLOC_STEP) + 1) * REALLOC_STEP);
    }

    char *postfix = *dst + end;
    memmove(postfix + diff, postfix, strlen(*dst) - end + 2); //null and start

    memcpy(*dst + start, src, n);

    return dst_size;
}

size_t substitute_string(char **dst, size_t dst_size, const char *src, const regmatch_t *match, unsigned sub_index)
{
    const regoff_t src_start = match->rm_so;
    const regoff_t src_end = match->rm_eo;
    const regoff_t src_len = src_end - src_start;


    regmatch_t sub_pmatch;
    unsigned s;
    if((s = sub_find_next(*dst, &sub_pmatch)) != sub_index) {
        fprintf(stderr, "Unknown substitution: %u\n", s);
        free(*dst);
        return 0;
    };

    return substitute(dst, dst_size, sub_pmatch.rm_so, sub_pmatch.rm_eo, src + src_start, src_len);
}

int main(int argc, char **argv)
{
    if(argc < 4) {
        return 1;
    }
    const char *regexp = argv[1];
    const char *substitution = argv[2];
    const char *str = argv[3];

    size_t dst_size = strlen(str) + 1;
    char *dst = malloc(strlen(str) + 1);
    strcpy(dst, str);

    atexit(free_sub_regex);
    compile_sub_regex();

    const unsigned pmatch_size = sub_parse_count(substitution) + 1;
    if(pmatch_size > 1) {
        regmatch_t *pmatch = malloc(pmatch_size * sizeof(regmatch_t));
        regex_t regex;
        int error_code;
        if((error_code = regcomp(&regex, regexp, REG_EXTENDED))) {
            print_error(error_code, &regex);
            regfree(&regex);
            return 1;
        }


        if(!regexec(&regex, str, pmatch_size, pmatch, 0)) {
            size_t buf_size = strlen(substitution) + 10;
            char *buf = malloc(buf_size);
            strcpy(buf, substitution);

            for(unsigned i = 1; i < pmatch_size; ++i) {
                if((buf_size = substitute_string(&buf, buf_size, str, pmatch + i, i)) == 0) {
                    regfree(&regex);
                    free(pmatch);
                    free(dst);
                    return 1;
                }
            }
            substitute(&dst, dst_size, pmatch[0].rm_so, pmatch[1].rm_eo, buf, strlen(buf));
            free(buf);
        }


        regfree(&regex);
        free(pmatch);
    } 
    printf("%s\n", dst);
    free(dst);
    return 0;
} 

