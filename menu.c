#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "decl.h"
#define MAXFILENAME 30

// Global Var
struct cdrtype new_cdr;

void menu(void)
{
	// Var declerations
	int exit = FALSE, choice, error = FALSE, i, c, j;
	char search_number[15], search_number2[15], del_cdr_id[11], file_name[MAXFILENAME];
	char ans[5], ar_date[2][9], ar_time[2][6];

	while(!exit){
		printf("Press\n");
		printf("1.  >> insert\n2.  >> delete\n3.  >> find\n4.  >> lookup\n");
		printf("5.  >> indist1\n6.  >> topdest\n7.  >> top\n8.  >> bye\n9.  >> print\n10. >> dump\n");
		choice = getchoice();
		switch (choice) {
			case '1':
				getcdr();
				if (input(new_cdr) != -1)
					printf("CDR inserted\n");
				else
					printf("Memory error - couldn't allocate memory\n");
				break;
			case '2':
				// Enter the CDR id
				printf("Enter the number\n");
				getnumber(search_number);
				do{
					error = FALSE;
					i = 0;
					printf("Enter the code of the cdr you want to delete\n");
					while ((c = getchar()) != '\n'){
						if ((isdigit(c) || isalpha(c)) && i < 10)
							del_cdr_id[i++] = c;
						else
							error = TRUE;
					}
					if (!error)
						del_cdr_id[i] = '\0';
					else if (i == 10)
						printf("Error: You entered more than 7 digits\n");
					else
						printf("Error: Use only alphanumeric characters\n");
				}while(error);
				delcdr(search_number,del_cdr_id);
				break;
			case '3':
				printf("Enter the caller's number\n");
				getnumber(search_number);
				printf("Enter time? (Yes)\n");
				j = 0;
				while ((c = getchar()) != '\n'){
					if (isalpha(c) && j < 4)
						ans[j++] = c;
				}
				ans[j] = '\0';
				if (strcomp(ans,"Yes",0)==0){
					printf("time1:\n");
					gethour(ar_time[0]);
					printf("time2:\n");
					gethour(ar_time[1]);
				}
				else {
					strcp(ar_time[0],"", 0);
					strcp(ar_time[1],"", 0);
				}
				printf("Enter date? (Yes)\n");
				j = 0;
				while ((c = getchar()) != '\n'){
					if (isalpha(c) && j < 4)
						ans[j++] = c;
				}
				ans[j] = '\0';
				if (strcomp(ans,"Yes",0)==0){
					printf("date1:\n");
					getdate(ar_date[0]);
					printf("date2:\n");
					getdate(ar_date[1]);
				}
				else {
					strcp(ar_date[0],"", 0);
					strcp(ar_date[1],"" ,0);
				}
				find(search_number, 1, ar_time, ar_date);
				break;
			case '4':
				printf("Enter the callee's number\n");
				getnumber(search_number);
				printf("Enter time? (Yes)\n");
				j = 0;
				while ((c = getchar()) != '\n'){
					if (isalpha(c) && j < 4)
						ans[j++] = c;
				}
				ans[j] = '\0';
				if (strcomp(ans,"Yes",0)==0){
					printf("time1:\n");
					gethour(ar_time[0]);
					printf("time2:\n");
					gethour(ar_time[1]);
				}
				else {
					strcp(ar_time[0],"", 0);
					strcp(ar_time[1],"", 0);
				}
				printf("Enter date? (Yes)\n");
				j = 0;
				while ((c = getchar()) != '\n'){
					if (isalpha(c) && j < 4)
						ans[j++] = c;
				}
				ans[j] = '\0';
				if (strcomp(ans,"Yes",0)==0){
					printf("date1:\n");
					getdate(ar_date[0]);
					printf("date2:\n");
					getdate(ar_date[1]);
				}
				else {
					strcp(ar_date[0],"", 0);
					strcp(ar_date[1],"", 0);
				}
				find(search_number, 2, ar_time, ar_date);
				break;
			case '5':
				printf("Enter caller1\n");
				getnumber(search_number);
				printf("Enter caller2\n");
				getnumber(search_number2);
				indist(search_number, search_number2);
				break;
			case '6':
				printf("Enter the caller's number\n");
				getnumber(search_number);
				topdest(search_number);
				break;
			case '7':
				printf("Best customer: %s (val: %.2f)\n", topk_root->number, topk_root->val);
				do{
					printf("Give the top k%%\n");
					i = getint();
					if (i>100 || i<0)
						printf("Give a number in the range 0-100\n");
				}while(i>100 || i<0);
				topk(i);
				break;
			case '8':
				free_all();
				do{	
					printf("Choose:\n1. Import from a file\n2. Exit\n");
					i = getchoice();
					switch (i){
						case '1':
							if (init() == -1){
								printf("Memory error: couldn't allocate memory\n");
								return;
							}
							printf("Enter the operation file name\n");
							for (c = getchar(), j=0; c != '\n'; c = getchar(), j++){
								if (j < MAXFILENAME-1)
									file_name[j] = c;
							}
							file_name[j] = '\0';
							if ((fp = fopen(file_name, "r")) == NULL){
								printf("Error: Can't open the Operations file %s\n", file_name);
							}
							else
								file_menu();
							break;
						case '2':
							exit = 1;
							break;
						default:
							printf("Wrong choice, please choose again");
							break;
						}
					}while(i != '1' && i != '2');
				break;
			case '9':
				do{
					printf("Choose which table to print:\n1. Hashtable 1 (caller)\n2. Hashtable 2 (callee)\n");
					i = getchoice();
					if (i == '1' || i == '2')
						printhashtable(i);
					else
						printf("Error: Wrong choice, please choose again\n");
				}while(i != '1' && i != '2');
				break;
			default:
			printf("Wrong choice, please choose one of the following:\n");\
			break;
		}
	}
	printf("\nGoodbye!\n");
}

