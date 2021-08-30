Hashtable.cpp and Hashtable.h are implementations of a hashtable

- at the command line, detail what type of probing you want
  0: linear probing
  1: quadratic
  2: double hashing
  3: USE AVL Tree instead

counting.cpp

- Allows for input.txt which will use hashtable.cpp to create mapping of words to their occurences in the text
- Will also say how long process took
- To run: ./counting input.txt output.txt type_of_probing debug_mode_on_off iterations

Answers to HW6 Questions:

I used input sizes of 32, 306, 3042, and 31946

I used an AVL Tree.

Size 32:
Probe type time per iteration (s) per operation (s)

Linear 0.000029 0.00000259
Quadratic 0.000133 0.000004156
Double Hashing 0.0001044 0.000003625
AVL 0.0000418 0.00000131

Size 306:
Probe type time per iteration (s) per operation (s)

Linear 0.0011815 0.00000386  
Quadratic 0.0014723 0.000004811
Double Hashing 0.0012245 0.000004002
AVL 0.0004575 0.000001495

Size 3042:
Probe type time per iteration (s) per operation (s)

Linear 0.0081754 0.0000026875
Quadratic 0.00904 0.00000297  
Double Hashing 0.00870 0.000002862
AVL 0.004959 0.00000163

Size 31946:
Probe type time per iteration (s) per operation (s)

Linear 0.0612834 0.00000192  
Quadratic 0.061425 0.000001923
Double Hashing 0.06096 0.00000191  
AVL 0.048072 0.000001505

I think my results turned out to be slower than some others because I have some expensive operations when hashing.
Because it made more sense to me to hash based on the reversed string based on the hashing protocol outlined in the
writeup, I reversed every string before hashing it in my reverseString() function.

In terms of how my runtimes changed with different input sizes, I think that it makes sense that the per operations times
were very similar regardless of the size since these are constant no matter how many words are being inserted. I think it
is a little surprising that they did seem to get faster per operation on the larger input sizes. Based on what I know and have read on piazza, I think this is because of caching which means that certain data are going to be stored in an "easy-to-acces" area of memory. This means on larger inputs, there is a higher likelihood of caching helping the runtime on any given word.
I think the time per iterations make sense in the sense that they all grow with the larger input sizes at a relatively steady rate. This makes sense given what I know about hashtable runtimes' constant time growth.

I am surprised the AVL tree performs better than my hashtable in all input sizes. This is very counterintuititve because of the log(n) compared to (1) supposed runtimes for AVL trees vs Hashtables. I think this is because I am doing various operations on top of my hashtable such as reverseString() that are not factored into the traditional analysis used to examine the runtimes of hashtables. Also, in hashtables, you have to resize sometimes which is O(n) while fixing an AVL tree is
O(log n) at worst.
# cpp-hashtable-implementation
