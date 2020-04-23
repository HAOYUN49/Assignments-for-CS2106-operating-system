#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2 // New instruction compared to ex1. 

// Node and SubNode declarations ( Do not Edit )
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
void insertNodeNext(int,int,node*); // TODO
void insertNodePrevious(int,int,node*); //TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO
void insertSubNode(int,int,int,node*); // TODO
void deleteAllSubNodes(node*); // Optional TODO: will be useful in ex3. 

void printList(node*); // Implemented for you
void printSubNodes(node*); // Implemented for you :Helper function for print list

// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;
	
	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;
	
	// input and processing has been done for you 
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned. 
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
		}
		else if(instruction == INSERTSUBNODE)
		{
			scanf("%i%i",&subPosition,&value);
			insertSubNode(position,subPosition,value,originNode);
		}
		else if(instruction == NEXT)
		{
			scanf("%i",&value);
			insertNodeNext(position,value,originNode);
		}
		else if(instruction == PREVIOUS)
		{
			scanf("%i",&value);
			insertNodePrevious(position,value,originNode);
		}
	}
	
	printList(originNode);
	deleteList(originNode);
	
	printf("Circular List after delete\n");
	printList(originNode);
	
	free(originNode);
	return 0;
}


//Function Implementations 
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
	// Note: you can use a similar implementation as in ex1. 
}

void insertNodeNext(int position, int value, node* originNode)
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
	// Note: you can use a similar implementation as in ex1. 
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
	// Note: you can use a similar implementation as in ex1. 
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
	// it is advised to use a helper function in conjunction with deleteNode(). 
}

void deleteList(node* originNode)
{
	node *deNode = originNode->nextNode;
          while (deNode != originNode) {
            deleteNode(1, originNode);
            deNode = originNode->nextNode;
          }
          deleteAllSubNodes(originNode);
	// Note: you can use a similar implementation as in ex1. 
}

//Print list has been implemented for you. 
// You may study the functions below to help you implement your own functions. 
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
