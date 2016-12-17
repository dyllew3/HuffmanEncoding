#ifndef HUFF_TREE
#define HUFF_TREE

struct huff_node{
  
  int is_leaf;
  int amount;
  struct huff_node * left_branch;
  struct huff_node * right_branch;
  unsigned char c;
};

struct huff_node * new_huff(int is_leaf, int amount ,char c);
struct huff_node * combine(struct huff_node * this, struct huff_node * that);
struct huff_node * get(int value ,struct huff_node * tree);
struct huff_node ** build_tree(unsigned int * array, int length);
unsigned char ** coordinates(struct huff_node ** tree, int size);
void get_coord(struct huff_node * starting_node,unsigned char ** coords, char values[], int index );
void print_coord(struct huff_node * starting_node, char * string[],int index);
char * convert_coord(unsigned int coord, int end);


#endif
