#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include "regex_helper.h"

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

int is_possible(int number, char *color) {
    if(strcmp(color, "red") == 0) return number <= MAX_RED; 
    if(strcmp(color, "green") == 0) return number <= MAX_GREEN; 
    if(strcmp(color, "blue") == 0) return number <= MAX_BLUE; 
    
    return false;
}


int main(int argc, char* argv[]) {
	if(argc != 2){
		printf("Include a file to parse in cmd line\n");
		return 1;
	}

    //Compile regex
    regex_t game_index_pattern;
    regex_t cube_pattern;
    regmatch_t match[4]; 
    int regex_flag;

    regex_flag = regcomp(&game_index_pattern, 
            "Game ([[:digit:]]+): ",
            REG_EXTENDED);
    if(regex_flag) {
        printf("Regex compolation error for game index pattern");
        return 1;
    }
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
    char* game_idx = (char*) malloc(5*sizeof(char));
    char* num_blocks = (char*) malloc(5*sizeof(char));
    char* color = (char*) malloc(6*sizeof(char)); //green is longest color
    int valid_game_flag = 1;
    regoff_t offset = 0;

    char* temp_str = (char*) malloc(100*sizeof(char));

    while ((read = getline(&line, &len, fptr)) != -1) {
        if(read < 2) {
            continue; //empty line
        }    
        //Parse game number
        regex_flag = regexec(&game_index_pattern, line, 4, match, 0);
        if(regex_flag) {
            printf("No matches found\n");
            continue;
        }
        get_substr(line, match[1], game_idx);
        //Reset inner loop vars
        offset = 0;  
        valid_game_flag = 1;
        while(valid_game_flag) {
            regex_flag = regexec(&cube_pattern, line+offset, 4, match, 0);
            if(regex_flag) {
                //If we got here with a valid game flag, game is valid
                total += atoi(game_idx);
                break;
            }
            get_substr(line+offset, match[1], num_blocks);
            get_substr(line+offset, match[2], color);
            valid_game_flag *= is_possible(atoi(num_blocks), color); 
            offset += match[0].rm_eo + 1;
        }
    }    
    printf("Final total: %d\n", total);
    free(num_blocks);
    free(color);
    free(game_idx);
	fclose(fptr);
    regfree(&game_index_pattern);
    regfree(&cube_pattern);
    
	return 0;
}

