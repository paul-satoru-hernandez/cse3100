//In this assignment, we practice call by reference.

//Below description of call by reference is from the following link
//https://www.tutorialspoint.com/cprogramming/c_function_call_by_reference.htm

//The call by reference method of passing arguments to a function copies 
//the address of an argument into the formal parameter. Inside the function, 
//the address is used to access the actual argument used in the call. 
//It means the changes made to the parameter affect the passed argument.

//We use an example of linked list for this purpose.
//In the example, an implementation of how to add a node to the head of the list
//is given. We need to implement how to remove a node from the head of the list.
//Both of the functions should keep track of the length of the list.
//If we successuflly added a node to the list, the length should be incremented by one.
//Also, if we successfully rmoeved a node from the list, the length should be decrementd 
//by one.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_tag
{
   int    v;                    // data
   struct node_tag * next;  // A pointer to this type of struct
} node;                        // Define a type. Easier to use.


node * create_node(int v)
{
    node    * p = malloc(sizeof(node)); // Allocate memory
    assert(p != NULL);      // you can be nicer

    // Set the value in the node.
    p->v = v;
    p->next = NULL;
    return p;           // return
}

//This function show us how to add a new node to the font of a linked list,
//at the same time, how to update the length of the list.
//Note the ** in front of head
//Note the * in front of length
//Think why this is call by reference for a pointer and an integer
void add_first(node **head, node *newnode, int *length)
{
    if(*head == NULL)
    {
        *head = newnode;
        newnode->next = NULL;
    }
    else
    {
        newnode->next = *head;
        *head = newnode;
    }
	(*length) ++;
}

//Now we need to implement the remove_first function
node * remove_first(node **head, int *length)
{
	//Add your code below
    node * to_delete = *head;
    if (*head == NULL){
        length = 0;
    }
    else {
        *head = (*head) -> next;
        (*length)--;
    }
    return to_delete;
}

void print_list(node *head)
{   
    while(head!=NULL)
    {   
        printf("%d ", head->v);
        head = head->next;
    }
    printf("\n");
}

//Do not change the code in main function

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s n\n", argv[0]);
		return -1;
	}
	int n = atoi(argv[1]);
	assert(n>=1 && n<=25);
	node *head, *p;
	int length = 0;
	//Do not forget the initilization of head
	head = NULL;

	for(int i=1; i<=n; i++)
	{
		p = create_node(i);
		//Note how we call by reference for head and length
		//Note the & in front of head and length
		add_first(&head, p, &length);
		printf("length=%d\n", length);
		print_list(head);
	}
	while(head!=NULL)
	{
		//Note how we call by reference for head and length
		//Note the & in front of head and length
		p = remove_first(&head, &length);
		printf("length=%d\n", length);
		print_list(head);
		//remember we need to free all the memory allocated from heap
		free(p);
	}
	return 0;
}
