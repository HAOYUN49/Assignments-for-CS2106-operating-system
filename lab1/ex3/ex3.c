#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2
#define COLLAPSE 3 // New instruction compared to ex2.

// Node declarations

typedef struct SUBNODE
{
	int data;
	struct SUBNODE* nextSubNode;
} subNode;

typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
	subNode* subNodeHead;
} node;

// Function prototypes 

void insertNodeNext(int,int,node*); 
void insertNodePrevious(int,int,node*);
void deleteNode(int,node*);
void printList(node*);
void printSubNodes(node*);
void deleteList(node*);
void insertSubNode(int,int,int,node*);
void deleteAllSubNodes(node*);

void collapseSubNodes(int, node*); //TODO 

void (*func[])() = {insertNodeNext, insertNodePrevious, deleteNode, insertSubNode, collapseSubNodes}; 

// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;
	
	// Declaration of the origin Node
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;
	
	
	
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned. 
	{
	       if (instruction == PREVIOUS) {
                 scanf ("%d", &value);
                 (*func[1]) (position, value, originNode);
               }
               else if (instruction == NEXT) {
                 scanf ("%d", &value);
                 (*func[0]) (position, value, originNode);
               }
               else if (instruction == INSERTSUBNODE) {
                 scanf ("%d%d", &subPosition, &value );
                 (*func[3]) (position, subPosition, value, originNode);
               }
               else if (instruction == DELETE) {
                 (*func[2]) (position, originNode);
               }
               else if (instruction == COLLAPSE) {
                 (*func[4]) (position, originNode);
               }
               //You would have to implement input and processing yourself. 
	}
	printList(originNode);
	deleteList(originNode);
	
	printf("Circular List after delete\n");
	printList(originNode);
	
	free(originNode);
	
	return 0;
}


//Function Implementations 

void collapseSubNodes(int position,node* targetNode)
{
	for (int i = 0; i < position; i++) {
          targetNode = targetNode->nextNode;
        }
        int sum = 0;
        subNode *deSubNode = targetNode->subNodeHead, *newHead;
        targetNode->subNodeHead = NULL;
        while (deSubNode != NULL) {
          newHead = deSubNode->nextSubNode;
          sum += deSubNode->data;
          free(deSubNode);
          deSubNode = newHead;
        }
        targetNode->data = targetNode->data + sum;
}

void insertNodeNext(int position,int value, node* originNode)
{
        node *newNode = (node*)malloc(sizeof(node));
        newNode->data = value; newNode->subNodeHead = NULL;
        node *preNode = originNode, *neNode;
        for (int i = 0; i < position; i++) {
          preNode = preNode->nextNode;
        }
        neNode = preNode->nextNode;
        newNode->previousNode = preNode;
        newNode->nextNode = neNode;
        preNode->nextNode = newNode;
        neNode->previousNode = newNode;
	// Use implementation from ex2. 
}	

void insertNodePrevious(int position,int value, node* originNode)
{
        node *newNode = (node*)malloc(sizeof(node));
        newNode->data = value; newNode->subNodeHead = NULL;
        node *preNode = originNode, *neNode;
        for (int i = 1; i < position; i++) {
          preNode = preNode->nextNode;
        }
        if (position == 0) preNode = originNode->previousNode;
        neNode = preNode->nextNode;
        newNode->previousNode = preNode;
        newNode->nextNode = neNode;
        preNode->nextNode = newNode;
        neNode->previousNode = newNode; 	
       // Use implementation from ex2. 
}

void insertSubNode(int position,int subPosition,int value,node* originNode)
{
	subNode *newSubNode = (subNode*)malloc(sizeof(subNode));
        newSubNode->data = value;
        node *inNode = originNode;
        for (int i = 0; i < position; i ++) {
          inNode = inNode->nextNode;
        }
        if (subPosition == 0) {
          newSubNode->nextSubNode = inNode->subNodeHead;
          inNode->subNodeHead = newSubNode;
        } else {
          subNode *preSubNode = inNode->subNodeHead;
          for (int i = 1; i < subPosition; i++) {
            preSubNode = preSubNode->nextSubNode;
          }
          newSubNode->nextSubNode = preSubNode->nextSubNode;
          preSubNode->nextSubNode = newSubNode;
        }
        // Use implementation from ex2. 
}

void deleteAllSubNodes (node* targetNode)
{
       subNode *deSubNode = targetNode->subNodeHead, *newHead;
        targetNode->subNodeHead = NULL;
        while (deSubNode != NULL) {
          newHead = deSubNode->nextSubNode;
          free(deSubNode);
          deSubNode = newHead;
        }	
       // Use implementation from ex2. 
}

void deleteNode (int position,node* originNode)
{
       node *deNode = originNode;
        for (int i = 0; i < position; i++) {
          deNode = deNode->nextNode;
        }
        node *preNode = deNode->previousNode, *neNode = deNode->nextNode;
        deleteAllSubNodes (deNode);
        preNode->nextNode = neNode;
        neNode->previousNode = preNode;
        free(deNode);	
       // Use implementation from ex2. 
}

void deleteList(node* originNode)
{
      node *deNode = originNode->nextNode;
          while (deNode != originNode) {
            deleteNode(1, originNode);
            deNode = originNode->nextNode;
          }
          deleteAllSubNodes(originNode);	
      // Use implementation from ex2. 
}



//Print list has been implemented for you. 
void printList(node* originNode)
{
	int count = 0;
	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);		
	
	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]",0,originNode->data);	
	// printing subNodes
	printSubNodes(originNode);
	printf("\n");

	return;
}

void printSubNodes(node* mainNode)
{
	int count = 0;
	subNode* iterator = mainNode->subNodeHead;
	while(iterator != NULL)
	{
		printf("->[subNode pos %i:%i]",count,iterator->data);
		iterator = iterator->nextSubNode;
		count++;
	}
}
