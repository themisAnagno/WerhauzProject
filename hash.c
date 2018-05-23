#include <stdio.h>
#include <stdlib.h>
#include "decl.h"

// The init function
//Initializes the 2 hashtables and the max binary heap tree
int init(void)
{
	int i;

	// Initializes the two hashtables
	if ((hashtable1 = (struct caller_bucket_list **)malloc(sizeof(struct caller_bucket_list *)*hashtableSize1)) == NULL)	// Dynamically allocates memory for the hash table 1
		return -1;
	for (i = 0; i < hashtableSize1; i++)
		hashtable1[i] = NULL;
	if ((hashtable2 = (struct caller_bucket_list **)malloc(sizeof(struct caller_bucket_list *)*hashtableSize2)) == NULL)	// Dynamically allocates memory for the hash table 1
		return -1;
	for (i = 0; i < hashtableSize2; i++)
		hashtable2[i] = NULL;
	printf("Hashtables initialized Succesfully\n");

	// Initializes the root node for the topk tree
	if ((topk_root = (struct topk_node_type *)malloc(sizeof(struct topk_node_type))) == NULL)
		return -1;
	topk_root->par = NULL;
	topk_root->left = NULL;
	topk_root->right = NULL;
	Nnodes = 0;
	tot_value = 0;
	return 0;
}


