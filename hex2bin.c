#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BYTES		1
#define HEX_CHARS	BYTES*2

unsigned char inBuffer[2];
unsigned char outBuffer[8];
unsigned char prev = 0;
const unsigned char BIN_PREFIX[2] = {'0','B'};
const unsigned char BIN[16][5] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
const char DEC[] = "0123456789abcdef";

typedef enum {IDLE, ZERO, CONVERT_1, CONVERT_2, } State_t; 

int main(int argc, char *argv[]) {

	FILE *fpr, *fpw;
	State_t state;
	char c;
	int pos;
	char binTmp[9];
	binTmp[0] = '\0';
	int savedZeros = 0;

	if(argc != 2){
		printf("Usage: %s <filename>\n", argv[0]);
	}
	else {
		fpr = fopen (argv[1], "r");
		fpw = fopen("bin.c", "w");

		if (fpr!=NULL) {
		  	state = IDLE;
			do {
      			c = fgetc(fpr);

      			switch(state){
	      			case IDLE:
	      				if(c == EOF)
	      					break;
	      				if(c == '0'){
							prev = c;
	      					savedZeros++;
	      				}
	     				else if (c == 'x' && prev == '0') {
	     					savedZeros = 0;
	     					state = CONVERT_1;
	     					fwrite((void *)BIN_PREFIX, sizeof(char), sizeof(BIN_PREFIX), fpw);
	     				}
		   	  			else {
		   	  				while(savedZeros--)
		   	  					fputc('0', fpw);
		   	  				savedZeros = 0;
		   	  				fputc(c, fpw);
		   	  				prev = c;
		   	  			}
		   	  			break;
	      			case CONVERT_1:
	      				state = CONVERT_2;
	      				pos = strchr(DEC, tolower(c)) - DEC;
	      				strcpy(binTmp, BIN[pos]);
	      				break;
	      			case CONVERT_2:
	      				state = IDLE;
	      				pos = strchr(DEC, tolower(c)) - DEC;
						strcat(binTmp, BIN[pos]);
						fprintf(fpw, "%s ", binTmp);
	      				break;
				}
    		} while (c != EOF);

    		fclose(fpr);
			fclose (fpw);
		}
		else printf("Couldn't open file.\n");
	}
	
	return 0;

}