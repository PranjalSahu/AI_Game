#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int flagtype=-1;		// which method to apply 1 DFS,  2 BFS, 3 A*
int getnum_x=0;			// getnum sets it to the index of x in string
int getnum_num=0;		// getnum sets it to the unique value of number 
						// formed from bit represantation of string

int  g_count =0;		// keeps the count of total nodes expanded 
int  g_x =0;			// will store the position of blank in operateon string
char succ[50];			// for printing state obtained by applying the successor function
char operateon[50];		// the current state on which the successor function will be applied
						// 	this will be set in expand nodes function before calling the move function

char **check;			// for checking loops , checking open or closed
						// 1 means closed i.e. already visited

typedef struct node{		// structure for storing the states	
	char s[50];
	int priority;
	struct node *next;
	struct node *parent;
}node;

int N;						// length of the input string

node *closed;
node *openlists;			// dummy node representing the first node of openlist
node *openliste;			// dummy node representing the last node of openlist

/*void printsucc(){
	printf("%s\n", succ);
}*/

// returns 1 if goal state else returns 0
int checkgoal(char *str){	
	int i=0, myflag = 0;
	char s[50];
	strcpy(s, str);
	
	while(i<N){
		if(s[i] == 'W' && myflag ==1)	// not a goal state
			return 0;
			
		if(s[i] == 'B'){			// first balck occured
			myflag = 1;
			//break;
		}	
		++i;	
	}
	
	if(i==N)
		return 1;
}

void initfunc(){
	int i;
	int k = 1024*1024;//pow(2, N);
	
	g_count = 0;
	g_x     = 0;
	closed = (node *)malloc(sizeof(node));
	closed->next = NULL;
	
	openlists = (node *)malloc(sizeof(node));
	openliste = (node *)malloc(sizeof(node));
	
	openlists->next = NULL;
	openliste->next = NULL;	
	
	check = (char **)malloc(sizeof(char *)*(1024*1024));
	for(i=0;i<k;++i)
		check[i] = (char *)malloc(sizeof(char)*21);	
	
	return;
}

void getnum(char *str){
		int i;		
		int num = 0;
		for(i=0;i<N;++i){
			if(str[i] == 'B'){			
				num = num<<1;
				num = num|1;
			}
			else if(str[i] == 'W'){
				num = num<<1;			
			}
			else{
				getnum_x = i;
			}	
		}
		
		getnum_num = num;
		return;
}	

int checkinclosed(char *str){
	int i, j;
	getnum(str);
	i = getnum_num;
	j = getnum_x;
	if(check[i][j] == 1)
		return 0;
	return 1;				//return 1 if not in closed
}	

void insertinclosed(char *str){	
	int i, j;
	getnum(str);
	i = getnum_num;
	j = getnum_x;	
	check[i][j] = 1;		// inserted in closed
	return;
	/*node *temp;
	node *newnode;			// newnode to be inserted
	temp = closed;
	
	while(temp->next != NULL){	// getting the last node in the list
		temp = temp->next;
	}
	
	newnode = (node *)malloc(sizeof(node));	// inerting new node
	newnode->next = NULL;
	strcpy(newnode->s, str);
	temp->next = newnode;
	return;*/
}

// heueristic function calculates the number of 'W' before 1st occurence of 'B'
int getpriority(char *str){	
	int i=0;
	while(str[i] =='W' && i<N){
		++i;
	}
	//printf("returning priority %d\n", i);
	return i;
}	

// inserts the node in the open list depending on the flag
// flagtype = 1 means insert at start
// flagtype = 2 means insert at end
// flagtype = 3 means insert with priority

void insertinopen(char *str, node *currnode){
	
	node *newnode;								// newnode to be inserted
	newnode = (node *)malloc(sizeof(node));		// inserting new node
	
	newnode->parent = currnode;					// setting the parent for storing path
	strcpy(newnode->s, str);
	
	if(openlists->next == NULL){				// if the list is empty			
		openliste->next = newnode;				// setting the last 		
		openlists->next = newnode;
		newnode->next   = NULL;					// inserting at start
		newnode->priority = getpriority(newnode->s);
		return;
	}
	
	// LIST IS NON EMPTY
	if(flagtype == 1){
		newnode->next   = openlists->next;		// inserting at start		
		openlists->next = newnode;
	}
	else if(flagtype == 2){						// inserting at end
		newnode->next   		= NULL;		
		(openliste->next)->next = newnode;
		openliste->next = newnode;
	}
	else if(flagtype == 3){							// insert with some priority
		int tp;
		newnode->priority = getpriority(newnode->s);	
		tp = newnode->priority;
		node *temp1;
		temp1 = openlists;
		while(temp1->next!= NULL){
				if((temp1->next)->priority<=tp)
					break;
				temp1 = temp1->next;	
		}
		newnode->next = temp1->next;
		temp1->next   = newnode; 
	}	
	return;
}

