#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include "regex_helper.h"

void check_max(int number, char *color, int *red_max, int *blue_max, int *green_max) {
    if(strcmp(color, "red") == 0){
        if(number > *red_max) *red_max = number;
        return;
    }
    if(strcmp(color, "blue") == 0){
        if(number > *blue_max) *blue_max = number;
        return;
    }
    if(strcmp(color, "green") == 0){
        if(number > *green_max) *green_max = number;
    }
}


int main(int argc, char* argv[]) {
	if(argc != 2){
		printf("Include a file to parse in cmd line\n");
		return 1;
	}

    //Compile regex
    regex_t cube_pattern;
    regmatch_t match[4]; 
    int regex_flag;

    regex_flag = regcomp(&cube_pattern, 
            "([[:digit:]]+) (blue|red|green)",
            REG_EXTENDED);
    if(regex_flag) {
        printf("Regex compolation error for cube pattern");
        return 1;
    }

    //File handlers
	char *line = NULL;
    size_t len = 0;
	FILE *fptr;
    ssize_t read;
	if((fptr = fopen(argv[1], "r"))== NULL) {
		printf("Error, file %s can not be read\n", argv[1]);
        exit(EXIT_FAILURE);
	}

    //Loop variables
    int total; //sum of all valid game indexes
    char* num_blocks = (char*) malloc(5*sizeof(char));
    char* color = (char*) malloc(6*sizeof(char)); //green is longest color
    regoff_t offset = 0;
    int* red_max = calloc(1, sizeof(int));
    int* green_max = calloc(1, sizeof(int));
    int* blue_max = calloc(1, sizeof(int));
    while ((read = getline(&line, &len, fptr)) != -1) {
        if(read < 2) {
            continue; //empty line
        }    
        printf("--------");
        printf("%s", line);
        //Reset inner loop vars
        offset = 0;  
        *red_max = 0;
        *green_max = 0;
        *blue_max = 0;
        while(regexec(&cube_pattern, line+offset, 4, match, 0) == 0) {
            get_substr(line+offset, match[1], num_blocks);
            get_substr(line+offset, match[2], color);
            check_max(atoi(num_blocks), color, red_max, blue_max, green_max); 
            offset += match[0].rm_eo + 1;
        }
        total += (*red_max) * (*blue_max) * (*green_max);
    }    
    printf("Final total: %d\n", total);
    free(num_blocks);
    free(color);
    free(red_max);
    free(green_max);
    free(blue_max);
	fclose(fptr);
    regfree(&cube_pattern);
    
	return 0;
}

