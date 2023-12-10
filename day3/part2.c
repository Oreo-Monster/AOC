#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include "regex_helper.h"


int find_num(regoff_t pos, char* line){
    if(!isdigit(*(line+pos))) return 0;
    int so;
    int eo;
    for(so = pos-1; isdigit(*(line+so)); so--);
    for(eo = pos+1; isdigit(*(line+eo)); eo++);
    //Loop will end with so and eo off the number by 1
    so++;
    char num_buffer[(eo-so+1)];
    strncpy(&num_buffer, line + so, eo-so );
    num_buffer[eo-so] = '\0';
    return atoi(num_buffer);
}

void check_line(regoff_t pos, char* line, int* num_parts, int* gear_ratio){
    int num;
    if(!isdigit(*(line+pos))) { //Check both sides for numbers
        num = find_num(pos-1, line);
        if(num){
            *num_parts += 1;
            *gear_ratio *= num;
        }
        num = find_num(pos+1, line);
        if(num){
            *num_parts += 1;
            *gear_ratio *= num;
        }
    } else { //Only one num possible on this line
        num = find_num(pos, line);
        if(num){
            *num_parts += 1;
            *gear_ratio *= num;
        }
    }
}

int find_gear_ratio(regoff_t pos, char* prev_line, char* current_line, char* next_line) {
    int num_parts_found = 0;
    int gear_ratio = 1;
    if(prev_line != NULL) check_line(pos, prev_line, &num_parts_found, &gear_ratio);
    check_line(pos, current_line, &num_parts_found, &gear_ratio);
    if(next_line != NULL) check_line(pos, next_line, &num_parts_found, &gear_ratio);
    if(num_parts_found == 2) {
        printf("Gear found at position: %lld\n", pos);
        return gear_ratio;
    }
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
            "[*]",
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
            total += find_gear_ratio(match[0].rm_so + offset, prev_line, current_line, next_line);
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