// The getchoice function
int getchoice(void)
{
	int c, s, n = 0;

	while ((c = getchar()) != '\n'){
		s = c;
		n++;
	}
	return n == 1 ? s : -1;
}

// The getcdr function
void getcdr(void)
{
	int c, error, i;

	// Enter the CDR id
	do{
		error = FALSE;
		i = 0;
		printf("Enter the 7digit CDR code\n");
		while ((c = getchar()) != '\n'){
			if ((isdigit(c) || isalpha(c)) && i < 7)
				new_cdr.cdrid[i++] = c;
			else
				error = TRUE;
		}
		if (!error)
			new_cdr.cdrid[i] = '\0';
		else if (i == 7)
			printf("Error: You entered more than 7 digits\n");
		else
			printf("Error: Use only alphanumeric characters\n");
	}while(error);

	// Enter the source number
	printf("Source number\n");
	getnumber(new_cdr.source_num);

	// Enter the dest number
	printf("Dest number\n");
	getnumber(new_cdr.dest_num);
	while (strcomp(new_cdr.source_num,new_cdr.dest_num, 0) == 0){
		printf("Can't have the same source and dest number. Enter again the dest number\n");
		getnumber(new_cdr.dest_num);
	}

	// Enter the date
	printf("Enter the date (format DDMMYYYY)\n");
	getdate(new_cdr.date);

	// Enter initialization time
	printf("Enter the initialization time\n");
	gethour(new_cdr.init_time);

	// Enter the call duration
	printf("Enter the duration in mins\n");
	new_cdr.dur = getint();

	// Enter type
	do {
		printf("Enter the type number\n");
		i = 0;
		error = FALSE;
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i<1){
				i++;
				new_cdr.type = c;
			}
			else
				error = TRUE;
		}
		if (error)
			printf("Error, please enter again\n");
	}while(error);

	// Enter tarrif
	do {
		printf("Enter the tarrif\n");
		i = 0;
		error = FALSE;
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i<1){
				i++;
				new_cdr.tarrif = c;
			}
			else
				error = TRUE;
		}
		if (error)
			printf("Error, please enter again\n");
	}while(error);

	// Enter the fault condition
	printf("Enter the fault condition\n");
	new_cdr.faultCondition = getint();
	new_cdr.nextcdr = NULL;
}

