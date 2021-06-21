#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//0==black 1==red

struct node{
int key;
char *s;
struct node *right;
struct node *left;
struct node *parent;
int color;
};

struct RBT{
struct node *root;
struct node *leaf;
};

struct node* new_tree_node(int data, char *v) {
struct node* n = malloc(sizeof(struct node));
n->key = data;
n->s=v;
n->color = 1;
n->left = NULL;
n->right = NULL;
n->parent = NULL;
return n;
}

struct RBT *new_red_black_tree() {
struct RBT *t = malloc(sizeof(struct RBT));
struct node *nil_node = malloc(sizeof(struct node));
nil_node->left = nil_node->right = nil_node->parent = NULL;
nil_node->color = 0;
nil_node->key = 0;
nil_node->s="";
t->leaf = nil_node;
t->root = t->leaf;

return t;
}
//......................................................................................................................................................................

void left_rotate(struct RBT *t, struct node *x) {
struct node *y = x->right;
x->right = y->left;
if(y->left != t->leaf) {
  y->left->parent = x;
}
y->parent = x->parent;
if(x->parent == t->leaf) { //x is root
  t->root = y;
}
else if(x == x->parent->left) { //x is left child
  x->parent->left = y;
}
else { //x is right child
  x->parent->right = y;
}
y->left = x;
x->parent = y;
}


void right_rotate(struct RBT *t, struct node *x) {
struct node *y = x->left;
x->left = y->right;
if(y->right != t->leaf) {
  y->right->parent = x;
}
y->parent = x->parent;
if(x->parent == t->leaf) { //x is root
  t->root = y;
}
else if(x == x->parent->right) { //x is left child
  x->parent->right = y;
}
else { //x is right child
  x->parent->left = y;
}
y->right = x;
x->parent = y;
}
//....................................................................................................................................................................


void insertion_fixup(struct RBT *t, struct node *z) {
while(z->parent->color == 1) {
  if(z->parent == z->parent->parent->left) { //z.parent is the left child

    struct node *y = z->parent->parent->right; //uncle of z

    if(y->color == 1) { //case 1
      z->parent->color = 0;
      y->color = 0;
      z->parent->parent->color = 1;
      z = z->parent->parent;
    }
    else { //case2 or case3
      if(z == z->parent->right) { //case2
        z = z->parent; //marked z.parent as new z
        left_rotate(t, z);
      }
      //case3
      z->parent->color = 0; //made parent black
      z->parent->parent->color = 1; //made parent red
      right_rotate(t, z->parent->parent);
    }
  }
  else { //z.parent is the right child
    struct node *y = z->parent->parent->left; //uncle of z

    if(y->color == 1) {
      z->parent->color = 0;
      y->color = 0;
      z->parent->parent->color = 1;
      z = z->parent->parent;
    }
    else {
      if(z == z->parent->left) {
        z = z->parent; //marked z.parent as new z
        right_rotate(t, z);
      }
      z->parent->color = 0; //made parent black
      z->parent->parent->color = 1; //made parent red
      left_rotate(t, z->parent->parent);
    }
  }
}
t->root->color = 0;
}
//....................................................................................................................................................................


void insert(struct RBT *t, struct node *z) {
struct node* y = t->leaf; //variable for the parent of the added node
struct node* temp = t->root;

while(temp != t->leaf) {
  y = temp;
  if(z->key < temp->key)
    temp = temp->left;
  else
    temp = temp->right;
}
z->parent = y;

if(y == t->leaf) { //newly added node is root
  t->root = z;
}
else if(z->key < y->key) //data of child is less than its parent, left child
  y->left = z;
else
  y->right = z;

z->right = t->leaf;
z->left = t->leaf;

insertion_fixup(t, z);
}
//...................................................................................................................................................................
//PROCEDURA PER LA STAMPA
void show(struct RBT *t, struct node *trav)
{
  if (trav == t->leaf)
    { 
      printf ("NULL ");
    }
  else
    {
      printf ("%d:", trav->key);	//stampa della chiave e della stringa del nodo, es: 5 lorenzo

      for (int i = 0; trav->s[i] != '\0'; i++)
	     {
	         printf ("%c", trav->s[i]);
	     }
      if(trav->color==1){
          printf(":%s ", "red");
      }else{
           printf(":%s ", "black");
      }
      
      
      
      //chiamata di stampa sul figlio sx e dx
      show(t, trav->left);
      show(t, trav->right);
    }
}

//....................................................................................................................................................................
void destroyTree(struct RBT *t, struct node *trav){
    if(trav == t->leaf){
        return;;
    }else{
    destroyTree(t, trav->right);
    destroyTree(t, trav->left);
    free(trav);
    }
}


//....................................................................................................................................................................
//PROCEDURA DI RICERCA
struct node* find(struct RBT *t, struct node *trav, int k)
{
    if(trav==t->leaf || trav->key==k) //if root->data is x then the element is found
        return trav;
    else if(k>trav->key) // x is greater, so we will search the right subtree
        return find(t, trav->right, k);
    else //x is smaller than the data, so we will search the left subtree
        return find(t, trav->left, k);
}
//......................................................................................................................................

int main ()
{

  char op[20];  //stringa contenente l'operazione che voglio eseguire: insert find delete
  /*
     -memorizzo le stringhe dei nodi in un array v.
     -per come C( dimensionato possiamo al massimo inserire 100 nodi(dato che puC2 contenere al max 100 stringhe)
     e ogni nodo puC2 contenere un stringa di al massimo 100 caratteri
   */
  char v[100][100];
  int i = 0;
  struct RBT *t = new_red_black_tree();
  int k;
  struct node *root = NULL;
  //printf ("operazione da eseguire: \n");
  scanf("%s", op);  //ricavo l'operazione che voglio eseguire da stdin
 



  while (strcmp (op, "exit") != 0)
    {
      
      //inserimento
      if(strcmp(op, "insert")==0){
          scanf("%i", &k);
          scanf("%s", v[i]);   //se faccio un insert ricavo la stringa da passare al nodo da stdin
          struct node *n= new_tree_node(k,v[i]);
          insert(t,n);
	    
	  //ricerca
      }else if(strcmp(op, "find")==0){
           scanf("%i", &k);
           printf("%s \n",find(t, t->root,k)->s);  //se voglio eseguire una ricerca cerco il nodo con chiave k e stampo la sua stringa
      }else{
           show(t,t->root);
           printf("\n");
      }
      

      //printf ("operazione da eseguire: \n");
      scanf ("%s", op);
      i++;
    }



  destroyTree(t, t->root); //deallocazione dell'albero
  free(t->leaf);
  free(t);
  return 0;
}
