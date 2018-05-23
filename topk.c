#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "decl.h"


// The insert function. 
// Inserts a new cdr in the topk tree. Returns -1 in case of memory allocation error or 1 if everything works fine
int insert_node(struct cdrtype new)
{
	struct topk_node_type *new_node;
	float value =  get_value((new.type -'0'),(new.tarrif -'0'), new.dur);

	tot_value += value;
	if (Nnodes == 0){
		strcp(topk_root->number,new.source_num,0);
		topk_root->val = value;
		Nnodes = 1;
		return 1;
	}
	if (lookup_topk(new.source_num, topk_root, value))		// Use the lookupa function to determine if the caller is already in the tree, add the vallue and return. if not proceeds to insert a new node.
		return 1;
	if ((new_node = (struct topk_node_type *)malloc(sizeof(struct topk_node_type))) == NULL)	// Returns error value (-1) to insert function in case that memory allocation error appears
		return -1;
	new_node->par = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	Nnodes++;											// Global var Nnodes is increased by 1
	strcp(new_node->number,new.source_num,0);				// Put values in the new_node
	new_node->val =	value;								
	insert_node_pos(new_node, topk_root, Nnodes);			// Function to insert a new node in the topk tree
	return 1;
}

void insert_node_pos(struct topk_node_type *node, struct topk_node_type *root, int N)
{
	int l = 0, a;

	if (root->left == NULL){
		root->left = node;
		node->par = root;
		heapify_topk(node);
	}
	else if (root->right == NULL){
		root->right = node;
		node->par = root;
		heapify_topk(node);
	}
	else{
		for(l = 0; N >= mypow(2,l+1); l++)
			;
		a = ((mypow(2,l))+mypow(2,l+1))/2;
		if (N < a){
			insert_node_pos(node, root->left, N-mypow(2,l-1));
		}
		else{
			insert_node_pos(node, root->right, N-mypow(2,l));
		}
	}
}

int lookup_topk(char *source_num, struct topk_node_type *root, float value)
{
	if (strcomp(root->number, source_num,0) == 0){
		root->val += value;
		heapify_topk(root);
		return 1;
	}
	if (root->left != NULL)
		return lookup_topk(source_num, root->left, value);
	if (root->right != NULL)
		return lookup_topk(source_num, root->right, value);
	return 0;
}


void heapify_topk(struct topk_node_type *node)
{
	char tempnumber[15];
	int tempval;

	if (node->par != NULL && node->val>node->par->val){
		strcp(tempnumber,node->par->number,0);
		strcp(node->par->number,node->number,0);
		strcp(node->number,tempnumber,0);
		tempval = node->par->val;
		node->par->val = node->val;
		node->val = tempval;
		heapify_topk(node->par);
	}
}

void topk(int k)
{
	int next_pos_ptr, avail_pos_ptr, i;
	struct topk_node_type *max_array[Nnodes];
	float perc, search_value, cur_value = 0;

	search_value = (k * tot_value)/100.0;
	perc = ((float)(k)*100.0/(float)tot_value);
	printf("--searching (%d%%) best callers (value %.2f)--\n", k, search_value);
	for (i = 0; i < Nnodes; i++)
		max_array[i] = NULL;
	if (Nnodes == 0){
		printf("No data\n");
		return;
	}
	max_array[0] = topk_root;
	cur_value += topk_root->val;
	next_pos_ptr = 0;
	avail_pos_ptr = 1;
	while(cur_value < search_value){
		if (max_array[next_pos_ptr]->left != NULL){
			array_insert(max_array, max_array[next_pos_ptr]->left, next_pos_ptr, avail_pos_ptr);
			avail_pos_ptr++;
		}
		if (max_array[next_pos_ptr]->right != NULL){
			array_insert(max_array, max_array[next_pos_ptr]->right, next_pos_ptr, avail_pos_ptr);
			avail_pos_ptr++;
		}
		if ((cur_value += max_array[next_pos_ptr+1]->val) <= search_value)
			next_pos_ptr ++;
	}
	for (i = 0; i <= next_pos_ptr; i++){
		perc = ((float)(max_array[i]->val)*100.0/(float)tot_value);
		printf("%d. %s, value: %.2f, %.2f%% \n",i+1, max_array[i]->number, max_array[i]->val, perc);
	}
}

void array_insert(struct topk_node_type *max_array[], struct topk_node_type *node, int ptr1, int ptr2)
{
	int i, j;

	for(i = ptr1+1; i <= ptr2; i++){
		if (max_array[i] == NULL)
			max_array[i] = node;
		else if (max_array[i]->val < node->val){
			for(j = ptr2; j > ptr1; j--){
				max_array[j] = max_array[j-1];
			}
			max_array[i] = node;
			i = ptr2+1;
		}
	}
}


void free_topk(struct topk_node_type *root)
{
	if (root->left != NULL)
		free_topk(root->left);
	if (root->right != NULL)
		free_topk(root->right);
	free(root);
}