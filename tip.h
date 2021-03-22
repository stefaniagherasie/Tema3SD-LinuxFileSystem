/* GHERASIE STEFANIA-ELENA 313 CB */
#define SIZE 40
typedef struct node {
  struct node *children[SIZE];
  int isFile;
  int isDirectory;
} TNode;