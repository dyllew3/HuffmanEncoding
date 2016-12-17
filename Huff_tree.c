#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Huff_tree.h"

//creates new huffman node for tree
struct huff_node * new_huff(int is_leaf, int amount ,char c){
  
  struct huff_node * result = malloc(sizeof(struct huff_node));
  result->is_leaf = is_leaf;
  result->amount = amount;
  result->c = c;
  result->left_branch = NULL;
  result->right_branch = NULL;
  return result;
}

//combines two huff node leaves
struct huff_node * combine(struct huff_node * this, struct huff_node * that){

  int total = (this->amount) + (that->amount);
  //creates new node
  struct huff_node * combo = new_huff(0, total ,'\0');
  combo->left_branch = malloc(sizeof(struct huff_node));
  combo->right_branch = malloc(sizeof(struct huff_node));
  //assigns branches based on which has a greater amount
  if((this->amount) > (that->amount)){
    
    combo->left_branch = that;
    combo->right_branch = this;
    
  }
  else{
    combo->left_branch = this;
    combo->right_branch = that;
   }
  return combo;
}
//gets node at coo-ordinate, goes from most significant bit 
struct huff_node * get(int value,  struct huff_node * tree){

  if(tree->is_leaf){
    return tree; 
  }
  else if(tree == NULL){
    printf("Huffman tree is empty");
    return NULL;
  }
  else{
    if(value){
      
      return(tree->right_branch);
    }
    else{
      return(tree->left_branch);
    }
    
    
  }
  
  
  
}
//builds tree based on int array
struct huff_node ** build_tree(unsigned int * array, int length){
  
  struct huff_node ** huff_tree = malloc(sizeof(struct huff_node *) * length);
  int i ;
  for(i = 0 ; i < length; i++){
    
    huff_tree[i] = new_huff(1, array[i],i);    
  }
  return huff_tree;
  
  
}

//returns co-ordinates of all characters in tree
unsigned char ** coordinates(struct huff_node ** tree, int size){
	

  
  unsigned char ** result = malloc(sizeof(unsigned char*) * 256);
  char values[256];
  get_coord(tree[0], result,values , 0);
  return result;
  
}

//gets co-ordinates of all the nodes of the tree in theta(N) time
void  get_coord(struct huff_node * starting_node, unsigned char ** coords, char values[], int index){
  if(starting_node->is_leaf == 0){
    values[index] = '1';
    get_coord(starting_node->right_branch,coords,values ,(index + 1) );
    values[index] = '0';
    get_coord(starting_node->left_branch,coords,values, (index + 1));
  }
  else {
    
    unsigned int c = starting_node->c;
    values[index] = '\0';
    coords[c] = malloc(sizeof(unsigned char) * 256);
    strcpy(coords[c],values);
  }
  
}

//print all the coordinates in the tree using recursion
void print_coords(struct huff_node * starting_node, char string[],int index){
  if(starting_node->is_leaf){
    
    printf("Character %c has code %s\n", starting_node->c  ,(string));
  }
  else {
    string[index] = '1';
    string[index + 1] = '\0';
    print_coords(starting_node->right_branch, string, index+1);
    string[index] = '0';
    print_coords(starting_node->left_branch,string, index+1);
  }
  
  
}
//converts a node's coordinate(int) into a character array of 1's and 0's  
char * conver_coord(unsigned int coord, int end){
  
  
  char * result = malloc(sizeof(char) * end);
  int i;
  for(i = 0; i < end; i++){
    result[i] = ((coord & (1<<i)) >> i) + '0';
    
  }
  return result;
}
