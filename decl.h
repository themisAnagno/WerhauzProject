#define TRUE 1
#define FALSE 0

// Global Variables inserted during program call
// *********************************************
// =============================================
int hashtableSize1, hashtableSize2, bucketSizeBytes, BUCKETSIZE;
FILE *fp;


// Every primary and secondary structure decleration
// *************************************************
// =================================================
struct cdrtype					// The struct we use to store CDR indfos. Each instance of this structs is dynamically created for every new CDR entry.
{
	char cdrid[11];
	char source_num[15];
	char dest_num[15];
	char date[9];
	char init_time[6];
	int dur;
	char type;
	char tarrif;
	int faultCondition;
	struct cdrtype *nextcdr;	// CDR are stored as a chained dynamic list
};

struct caller_type		// The struct for each caller number
{								// The struct contains the caller num and the a pointer that points to the chain list of CDR of this number.
	char number[15];						
	struct cdrtype *data;
};

struct caller_bucket_list			// The dynamic chain list contain the caller buckets. The Hash Table shows to the first node of this list
{
	struct caller_bucket_list *next;
	struct caller_type *caller_bucket;
};

struct caller_bucket_list **hashtable1, **hashtable2;

// Tree types declerations
// ***********************
// =======================
struct dest_type{					// The max binary heap tree for the topdest operation
	char id[3];
	int n;
	struct dest_type *par;
	struct dest_type *left;
	struct dest_type *right;
};

struct topk_node_type{				// The max binary heap tree for the topk operation
	char number[15];
	float val;
	struct topk_node_type *par;
	struct topk_node_type *left;
	struct topk_node_type *right;
} *topk_root;

int Nnodes;							// Number of nodes for the topk tree (Number of different callers)
float tot_value;						// Total value in the topk tree

// Helper Function declerations
// ****************************** 
// ==============================
void strcp(char *, const char *, int n);
int strcomp(const char *, const char *, int n);
int mypow(int, int);

// Function Decleration
// ********************
// ====================
// main.c/menu.c/file_menu.c functions
int init(void);
void menu(void);
void file_menu(void);
void config_file_handler(FILE *);
float get_value(int, int, int);
int getchoice(void);
void getcdr(void);
int getint(void);
void getdate(char *);
void gethour(char *);
void getnumber(char *);
void operation(int);
// hash.c functions
int input(struct cdrtype);
int hash(char *, int);
struct caller_type * lookup(char *, int);
void printcdr(struct cdrtype *);
void delcdr(char * , char *);
void printhashtable (int);
void free_all(void);
void find(char *, int, char [][6], char [][9]);
int datecomp(char *d1, char *d2);
void topdest(char *);
void treepos(struct dest_type* ,struct dest_type* , int);
int lookup_dest(char *, struct dest_type *);
void heapify(struct dest_type *);
void print_dest_tree(struct dest_type *);
void free_dest(struct dest_type *root);
void indist(char *, char *);				// inactive
void indist_comp(struct cdrtype *, struct cdrtype *);	// inactive
// topk.c functions
int insert_node(struct cdrtype);
void insert_node_pos(struct topk_node_type *, struct topk_node_type *, int);
int lookup_topk(char *, struct topk_node_type *, float);
void heapify_topk(struct topk_node_type *);
void topk(int);
void array_insert(struct topk_node_type **, struct topk_node_type *, int, int);
void free_topk(struct topk_node_type *);