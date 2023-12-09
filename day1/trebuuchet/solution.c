#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

    size_t first;
    size_t last;
    char tens_digit;
    char ones_digit;
    int total;
    char* num;
    num = (char*) malloc(3*sizeof(char));
    num[2] = (char) '\0';
    while ((read = getline(&line, &len, fptr)) != -1) {
        if(read < 2) {
            continue;
        }    
        
        last = read - 2;
        first = 0;
        num[0] = 0;
        num[1] = 0;
        while(last >= first && (num[0] == 0 || num[1] == 0)) {
            if(num[0] == 0){
                if ( isdigit(*(line+first))) {
                    num[0] = *(line+first);        
                } else {
                    first++;
                }
            }
            if(num[1] == 0){
                if ( isdigit(*(line+last))) {
                    num[1] = *(line+last);
                } else {
                    last--;
                }    
            }
        }
        total += atoi(num);
    }    
    printf("%d", total);

    free(num);
	fclose(fptr);

	return 0;
}
