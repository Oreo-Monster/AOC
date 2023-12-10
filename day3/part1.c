#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include "regex_helper.h"

int is_symbol(char c) {
    return c!='.' && !isdigit(c) && c!='\n' && c!='\0';
}
int check_line(regoff_t so, regoff_t eo, char* line){
    for(int i = so-1; i <= eo; i++){
        if(is_symbol(*(line+i))) return 1;
    }
    return 0; //no symbols found
}
int is_partnum(regoff_t so, regoff_t eo, char *prev_line, char *current_line, char *next_line) {
    if(prev_line != NULL && check_line(so, eo, prev_line)) return 1;
    if(check_line(so, eo, current_line)) return 1;
    if(next_line != NULL && check_line(so, eo, next_line)) return 1;
    return 0;
}


int main(int argc, char* argv[]) {
	if(argc != 2){
		printf("Include a file to parse in cmd line\n");
		return 1;
	}

    //Compile regex
    regex_t pattern;
    regex_t symbol_pattern;
    regmatch_t match[2];
    int regex_flag;

    regex_flag = regcomp(&pattern, 
            "([[:digit:]]+)",
            REG_EXTENDED);
    if(regex_flag) {
        printf("Regex compolation error for cube pattern");
        return 1;
    }

    //File handlers
    size_t len = 0;
	FILE *fptr;
    ssize_t read;
	if((fptr = fopen(argv[1], "r"))== NULL) {
		printf("Error, file %s can not be read\n", argv[1]);
        exit(EXIT_FAILURE);
	}

    //Loop variables
    int total; //sum of all part numbers
    regoff_t offset = 0;
    char* part_num = (char*) malloc(20 * sizeof(char));
    char* prev_line = NULL;
    char* next_line = NULL;
    char* current_line = NULL;
    //Read in first line to current to start loop:
    getline(&current_line, &len, fptr);
    while (current_line != NULL) {
        read = getline(&next_line, &len, fptr);
        if(read < 2) {
            next_line = NULL; //empty line
        }
        printf("--------\n");
        
        if(prev_line != NULL) printf("%s", prev_line);
        printf("%s", current_line);
        if(next_line != NULL) printf("%s", next_line);
        //Reset inner loop vars
        offset = 0;  
        while(regexec(&pattern, current_line+offset, 2, match, 0) == 0) {
            if(is_partnum(match[0].rm_so+offset, match[0].rm_eo+offset, prev_line, current_line, next_line)){
                get_substr(current_line+offset, match[0], part_num);
                printf("Found partnum: %s\n", part_num);
                total += atoi(part_num);
            }
            offset += match[0].rm_eo + 1;
        }

        //Line shuffle
        free(prev_line);
        prev_line = current_line;
        current_line = next_line;
        next_line = NULL;
    }    
    printf("Final total: %d\n", total);
    free(part_num);
	fclose(fptr);
    regfree(&pattern);
    
	return 0;
}

