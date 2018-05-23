#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "decl.h"

float value_table[][3] =	{	// Two dimensional table for type/tarrif/value combinations. Each row is a different combination of type/tarrif. The 1st column is type, 2nd column is tarrif and 3rd is value/min
				{0,0,0.5},	// Initial values, in case that no config file is inserted
				{0,1,1},
				{0,2,1.5},
				{1,0,0.7},
				{1,1,1.4},
				{1,2,2.1},
				{2,0,1},
				{2,1,2},
				{2,2,3}
			};
int No_of_comb = 0;


// The config_file_handler function
// Reads the config.txt file and stores the values for the cost in the value_table var
void config_file_handler(FILE *config_p)
{
	int c, exit = FALSE, i = 0, j = 0, k;
	float N = 0;
	char number[10];

	printf("*** loading config file ***\n");
	while (!exit){
		N = 0;
		k = 0;
		while ((c = getc(config_p)) != '\n' && c != EOF && c != ';' && k < 9){
			number[k++] = c;
		}
		number[k] = '\0';
		N = atof(number);
		if (c == ';' && j < 2){
			value_table[i][j] = N;
			j++;
		}
		else if (c == '\n' && i < 10){
			value_table[i][j] = N;
			i++;
			j = 0;
		}
		else{
			if (j == 2 && i < 10){
				value_table[i][j] = N;
				i++;
			}
			exit = TRUE;
		}
	}
	No_of_comb = i;
	fclose(config_p);
}

float get_value(int type, int tarrif, int dur)
{
	int i;
	/*for (i = 0; i < No_of_comb; i++){
		printf("\n");
		for (j = 0; j < 3; j++){
			printf("%.2f\t", value_table[i][j]);
		}
	}*/

	for (i = 0; i < No_of_comb; i++){
		if (type == value_table[i][0] && tarrif == value_table[i][1]){
			return dur * value_table[i][2];
		}
	}
	return dur * 1;	// Default value/min is 1 if no correct type/tarrif combination is inserted
}