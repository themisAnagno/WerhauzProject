#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "decl.h"
#define MAXWORDLENGTH 15	//I set a max value of 15, as it is the max length of any input, which are the source and dest numbers
#define MAXWORDPERLINE 12	// We assume that the max word count per line is 11 (in the case of insert) + one word to indicate the end of line


// Global Var
struct cdrtype new_cdr;
char *file_words[MAXWORDPERLINE];		// A table of words for each line

void file_menu(void)
{
	int wc = 0, i = 0, j, c;
	int end_file = FALSE;

	printf("*** loading operations file ***\n");
	while(!end_file){
		if ((file_words[wc] = (char *)malloc(sizeof(char)*MAXWORDLENGTH)) == NULL) 						// Allocate memory for a 15-char (max) word
			continue;
		while((c = getc(fp)) != ' ' && c != ';' && c != '\n' && c != EOF){		// Every word is seperated with an empty character, a ';' or a new line character
			if (i < MAXWORDLENGTH-1)
				file_words[wc][i++] = c;
		}
		file_words[wc][i] = '\0';
		if (c == ';' || c == ' '){ 		// A word has ended and another one follows
			i = 0;
			if (wc < MAXWORDPERLINE-1)
				wc++;
		}
		else if (c == '\n'){			// The line has ended and has to determine the operation
			operation(wc);
			wc = 0;
			i = 0;
			for (j = 0; j <= wc; j++)
				free(file_words[j]);
		}
		else{							// The line has ended with the EOF. We must run the operation and close the file
			operation(wc);
			for (j = 0; j <= wc; j++)
				free(file_words[j]);
			end_file = TRUE;
		}
	}
	fclose(fp);
	printf("*** end of file ***\n");
}

void operation(int wc)
{
	char ar_date[2][9], ar_time[2][6];
	int k;

	if (strcomp(file_words[0],"insert", 0) == 0 && wc == 9){				// Insert operation
		// Store cdr data
		strcp(new_cdr.cdrid, file_words[1], 0);
		strcp(new_cdr.source_num, file_words[2], 0);
		strcp(new_cdr.dest_num, file_words[3], 0);
		strcp(new_cdr.date, file_words[4], 0);
		strcp(new_cdr.init_time, file_words[5], 0);
		new_cdr.dur = atoi(file_words[6]);
		new_cdr.type = file_words[7][0];
		new_cdr.tarrif = file_words[8][0];
		new_cdr.faultCondition = atoi(file_words[9]);
		if (input(new_cdr) != -1){
			printf("CDR inserted: ");
			printcdr(&new_cdr);
		}
		else
			printf("Memory error - couldn't allocate memory\n");
	}
	else if (strcomp(file_words[0],"delete",0) == 0 && wc == 2){
		delcdr(file_words[2], file_words[1]);
	}
	else if (strcomp(file_words[0],"find",0) == 0 && (wc == 5 || wc == 3 || wc == 1)){
		if (wc == 1){
			strcp(ar_time[0],"",0);
			strcp(ar_time[1],"",0);
			strcp(ar_date[0],"",0);
			strcp(ar_date[1],"",0);
		}
		else if (wc == 3 && (strlen(file_words[2]) == 5)){
			strcp(ar_time[0],file_words[2],0);
			strcp(ar_time[1],file_words[3],0);
			strcp(ar_date[0],"",0);
			strcp(ar_date[1],"",0);
		}
		else if (wc == 3 && (strlen(file_words[2]) == 8)){
			strcp(ar_date[0],file_words[2],0);
			strcp(ar_date[1],file_words[3],0);
			strcp(ar_time[0],"",0);
			strcp(ar_time[1],"",0);
		}
		else{
			strcp(ar_date[0],file_words[3],0);
			strcp(ar_date[1],file_words[5],0);
			strcp(ar_time[0],file_words[2],0);
			strcp(ar_time[1],file_words[4],0);
		}
		find(file_words[1], 1, ar_time, ar_date);
	}
	else if (strcomp(file_words[0],"lookup",0) == 0 && (wc == 5 || wc == 3 || wc == 1)){
		if (wc == 1){
			strcp(ar_time[0],"",0);
			strcp(ar_time[1],"",0);
			strcp(ar_date[0],"",0);
			strcp(ar_date[1],"",0);
		}
		else if (wc == 3 && (strlen(file_words[2]) == 5)){
			strcp(ar_time[0],file_words[2],0);
			strcp(ar_time[1],file_words[3],0);
			strcp(ar_date[0],"",0);
			strcp(ar_date[1],"",0);
		}
		else if (wc == 3 && (strlen(file_words[2]) == 8)){
			strcp(ar_date[0],file_words[2],0);
			strcp(ar_date[1],file_words[3],0);
			strcp(ar_time[0],"",0);
			strcp(ar_time[1],"",0);
		}
		else{
			strcp(ar_date[0],file_words[3],0);
			strcp(ar_date[1],file_words[5],0);
			strcp(ar_time[0],file_words[2],0);
			strcp(ar_time[1],file_words[4],0);
		}
		find(file_words[1], 2, ar_time, ar_date);
	}
	else if (strcomp(file_words[0],"indist",0) == 0 && wc == 2){
		indist(file_words[1], file_words[2]);
	}
	else if (strcomp(file_words[0],"topdest",0) == 0 && wc == 1){
		topdest(file_words[1]);
	}
	else if (strcomp(file_words[0],"top",0) == 0 && wc == 1){
		k = atoi(file_words[1]);
		if (k>=0 && k <= 100)
			topk(k);
		else
			printf("Error, wrong k%%\n");
	}
	else if (strcomp(file_words[0],"bye",0) == 0){
		free_all();
		}
	else if (strcomp(file_words[0],"print",0) == 0 && wc == 1){
		if(strcomp(file_words[1], "hashtable1",0) == 0)
			printhashtable('1');
		else if (strcomp(file_words[1], "hashtable2", 0) == 0)
			printhashtable('2');
		else
			printf("Error: Wrong print argument\n");
	}
	else if (strcomp(file_words[0],"dump", 0) == 0 && wc == 1){
		printf("pr_dump\n");
	}
	else if (strcomp(file_words[0],"", 0) == 0){
		;
	}
	else
		printf("error, next line\n");
}