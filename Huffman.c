#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Huff_tree.h"



const int MAX_CHARS = 256;

struct bitfile{
  FILE * file;
  int bits;
  int index;
};


int  to_string(unsigned char byte,char string[],int start);

int length(char * string);

void encode_to_file(FILE * file_dest,FILE * file_source, unsigned char ** coords);

void compress_tree(struct huff_node ** tree, int size);

struct huff_node * remove_min(struct huff_node ** tree, int size);

void print_array(unsigned int * array, int length);

void set_to_zero(unsigned int array[], int length);

int equals(char * string1, char * string2);

struct huff_node ** trimmed(struct huff_node ** char_array, int length);

int get_size(struct huff_node ** tree);

void decode(FILE * dest, FILE * source,struct huff_node * tree);

void shift_array( char array[]);

int main(int argc, char ** argv){

  FILE * file;
  FILE * file2;
  FILE * file3;
  struct bitfile * bit_file;
  struct huff_node ** tree;
  file = fopen(argv[2], "r");
   unsigned char c;
    unsigned int char_array[MAX_CHARS];
    int i = 0;
    for(; i < MAX_CHARS; i++){
      char_array[i] = 0;
    }
   
  assert( file != NULL );
  unsigned char prev_char = c;
  c = fgetc(file);	// attempt to read a byte
  ;
  while( !feof(file) ) {
    
    prev_char = c;
    char_array[c] += 1;
    c = fgetc(file);
  }
    
  tree = build_tree(char_array, MAX_CHARS);
    
  
  //gets size of tree then compresses it down to one 
  int size = get_size(tree);
  compress_tree(tree, size);
  unsigned char ** coords = coordinates(tree, MAX_CHARS);

  if(equals((argv[1]), "encode")){
    char string[MAX_CHARS];
    print_coords(tree[0],string,0);
    
	rewind(file);
    file2 = fopen(argv[3], "w");
    encode_to_file(file2,file,coords);
    
    
  }
  else if(equals((argv[1]), "decode")){

   
    rewind(file);
    
    unsigned char c;
    //checks that both files can be used 
    file2 = fopen(argv[3],"r");
    file3 = fopen(argv[4],"w");
    assert(file2 != NULL);
    assert(file3 != NULL);
    
    
   //decodes files 
    decode(file3,file2,tree[0]);
	fclose(file3);
  }
  else{
    printf("Not valid command\n");
  } 
  fclose(file);
  fclose(file2);
  
  return 0;
}

//sets all elements to zero in an array
void set_to_zero(unsigned int array[], int length){

  int i ;
  for (i = 0; i < length; i++){
        
    array[i] = 0;
  }
    

}



//checks if two null terminated strings are empty
int equals(char * string1, char * string2){
  
  int index  = 0 ;
  
  while(string1[index] != '\0' && string2[index] != '\0'){
    
    if(string1[index] != string2[index])return 0;
    
    index++;
  }
  if(string1[index] != string2[index]) return 0;
  else return 1;
  
}

//gets number of nodes in tree(with a zero indicating the end)
int get_size(struct huff_node ** tree){
  
  int i = 0;
  while(tree[i] != NULL){
    
    
    i++;
    
  }
  return i;

}


//gets rid of all zero values in an array
//ends array with null 
struct huff_node ** trimmed(struct huff_node ** tree, int length){
  
  struct huff_node ** trim_array = malloc(sizeof(struct huff_node *) * length) ;
  int i = 0;
  int max = 0;
  int max_char = '0';
  int new_index = 0;
  for (i = 0; i < length; i++){
    
    if(tree[i]->amount){


     trim_array[new_index] = tree[i];
      new_index++;
    }
    
    
    
  }
  
  trim_array[new_index] = NULL;
  
  return trim_array;
  
}

//compress tree down to one node by combining two min nodes into
//one then repeating this proces until only one node remains
void compress_tree(struct huff_node ** tree, int size){
  int num_freq = size;
  while(num_freq > 1){
    struct huff_node * min_1 = remove_min(tree,num_freq);
    num_freq--;
    
    struct huff_node * min_2 = remove_min(tree,num_freq);
    num_freq--;
    
    struct huff_node * result = combine(min_1, min_2);
    tree[num_freq] = result;
    num_freq++;
  }
  
}

//removes min from tree, puts last index node into min's pos
//then sets the last index to null
struct huff_node * remove_min(struct huff_node ** tree, int size){

  struct huff_node * min = tree[0] ;
  
  int min_pos = 0;
  int i;
  for (i = 0; i < size; i++){
    //compares amounts
    if(min->amount > tree[i]->amount){
      min = tree[i];
      min_pos = i;
    }
    //checks if they are equal and if they it chooses the one with the greatest
    //ascii character
    else if(((min->amount) == (tree[i]->amount)) && (tree[i]->c > min->c)){
      min_pos = i;
      min = tree[i];

    }
    
    
  }
  tree[min_pos] = tree[size - 1];
  
  
  tree[size-1] = NULL;
  return min;

}
//encodes file to another file
void encode_to_file(FILE * file_dest, FILE * file_source, unsigned char ** coords){
  
  assert(file_dest != NULL);
  assert(file_source != NULL);
  int index = 0;
  
  unsigned char byte = 0;
  unsigned char c = fgetc(file_source);
  while(!feof(file_source)){

    

    int start = 0;
    int len = length(coords[c]); 
    //This checks if the number of directions in the bit
    //plus the current number of directions are enough to fill
    //a byte
    while(len + index > 7 ){
      while(index < 8){
	
	byte = byte | ((coords[c][start] -'0') << index);
	start++;
	len--;
	index++;
      }
      //writes full byte to file
      fputc(byte,file_dest);
      byte = 0;
      index = 0;
    }
    while(coords[c][start] != '\0'){
      byte = byte | ((coords[c][start] -'0') << index);
      start++;
      
      index++;
	

    }
    c = fgetc(file_source);
    
  }
  
  char end = 4;
  //encodes the end of file character
  if(index != 0)fputc(byte,file_dest);
  fputc(end,file_dest);
  
}

//gets length of null terminated char array
int length(char * string){

  int i = 0;
  while(string[i] != '\0'){

    i++;
  } 
  return i;

}

//converts byte to string form, returns new start
int to_string(unsigned char byte,char string[],int start){
  
  int j;
  for(j=0; j < 8; j++){
    
    string[start] = '0' + ((byte & (1 << j))>> j);
    start++;
  }
  string[start] = '\0';
  return start;
}

//decodes encoded file
void decode(FILE * dest, FILE * source,struct huff_node * tree){

  //rewinds in case file has already been used
  rewind(source);
  //first node of huffman tree
  struct huff_node * starting = tree;
  char string[MAX_CHARS];
  unsigned char c;
  int start = 0;
  
  ;
  int value  = 0;
  c = fgetc(source);
  
  while(!feof(source) ){

    
    //keeps going until it reaches a leaf
    while(starting->is_leaf == 0 ){
      //if its not the end of the string it goes to next node
      //else it gets the next byte
      if(start < 8){
	 	value =  ((c & (1 << start)) >> start) ; 
	 	
		start++;
		starting = get(value,starting);
		
      }
      else {
		start = 0;
		c = fgetc(source);
	//adds new directions to the end
	
     }
    }
    //makes sure it's the next character
    
    
   unsigned char d = starting->c;
   fputc(d,dest);
   printf("%c",starting->c);
    
    starting = tree;
    
  }
  
 
  
}

