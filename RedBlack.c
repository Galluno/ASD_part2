#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//0==black 1==red

struct RBTnode{
int key;
char *s;
struct RBTnode *right;
struct RBTnode *left;
struct RBTnode *parent;
int color;
};

struct RBT{
struct RBTnode *root;
struct RBTnode *leaf;
};

struct RBTnode* new_tree_node(int data, char *v) {
struct RBTnode* n = malloc(sizeof(struct RBTnode));
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
struct RBTnode *nil_node = malloc(sizeof(struct RBTnode));
nil_node->left = nil_node->right = nil_node->parent = NULL;
nil_node->color = 0;
nil_node->key = 0;
nil_node->s="";
t->leaf = nil_node;
t->root = t->leaf;

return t;
}
//......................................................................................................................................................................

void RBTleft_rotate(struct RBT *t, struct RBTnode *x) {
struct RBTnode *y = x->right;
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


void RBTright_rotate(struct RBT *t, struct RBTnode *x) {
struct RBTnode *y = x->left;
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


void RBTinsertion_fixup(struct RBT *t, struct RBTnode *z) {
while(z->parent->color == 1) {
  if(z->parent == z->parent->parent->left) { //z.parent is the left child

    struct RBTnode *y = z->parent->parent->right; //uncle of z

    if(y->color == 1) { //case 1
      z->parent->color = 0;
      y->color = 0;
      z->parent->parent->color = 1;
      z = z->parent->parent;
    }
    else { //case2 or case3
      if(z == z->parent->right) { //case2
        z = z->parent; //marked z.parent as new z
        RBTleft_rotate(t, z);
      }
      //case3
      z->parent->color = 0; //made parent black
      z->parent->parent->color = 1; //made parent red
      RBTright_rotate(t, z->parent->parent);
    }
  }
  else { //z.parent is the right child
    struct RBTnode *y = z->parent->parent->left; //uncle of z

    if(y->color == 1) {
      z->parent->color = 0;
      y->color = 0;
      z->parent->parent->color = 1;
      z = z->parent->parent;
    }
    else {
      if(z == z->parent->left) {
        z = z->parent; //marked z.parent as new z
        RBTright_rotate(t, z);
      }
      z->parent->color = 0; //made parent black
      z->parent->parent->color = 1; //made parent red
      RBTleft_rotate(t, z->parent->parent);
    }
  }
}
t->root->color = 0;
}
//....................................................................................................................................................................


void RBTinsert(struct RBT *t, struct RBTnode *z) {
struct RBTnode* y = t->leaf; //variable for the parent of the added RBTnode
struct RBTnode* temp = t->root;

while(temp != t->leaf) {
  y = temp;
  if(z->key < temp->key)
    temp = temp->left;
  else
    temp = temp->right;
}
z->parent = y;

if(y == t->leaf) { //newly added RBTnode is root
  t->root = z;
}
else if(z->key < y->key) //data of child is less than its parent, left child
  y->left = z;
else
  y->right = z;

z->right = t->leaf;
z->left = t->leaf;

RBTinsertion_fixup(t, z);
}
//...................................................................................................................................................................
//PROCEDURA PER LA STAMPA
void RBTshow(struct RBT *t, struct RBTnode *trav)
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
      RBTshow(t, trav->left);
      RBTshow(t, trav->right);
    }
}

//....................................................................................................................................................................
void RBTdestroyTree(struct RBT *t, struct RBTnode *trav){
    if(trav == t->leaf){
        return;;
    }else{
    RBTdestroyTree(t, trav->right);
    RBTdestroyTree(t, trav->left);
    free(trav);
    }
}


//....................................................................................................................................................................
//PROCEDURA DI RICERCA
struct RBTnode* RBTfind(struct RBT *t, struct RBTnode *trav, int k)
{
    if(trav==t->leaf || trav->key==k) //if root->data is x then the element is found
        return trav;
    else if(k>trav->key) // x is greater, so we will search the right subtree
        return RBTfind(t, trav->right, k);
    else //x is smaller than the data, so we will search the left subtree
        return RBTfind(t, trav->left, k);
}
//......................................................................................................................................

/*int main ()
{

  char op[20];  //stringa contenente l'operazione che voglio eseguire: RBTinsert RBTfind delete
  
     //-memorizzo le stringhe dei nodi in un array v.
     //-per come C( dimensionato possiamo al massimo inserire 100 nodi(dato che puC2 contenere al max 100 stringhe)
     //e ogni nodo puC2 contenere un stringa di al massimo 100 caratteri
   
  char v[100][100];
  int i = 0;
  struct RBT *t = new_red_black_tree();
  int k;
  struct RBTnode *root = NULL;
  //printf ("operazione da eseguire: \n");
  scanf("%s", op);  //ricavo l'operazione che voglio eseguire da stdin
 



  while (strcmp (op, "exit") != 0)
    {
      
      //inserimento
      if(strcmp(op, "RBTinsert")==0){
          scanf("%i", &k);
          scanf("%s", v[i]);   //se faccio un RBTinsert ricavo la stringa da passare al nodo da stdin
          struct RBTnode *n= new_tree_node(k,v[i]);
          RBTinsert(t,n);
	    
	  //ricerca
      }else if(strcmp(op, "RBTfind")==0){
           scanf("%i", &k);
           printf("%s \n",RBTfind(t, t->root,k)->s);  //se voglio eseguire una ricerca cerco il nodo con chiave k e stampo la sua stringa
      }else{
           RBTshow(t,t->root);
           printf("\n");
      }
      

      //printf ("operazione da eseguire: \n");
      scanf ("%s", op);
      i++;
    }



  RBTdestroyTree(t, t->root); //deallocazione dell'albero
  free(t->leaf);
  free(t);
  return 0;
}*/