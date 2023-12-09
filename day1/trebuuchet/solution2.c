#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


void get_substr(char* string, regmatch_t match, char* output) {
    strncpy(output, string + match.rm_so, match.rm_eo - match.rm_so); 
    output[match.rm_eo-match.rm_so] = '\0';
}

void flip_string(char* str) {
    char *end = str + strlen(str) - 1;
    while(str < end) {
        //XOR swap
        *str ^= *end;
        *end ^= *str;
        *str ^= *end;

        //Increment
        str++;
        end--;
    }
}

//I miss hashmaps :(
char* decode(char* str) {
    if (strcmp(str, "one") == 0) return "1";
    if (strcmp(str, "two") == 0) return "2";
    if (strcmp(str, "three") == 0) return "3";
    if (strcmp(str, "four") == 0) return "4";
    if (strcmp(str, "five") == 0) return "5";
    if (strcmp(str, "six") == 0) return "6";
    if (strcmp(str, "seven") == 0) return "7";
    if (strcmp(str, "eight") == 0) return "8";
    if (strcmp(str, "nine") == 0) return "9";
    //If we pass in a digit, we want to return it too
    return str;
}

int main(int argc, char* argv[]) {
	if(argc != 2){
		printf("Include a file to parse in cmd line\n");
		return 1;
	}

	char *line = NULL;
    size_t len = 0;
	FILE *fptr;
    ssize_t read;

	if((fptr = fopen(argv[1], "r"))== NULL) {
		printf("Error, file %s can not be read\n", argv[1]);
        exit(EXIT_FAILURE);
	}

    regex_t first_pattern;
    regex_t flipped_pattern;
    regmatch_t matches[2]; 
    int regex_flag;

    regex_flag = regcomp( &first_pattern, "one|two|three|four|five|six|seven|eight|nine|[[:digit:]]", REG_EXTENDED);
    if (regex_flag != 0) {
        printf("Regex compilation failiure");
        exit(EXIT_FAILURE);
    }

    regex_flag = regcomp( &flipped_pattern, "eno|owt|eerht|ruof|evif|xis|neves|thgie|enin|[[:digit:]]", REG_EXTENDED);
    if (regex_flag != 0) {
        printf("Regex compilation failiure");
        exit(EXIT_FAILURE);
    }

    int total;
    char* num = (char*) malloc(3*sizeof(char));
    char* first_match = (char*) malloc(6*sizeof(char));
    char* last_match = (char*) malloc(6*sizeof(char));
    
    num[2] = (char) '\0';
    while ((read = getline(&line, &len, fptr)) != -1) {
        if(read < 2) {
            continue;
        }    
        printf("-----------------------\n");
        printf("Input: %s", line);

        //Find first match
        regex_flag = regexec( &first_pattern, line, 2, matches, 0 );
        if(regex_flag != 0) {
            printf("no matches found\n");
            continue;
        }
        get_substr(line, matches[0], first_match);
        
        //Flip string to get last match
        //Get to the last match
        flip_string(line);
        regex_flag = regexec( &flipped_pattern, line, 2, matches, 0 );
        if(regex_flag != 0) {
            printf("no matches found\n");
            continue;
        }
        get_substr(line, matches[0], last_match);
        flip_string(last_match);

        
        num[0] = *decode(first_match);
        num[1] = *decode(last_match);
        printf("%s\n", num);
        total += atoi(num);
    }    
    printf("%d", total);
    regfree(&first_pattern);
    regfree(&flipped_pattern);
    free(first_match);
    free(last_match);
    free(num);
	fclose(fptr);

	return 0;
}

