#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "decl.h"

int main(int argc, char const *argv[])
{
	// Function Declerations

	// Var Declerations
	int i, input_error = FALSE, int_error, file = TRUE, config_file = FALSE;
	char *options[] = {			// A table of strings for every possible flag during the program call
					"-o",
					"-h1",
					"-h2",
					"-s",
					"-c"
		};
	int flags[] = {0, 0, 0, 0}, c, j;		// A table for all the fmandatory flags during the program call. If any flag is not given, the value will remain 0 and the program will not run.
	FILE *config_p;

	for (i = 1; i < argc; i++){
		if (strcomp(argv[i], options[0], 0) == 0){
			i++;
			if ((fp = fopen(argv[i], "r")) == NULL){
				printf("Error: Can't open the Operations file %s\n", argv[i]);
				file = FALSE;
			}
			flags[0] = 1;
		}
		else if (strcomp(argv[i], options[1], 0) == 0){
			int_error = FALSE;
			i++;
			for (c = *(argv[i]), j = 0; c != '\0'; c = argv[i][++j]){
				if (!isdigit(c)){
					int_error = TRUE;
					break;
				}
			}
			if (!int_error)
				hashtableSize1 = atoi(argv[i]);
			else{
				input_error = TRUE;
				printf("Error: Hashtable1NumOfEntries must be an integer\n");
			}
			flags[1] = 1;
		}
		else if (strcomp(argv[i], options[2], 0) == 0){
			int_error = FALSE;
			i++;
			for (c = *(argv[i]), j = 0; c != '\0'; c = argv[i][++j]){
				if (!isdigit(c)){
					int_error = TRUE;
				}
			}
			if (!int_error)
				hashtableSize2 = atoi(argv[i]);
			else{
				input_error = TRUE;
				printf("Error: Hashtable2NumOfEntries must be an integer\n");
			}
			flags[2] = 1;
		}
		else if (strcomp(argv[i], options[3], 0) == 0){
			int_error = FALSE;
			i++;
			for (c = *(argv[i]), j = 0; c != '\0'; c = argv[i][++j]){
				if (!isdigit(c)){
					int_error = TRUE;
				}
			}
			if (!int_error){
				bucketSizeBytes = atoi(argv[i]);
				BUCKETSIZE = bucketSizeBytes / sizeof(struct caller_type);
			}
			else{
				input_error = TRUE;
				printf("Error: BucketSize must be an integer\n");
			}
			flags[3] = 1;
		}
		else if (strcomp(argv[i], options[4], 0) == 0){
			i++;
			if ((config_p = fopen(argv[i], "r")) == NULL){
				printf("Error: Can't open the Configurations file %s\n", argv[i]);
			}
			else{
				config_file = TRUE;
			}

		}
		else{
			input_error = TRUE;
			break;
		}
	}
	for (i = 0; i < 4; i++){
		if (flags[i] == 0){
			input_error = TRUE;
			printf("Error: Missing the %s option\n", options[i]);
		}
	}
	if (!input_error && init() != -1){
		printf("---- Program Parameters ----\n");
		printf("Hashtable 1 size: %d entries\n", hashtableSize1);
		printf("Hashtable 2 size: %d entries\n", hashtableSize2);
		printf("Caller Bucket size: %d entries\n", BUCKETSIZE);
		printf("----------------------------\n");
		if (config_file)
			config_file_handler(config_p);
		if (file)
			file_menu();
		menu();
	}
	else if (input_error)
		printf("Proper program call is:\n./werhauz -o Operations -h1 Hashtable1NumOfEntries -h2 Hashtable2NumOfEntries -s BucketSize -c config-file\n");
	else
		printf("Memory error: Couldn't allocate memory\n");
	return 0;
}