#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

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

    regex_t reegex;
    regmatch_t matches[20]; 
    int regex_flag;

    regex_flag = regcomp( &reegex, "[:number:]", 0);
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

        regex_flag = regexec( &reegex, line, 20, matches, 0 );
        if(regex_flag != 0) {
            printf("no matches found\n");
            continue;
        }
        
        strncpy(first_match, line + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so); 
        
        printf("First match: %s\n", first_match);

      //  total += atoi(num);
    }    
    //printf("%d", total);

    free(first_match);
    free(last_match);
    free(num);
	fclose(fptr);

	return 0;
}