// it will apply the successor function on the string present in operateon global variable
// REMEMBER a is direct index >>> therefore dont subtract 1
// it also prints the move statement
void move(int a, node *currnode){	

	char t, tt=0;
	char temp[50];
	strcpy(temp, operateon);
	
	if(temp[a] == 'x')						// trying to move blank itself >> WRONG
		return;
	
	t 	      	  =	temp[g_x];
	temp[g_x] 	  = temp[a];
	temp[a]       =	t;	
	
	strcpy(succ, temp);						// storing the successor obtained after this operation in succ
	
	if(checkinclosed(succ)){				// if already not visited
		insertinopen(succ, currnode);		// inserting in open list	
		tt = 1;
	}	
	/*if(tt==1){
		//printf("move %d   %s >> %s <inserting>\n", a+1, operateon, succ);		
		//printf("move %3d \t%s\n", a+1, succ);
	}	
	else{
		//printf("move %d   %s >> %s <done>\n", a+1, operateon, succ);		
		//printf("move %3d \t%s\n", a+1, succ);
	}*/	
	return;
}

node *removefromopen(){				// removes the first node in the open list
	if(openlists->next == NULL)		// list is empty
			return NULL;
	
	node *output;
	output = openlists->next;
	openlists->next = (openlists->next)->next;
	/*if(output!=NULL)
		printf("IN removefromopen %s\n", output);*/
	return output;
}

int printpath(node *currnode){
		int a;
		if(currnode->parent == NULL){
			getnum(currnode->s);
			return getnum_x;
		}	
		a = printpath(currnode->parent);
		getnum(currnode->s);
		printf("move %3d %s\n", getnum_x+1, currnode->s);
		return getnum_x;
}
// recursive method
void expandnodes(){		// function to generate all possible nodes

	node *currnode = removefromopen();	
	char *curr = currnode->s;
	if(curr ==  NULL){
		printf("FAILURE\n");
		exit(0);
	}	
	else if(checkinclosed(curr)==0){	// if it is in closed then remove another node
			expandnodes();
			return;
	}
	
	//printf("APPLYING SUCSESSOR FUNCTION\n");
	int x, i;
	
	++g_count;							// nodes checked count increased
	
	if(checkgoal(curr) == 1){
		printpath(currnode);
		printf("\n\nGOAL STATE: %s\n", curr);
		printf("\n\nNUMBER OF NODES CHECKED = %d\n",g_count);		// END THE SEARCH
		exit(0);
		//return;
	}	
	
	// ELSE expanding the nodes	
	for(i=0;i<N;++i){
		if(curr[i] == 'x'){	// index of blank
			x   = i;		// REMEMBER it is the index
			g_x = x;
			break;
		}
	}
	
	strcpy(operateon, curr);
	int d = 0;
	if(x<N/2){
			/*for(i=0;i<x;++i)
				move(i, currnode);
			for(i=x+1;i<=x+N/2;++i)
				move(i, currnode);*/
			for(i=x-1;i>=0;--i)
				move(i, currnode);
			for(i=x+N/2;i>=x+1;--i)
				move(i, currnode);	
	}
	else if(x>N/2){
			/*for(i=x+1;i<=N-1;++i)
				move(i, currnode);
			for(i=x-N/2;i<=x-1;++i)
				move(i, currnode);*/
			for(i=N-1;i>=x+1;--i)
				move(i, currnode);
			for(i=x-1;i>=x-N/2;--i)
				move(i, currnode);	
	}	
	else{
		for(i=N-1;i>=0;--i)
			move(i, currnode);
	}
	
	insertinclosed(operateon);		// inserting in closed list
	//printf("INSERTED IN CLOSED\n");
	expandnodes();
	return;
}

int main(){		
	char str[50];	
	printf("ENTER THE METHOD TO BE USED:\n");
	printf("\t\tDFS : 1\n");
	printf("\t\tBFS : 2\n");
	printf("\t\tA*  : 3\n");
	
	scanf("%d", &flagtype);
	printf("YOU ENTERED %d as METHOD TYPE\n\n", flagtype);
	printf("ENTER THE INITIAL STATE\n");
	scanf("%s", str);
	printf("YOU ENTERED %s as INITIAL STATE\n\n", str);
	N = strlen(str);	
	initfunc();
	
	insertinopen(str, NULL);	// parent of root node is NULL
	expandnodes();
	return 0;
}