// The input function
// Inserts a new cdr in the list
int input(struct cdrtype new)
{

	// Var Declarations
	int pos, i = 0, j, found = FALSE;
	struct cdrtype *enter_cdr;			// Variables for every new struct or table will be created
	struct caller_bucket_list *new_caller_bucket, *search_bucket;
	struct caller_type *insert_bucket;

	// Input in Hashtable 1
	insert_bucket = lookup(new.source_num, 1);		// Looks up to find if the caller is already in
	if (insert_bucket != NULL){					// The caller was found
		if ((enter_cdr = (struct cdrtype *) malloc(sizeof(struct cdrtype))) == NULL)		// New cdr node is created
			return -1;
		*enter_cdr = new;						// Takes the values from the new cdr
		enter_cdr->nextcdr = insert_bucket->data;	// Points to the previous first node in the cdr node
		insert_bucket->data = enter_cdr;			// The enter_cdr becomes the forst node
	}
	else{										// The caller was not found
		pos = hash(new.source_num, 1);				// The position in the Hash Table 1
		if (hashtable1[pos] == NULL){			// The Hash Table is empty at this specific position and everything has to be created
			if ((new_caller_bucket = (struct caller_bucket_list *)malloc(sizeof(struct caller_bucket_list))) == NULL)
				return -1;
			if ((new_caller_bucket->caller_bucket = (struct caller_type *)malloc(sizeof(struct caller_type)*BUCKETSIZE)) == NULL)   // The bucket table is dynamically created 
				return -1;
			new_caller_bucket->next = NULL;
			for (j = 0; j < BUCKETSIZE; j++)
				new_caller_bucket->caller_bucket[j].data = NULL;
			new_caller_bucket->next = hashtable1[pos];
			hashtable1[pos] = new_caller_bucket;
			if ((enter_cdr =(struct cdrtype *)malloc(sizeof(struct cdrtype))) == NULL)
				return -1;
			*enter_cdr = new;
			new_caller_bucket->caller_bucket[0].data = enter_cdr;
			strcp(new_caller_bucket->caller_bucket[0].number, new.source_num, 0);
		}
		else{									// There is already an entry in that pos
			search_bucket = hashtable1[pos];
			i = 0;  // counter for the caller bucket
			found = FALSE;
			while (!found){
				if (search_bucket->caller_bucket[i].data == NULL){  // The next available pos in the hash table
					if ((enter_cdr = (struct cdrtype *) malloc(sizeof(struct cdrtype))) == NULL)
						return -1;
					*enter_cdr = new;
					enter_cdr->nextcdr = search_bucket->caller_bucket[i].data;
					search_bucket->caller_bucket[i].data = enter_cdr;
					strcp(search_bucket->caller_bucket[i].number, new.source_num, 0);
					found = TRUE;
				}
				else{
					//printf("occupied i= %d  ", i);
					if (i < BUCKETSIZE-1){
						i++;		// Moves to the next cell in the bucket table
					}
					else{			// The current bucket is full and needs to go to a new bucket
						if (search_bucket->next == NULL){	// Must create a new bucket
							i = 0;
							if ((new_caller_bucket = (struct caller_bucket_list *)malloc(sizeof(struct caller_bucket_list))) == NULL)
								return -1;
							if ((new_caller_bucket->caller_bucket = (struct caller_type *)malloc(sizeof(struct caller_type)*BUCKETSIZE)) == NULL)   // The bucket table is dynamically created
								return -1;
							new_caller_bucket->next = NULL;
							for (j = 0; j < BUCKETSIZE; j++)
								new_caller_bucket->caller_bucket[j].data = NULL;
							if ((enter_cdr = (struct cdrtype *) malloc(sizeof(struct cdrtype))) == NULL)
								return -1;
							*enter_cdr = new;
							search_bucket->next = new_caller_bucket;
							new_caller_bucket->caller_bucket[0].data = enter_cdr;
							strcp(new_caller_bucket->caller_bucket[0].number, new.source_num, 0);
							found = TRUE;
						}
						else{
							search_bucket = search_bucket->next;	// Moves to the next bucket
							i = 0;
						}
					}
				}
			}
		}
	}

	// Input in Hashtable 2
	insert_bucket = lookup(new.dest_num, 2);		// Looks up to find if the caller is already in
	if (insert_bucket != NULL){					// The callee was found
		if ((enter_cdr = (struct cdrtype *) malloc(sizeof(struct cdrtype))) == NULL)	// New cdr node is created
			return -1;
		*enter_cdr = new;						// Takes the values from the new cdr
		enter_cdr->nextcdr = insert_bucket->data;	// Points to the previous first node in the cdr node
		insert_bucket->data = enter_cdr;			// The enter_cdr becomes the forst node
	}
	else{										// The callee was not found
		pos = hash(new.dest_num, 2);				// The position in the Hash Table 2
		if (hashtable2[pos] == NULL){			// The Hash Table is empty at this specific position and everything has to be created
			if ((new_caller_bucket = (struct caller_bucket_list *)malloc(sizeof(struct caller_bucket_list))) == NULL)
				return -1;
			new_caller_bucket->next = NULL;
			if ((new_caller_bucket->caller_bucket = (struct caller_type *)malloc(sizeof(struct caller_type)*BUCKETSIZE)) == NULL)   // The bucket table is dynamically created 
				return -1;
			for (j = 0; j < BUCKETSIZE; j++)
				new_caller_bucket->caller_bucket[j].data = NULL;
			new_caller_bucket->next = hashtable2[pos];
			hashtable2[pos] = new_caller_bucket;
			if ((enter_cdr =(struct cdrtype *)malloc(sizeof(struct cdrtype))) == NULL)
				return -1;
			*enter_cdr = new;
			new_caller_bucket->caller_bucket[0].data = enter_cdr;
			strcp(new_caller_bucket->caller_bucket[0].number, new.dest_num, 0);
		}
		else{									// There is already an entry in that pos
			search_bucket = hashtable2[pos];
			i = 0;  // counter for the callee bucket
			found = FALSE;
			while (!found){
				if (search_bucket->caller_bucket[i].data == NULL){  // The next available pos in the hash table
					if ((enter_cdr = (struct cdrtype *) malloc(sizeof(struct cdrtype))) == NULL)
						return -1;
					*enter_cdr = new;
					enter_cdr->nextcdr = search_bucket->caller_bucket[i].data;
					search_bucket->caller_bucket[i].data = enter_cdr;
					strcp(search_bucket->caller_bucket[i].number, new.dest_num, 0);
					found = TRUE;
				}
				else{
					if (i < BUCKETSIZE-1){
						i++;		// Moves to the next cell in the bucket table
					}
					else{			// The current bucket is full and needs to go to a new bucket
						if (search_bucket->next == NULL){	// Must create a new bucket
							if ((new_caller_bucket = (struct caller_bucket_list *)malloc(sizeof(struct caller_bucket_list))) == NULL)
								return -1;
							if ((new_caller_bucket->caller_bucket = (struct caller_type *)malloc(sizeof(struct caller_type)*BUCKETSIZE)) == NULL)   // The bucket table is dynamically created 
								return -1;
							new_caller_bucket->next = NULL;
							for (j = 0; j < BUCKETSIZE; j++)
								new_caller_bucket->caller_bucket[j].data = NULL;
							if ((enter_cdr = (struct cdrtype *) malloc(sizeof(struct cdrtype))) == NULL)
								return -1;
							*enter_cdr = new;
							search_bucket->next = new_caller_bucket;
							new_caller_bucket->caller_bucket[0].data = enter_cdr;
							strcp(new_caller_bucket->caller_bucket[0].number, new.dest_num, 0);
							found = TRUE;
						}
						else{
							search_bucket = search_bucket->next;	// Moves to the next bucket
							i = 0;
						}
					}
				}
			}
		}
	}

	// Input in the topk tree
	return insert_node(new);
}


