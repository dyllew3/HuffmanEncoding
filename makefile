all: huff
huff: Huffman.c Huff_tree.o
	gcc -o huff Huffman.c Huff_tree.o -g
Huff_tree.o: Huff_tree.c Huff_tree.h
	gcc -c Huff_tree.c Huff_tree.h

clean:
	rm *.h.gch

