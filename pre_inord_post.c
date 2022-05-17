#include<stdio.h>
#include<stdlib.h>

typedef struct node 
{
    int data;
    struct node *left;
    struct node *right;
}node;

node* create(int data)
{
	node *temp;
	temp = (node*)malloc(sizeof(node));
	temp->data=data;
	temp->left=NULL;
	temp->right=NULL;
	
	return (temp);
}
void pre_order(node *temp)
{
	if(temp==NULL)
	{
		return ;
	}
	
	printf("%d ",temp->data);
	
	pre_order(temp->left);
	
	
	pre_order(temp->right);
	
}
void in_order(node *temp)
{
	if(temp==NULL)
	{
		return ;
	}
	
	in_order(temp->left);
	
	printf("%d ",temp->data);
	
	in_order(temp->right);
	
}
void post_order(node *temp)
{
	if(temp==NULL)
	{
		return ;
	}
	
	post_order(temp->left);
	
	
	post_order(temp->right);
	
	printf("%d ",temp->data);
}
void main()
{
	node *root;
	root=create(1);
	root->left=create(2);
	root->right=create(3);
	root->right->left=create(10);
	root->right->left->left=create(100);
	root->right->right=create(11);
	root->left->left=create(4);
	root->left->right=create(5);
	
       printf("Printing Pre-order Elements :");
       pre_order(root);
       printf("\n");
       printf("Printing in-order Elements :");
       in_order(root);
       printf("\n");
       printf("Printing Post-order Elements :");
       post_order(root);
       printf("\n");
       

    
}