// The Hash function
// Uses the modulo as hush function to return an index to Hashtable
int hash(char *s, int n)
{
	int size;
	int h = 0, a = 31;		// Initial value based on Kernighan and Ritchie's hash function

	if (n == 1)
		size = hashtableSize1;
	else
		size = hashtableSize2;
	while (*s != '\0'){
		h = ((h*a) + *s++)%size;
	}
	return h;		// Returns the index, position in the Hash Table.
}


// The lookup function
struct caller_type* lookup(char *find, int n)
{
	int pos, i = 0, found = FALSE;
	struct caller_bucket_list *search_bucket;
	struct caller_bucket_list **root;

	if (n == 1){
		root = hashtable1;
	}
	else{
		root = hashtable2;
	}
	pos = hash(find, n);
	search_bucket = root[pos];
	while(!found && search_bucket != NULL){
		if (strcomp(search_bucket->caller_bucket[i].number, find, 0) == 0){
			found = TRUE;
			return &(search_bucket->caller_bucket[i]);
		}
		else if (i < BUCKETSIZE-1)
			i++;
		else{
			search_bucket = search_bucket->next;
			i = 0;
		}
	}
		return NULL;
}


// The delcdr function
// This function uses the lookup function to find the head node of the the chain list of cdr for the given number (if any). It uses the method to delete the next node in the list, if it matches the
// given cdr id. That's why we use a different case for the first node of the list
void delcdr(char *del_name, char *del_id)
{
	struct caller_type *del_caller;
	struct cdrtype *search_cdr, *temp_cdr;
	int found = FALSE;

	if ((del_caller = lookup(del_name, 1)) == NULL)
		printf("Error: The number was not found\n");	
	else{
		search_cdr = del_caller->data;
		if (search_cdr != NULL){
			if(strcomp(search_cdr->cdrid,del_id,0) == 0){	// The case where the first node of the list is the node to be deleted
				if ((del_caller->data = search_cdr->nextcdr) == NULL)
					strcp(del_caller->number,"" , 0);
				printf("Succesfully deleted cdr:  ");
				printcdr(search_cdr);
				free(search_cdr);
				found = TRUE;
			}
			else{
				while(search_cdr->nextcdr != NULL && !found){
					if(strcomp(search_cdr->nextcdr->cdrid,del_id,0) == 0)				// Checks the next cdr node in the list if it matches the given one
						found = TRUE;
					else
						search_cdr = search_cdr->nextcdr;
				}
				if (found){
					temp_cdr = search_cdr->nextcdr;
					printf("Succesfully deleted the following cdr:\n");
					printcdr(temp_cdr);
					search_cdr->nextcdr = temp_cdr->nextcdr;
					free(temp_cdr);
				}
			}
		}
		if (!found){
			printf("Error: The cdr id was not found\n");
		}
	}

}

// The printhashtable functions
// It prints the entire hashtable with every caller bucket that has been created
void printhashtable(int n)
{
	struct caller_bucket_list **root;
	struct caller_bucket_list *search_bucket;
	struct cdrtype *p_cdr;
	int pos, i, size, b;
	char message[7];

	switch (n){
		case '1':
			root = hashtable1;
			size = hashtableSize1;
			strcp(message, "Caller", 0);
			break;
		case '2':
			root = hashtable2;
			size = hashtableSize2;
			strcp(message, "Callee", 0);
			break;
		default:
			printf("Error! Couldn't print\n");
			break;
	}
	printf("*************\n");
	printf("Hash Table %c\n", n);
	printf("*************\n");
	for (pos = 0; pos < size; pos++){
		if (root[pos] != NULL){
			printf("\n>>> pos %d <<<\n", pos);
			printf("--------------");
			b = 1;
			search_bucket = root[pos];
			while (search_bucket != NULL){
				printf("\n## %s Bucket %d ##\n", message, b);
				for (i = 0; i < BUCKETSIZE; i++){
					if ((p_cdr = search_bucket->caller_bucket[i].data) !=  NULL){
						printf("-- i=%d : %s = %s\n", i, message, search_bucket->caller_bucket[i].number);
						while (p_cdr != NULL){
							printcdr(p_cdr);
							p_cdr = p_cdr->nextcdr;
						}
					}
				}
				search_bucket = search_bucket->next;
				b++;
			}

		}
	}
	printf("\n");
}