// The get int function
int getint(void)
{
	char s[5];
	int i, c, error;

	while (1){
		error = 0;
		i = 0;
		printf("Please enter a correct integer: ");
		while ((c = getchar()) != '\n'){
			if (!isdigit(c))
				error = 1;
			else
				s[i++] = c;
		}
		if (error == 1)
			printf("Error: Not a correct integer\n");
		else{
			s[i] = '\0';
			return atoi(s);
		}
	}
}

// The getnumber function
void getnumber(char *number)
{
	char input[11], error, c;
	int i;

	do{
		error = FALSE;
		i = 0;
		printf("Enter the 3digit country code\n");
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i < 3)
				input[i] = c;
			else
				error = TRUE;
			i++;
		}
		if (i < 3)
			error = TRUE;
		if (!error)
			input[i] = '-';
		else if (i > 3)
			printf("Error: You entered more than 3 digits\n");
		else if (i < 3)
			printf("Error: You entered less than 3 digits\n");
		else
			printf("Error: Use only digits\n");
	}while(error);
	do{
		error = FALSE;
		i = 4;
		printf("Enter the 10digit number\n");
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i < 14)
				input[i] = c;
			else
				error = TRUE;
			i++;
		}
	//	if (i < 13)
	//		error = TRUE;
		if (!error)
			input[i] = '\0';
		else if (i > 13)
			printf("Error: You entered more than 10 digits\n");
	//	else if (i < 13)
	//		printf("Error: You entered less than 10 digits\n");
		else
			printf("Error: Use only digits\n");
	}while(error);
	strcp(number, input, 0);
}

void gethour(char *time)
{
	int i, c, error;

	// Enter hour
	do{
		error = FALSE;
		i = 0;
		printf("Hour: ");
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i < 2)
				time[i++] = c;
			else
				error = TRUE;
		}
		time[i] = '\0';
		if (i<2 || atoi(time) > 24)
			error = TRUE;
		
		if (!error){
			time[2] = ':';
		}
		else
			printf("Error: The format is HH:MM. Please enter again\n");
	}while(error);

	// Enter minutes
	do{
		error = FALSE;
		i = 3;
		printf("Minutes: ");
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i < 5)
				time[i++] = c;
			else
				error = TRUE;
		}
		time[i] = '\0';
		if (i<5 || atoi(time+3) > 60)
			error = TRUE;
		if (error)
			printf("Error: The format is HH:MM. Please enter again\n");
	}while(error);
}

void getdate(char *date)
{
	int i, c, error;

	// Enter day
	do{
		error = FALSE;
		i = 0;
		printf("Day: ");
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i < 2)
				date[i++] = c;
			else
				error = TRUE;
		}
		date[i] = '\0';
		if (i<2 || atoi(date) > 31)
			error = TRUE;
		
		if (error)
			printf("Error: The format is DDMMYYYY. Please enter again\n");
	}while(error);

	// Enter month
	do{
		error = FALSE;
		i = 2;
		printf("Month: ");
		while ((c = getchar()) != '\n'){
			if (isdigit(c) && i < 4)
				date[i++] = c;
			else
				error = TRUE;
		}
		date[i] = '\0';
		if (i<4 || atoi(date+2) > 12 || atoi(date+2) < 1)
			error = TRUE;
		if (error)
			printf("Error: The format is DDMMYYYY. Please enter again\n");
	}while(error);

	do{
		i = 4;
		printf("Year: ");
		error = FALSE;
		while ((c = getchar()) != '\n'){
			if ((isdigit(c)) && i < 8)
				date[i++] = c;
			else 
				error = TRUE;
		}
		if (!error)
			date[i] = '\0';
		else 
			printf("Error: The format is DDMMYYYY. Please enter again\n");
	}while(error);
}