#include "regex_helper.h"
#include <string.h>
#include <regex.h>

void get_substr(char* string, regmatch_t match, char* output) {
    strncpy(output, string + match.rm_so, match.rm_eo - match.rm_so); 
    output[match.rm_eo-match.rm_so] = '\0';
}