void printcdr(struct cdrtype *cdr)
{
	if (cdr != NULL)
		printf("%s;%s;%s;%s;%s;%d;%c;%c;%d\n", cdr->cdrid, cdr->source_num, cdr->dest_num, cdr->date, cdr->init_time, cdr->dur, cdr->type, cdr->tarrif, cdr->faultCondition);
}


// The free_all functions
// It sets free any memory part that has been allocated during the insert
void free_all(void)
{
	int pos, i;
	struct caller_bucket_list *search_bucket, *temp_bucket;
	struct cdrtype *temp_cdr;

	for (pos = 0; pos < hashtableSize1; pos++){		// Free hashtable 1
		if (hashtable1[pos] != NULL){
			search_bucket = hashtable1[pos];
			while (search_bucket != NULL){			
				for (i = 0; i < BUCKETSIZE; i++){	// Free every cdr in the caller bucket and then frees the caller bucket
					while (search_bucket->caller_bucket[i].data != NULL){
						temp_cdr = search_bucket->caller_bucket[i].data;
						search_bucket->caller_bucket[i].data = temp_cdr->nextcdr;
						if (temp_cdr){
							free(temp_cdr);
							temp_cdr = NULL;
						}
					}
				}
				search_bucket = search_bucket->next;
			}
		}
		while (hashtable1[pos] != NULL){			// Free  every caller bucket list node until reaches NULL
			temp_bucket = hashtable1[pos];
			hashtable1[pos] = temp_bucket->next;
			if (temp_bucket){
			for (i = 0; i < BUCKETSIZE; i++)
					free(temp_bucket->caller_bucket[i].data);
				free(temp_bucket);
				temp_bucket = NULL;
			}
		}
	}
	free(*hashtable1);

	for (pos = 0; pos < hashtableSize2; pos++){		// Free hashtable 2
		if (hashtable2[pos] != NULL){
			search_bucket = hashtable2[pos];
			while (search_bucket != NULL){
				for (i = 0; i < BUCKETSIZE; i++){	// Free every cdr in the caller bucket and then frees the caller bucket
					while (search_bucket->caller_bucket[i].data != NULL){
						temp_cdr = search_bucket->caller_bucket[i].data;
						search_bucket->caller_bucket[i].data = temp_cdr->nextcdr;
						if (temp_cdr){
							free(temp_cdr);
							temp_cdr = NULL;
						}
					}
				}
				search_bucket = search_bucket->next;
			}
		}
		while (hashtable2[pos] != NULL){			// Free  every caller bucket list node until reaches NULL
			temp_bucket = hashtable2[pos];
			hashtable2[pos] = temp_bucket->next;
			if (temp_bucket){
				for (i = 0; i < BUCKETSIZE; i++)
					free(temp_bucket->caller_bucket[i].data);
				free(temp_bucket);
				temp_bucket = NULL;
			}
		}
	}
	free(*hashtable2);
	printf("The hash tables were deleted.\n");

	free_topk(topk_root);	// The function that frees the topk tree
	printf("The topk tree was deleted\n");
}


// The find function
// It is called for the "find" and "lookup operations". The 2nd argument (int n) defines if the function will use Hashtable 1 (find operation for the caller) or 2 (lookup operation for the callee) as root.
// The time and date arguments may have empty values, if the user doesn't define them. So we use 2 flags to determine whether or not the date and time have to be checked. 
void find(char *number, int n, char time[][6], char date[][9])
{
	int time_flag = TRUE, date_flag = TRUE, pr_flag = FALSE;
	struct cdrtype *p_cdr = NULL;

	if (strcomp(time[0],"",0) != 0)
		time_flag = FALSE;
	if (strcomp(date[0],"",0) != 0)
		date_flag = FALSE;
	if (lookup(number, n) != NULL)
		p_cdr = (lookup(number, n))->data;
	while(p_cdr != NULL){
		if ((((strcomp(p_cdr->init_time,time[0],0) > 0) && (strcomp(p_cdr->init_time, time[1],0) < 0)) || time_flag) && (((datecomp(p_cdr->date,date[0]) > 0) && (datecomp(p_cdr->date, date[1]) < 0)) || date_flag)){
			printf("Found cdr:  ");
			pr_flag = TRUE;
			printcdr(p_cdr);
		}
		p_cdr = p_cdr->nextcdr;
	}
	if (!pr_flag)
		printf("No cdr found\n");
}

int datecomp(char *d1, char *d2)
{
	int r;
	
	if ((r = strcomp(d1+4,d2+4,0)) != 0){
		return r;
	}
	if ((r = strcomp(d1+2, d2+2,2)) !=0){
		return r;
	}
	return strcomp(d1,d2,2);
}

// The topdest functions
// ---------------------
void topdest (char *number)
{
	struct caller_type *caller;
	struct cdrtype *p_cdr;
	struct dest_type *dest_tree_root = NULL, *insert_node;
	int nodes = 1; 
	char dest[4];

	caller = lookup(number, 1);
	if (caller == NULL){
		printf("The caller was not found\n");
		return;
	}
	p_cdr = caller->data;
	if ((dest_tree_root = (struct dest_type *)malloc(sizeof(struct dest_type))) == NULL){
		printf("Memory allocation error\n");
		return;
	}
	strcp(dest_tree_root->id, caller->data->dest_num,3);
	dest_tree_root->n = 1;
	dest_tree_root->par = NULL;
	dest_tree_root->left = NULL;
	dest_tree_root->right = NULL;
	while((p_cdr=p_cdr->nextcdr) != NULL){
		strcp(dest, p_cdr->dest_num,3);
		if (lookup_dest(dest, dest_tree_root) == 1){
			continue;
		}
		nodes++;
		if ((insert_node = (struct dest_type *)malloc(sizeof(struct dest_type))) == NULL){
			printf("Memory allocation error\n");
			return;
		}
		strcp(insert_node->id, dest,3);
		insert_node->n = 1;
		insert_node->par = NULL;
		insert_node->left = NULL;
		insert_node->right = NULL;
		treepos(insert_node, dest_tree_root, nodes);
	}
	printf("Top dest is: %s (%d times)\n", dest_tree_root->id, dest_tree_root->n);
	print_dest_tree(dest_tree_root);
	free_dest(dest_tree_root);
}


void treepos(struct dest_type *node,struct dest_type *root, int N)
{
	int l = 0, a;

	if (root->left == NULL){
		root->left = node;
		node->par = root;
		heapify(node);
	}
	else if (root->right == NULL){
		root->right = node;
		node->par = root;
		heapify(node);
	}
	else{
		for(l = 0; N >= mypow(2,l+1); l++)
			;
		a = ((mypow(2,l))+mypow(2,l+1))/2;
		if (N < a){
			treepos(node, root->left, N-mypow(2,l-1));
		}
		else{
			treepos(node, root->right, N-mypow(2,l));
		}
	}
}

int lookup_dest(char *dest, struct dest_type *root)
{
	if (strcomp(root->id, dest,0) == 0){
		root->n++;
		heapify(root);
		return 1;
	}
	if (root->left != NULL)
		return lookup_dest(dest, root->left);
	if (root->right != NULL)
		return lookup_dest(dest, root->right);
	return 0;
}

void heapify(struct dest_type *node)
{
	char tempid[4];
	int tempn;

	if (node->par != NULL && node->par->n < node->n){
		strcp(tempid,node->par->id,0);
		tempn = node->par->n;
		strcp(node->par->id,node->id,0);
		node->par->n = node->n;
		strcp(node->id,tempid,0);
		node->n=tempn;
		heapify(node->par);
	}
}

void print_dest_tree(struct dest_type *root)
{
	printf("%s : %d\n", root->id, root->n);
	if (root->left != NULL)
		print_dest_tree(root->left);
	if (root->right != NULL)
		print_dest_tree(root->right);
}

void free_dest(struct dest_type *root)
{
	if (root->left != NULL)
		free_dest(root->left);
	if (root->right != NULL)
		free_dest(root->right);
	free(root);
}




// The indist functions
// --------------------
void indist(char *caller1, char *caller2)
{
	struct caller_type *c1, *c2;

	if ((c1 = lookup(caller1, 1)) == NULL){
		printf("Caller 1 not found\n");
		return;
	}
	if ((c2 = lookup(caller2, 1)) == NULL){
		printf("Caller 2 not found\n");
		return;
	}
	indist_comp(c1->data,c2->data);
}

void indist_comp(struct cdrtype *c1, struct cdrtype *c2)
{
	struct cdrtype *d1 = c1, *d2 = c2;

	printf("Indist 1 %s - %s \n",d1->source_num, d2->source_num);
	printf("---------------------------------------------\n");
	while (d1 != NULL){
		while (d2 != NULL){
			if (strcomp(d1->dest_num, d2->dest_num, 0) == 0)
				printf("%s\n", d1->dest_num);
			d2 = d2->nextcdr;
		}
		d1 = d1->nextcdr;
	}
}
