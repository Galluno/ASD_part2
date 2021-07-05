#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//0==black 1==red

struct rb_node{
int key;
char *s;
struct rb_node *right;
struct rb_node *left;
struct rb_node *parent;
int color;
};

//Poichè in un red-black-tree le foglie non sono semplici nodi NULL, ma hanno anche un colore, ossia son
//nere, abbiamo deciso di realizzare una struttura ad-hoc
struct RBT{
struct rb_node *root;
struct rb_node *leaf;
};

struct rb_node* new_tree_node(int data, char *v) {
struct rb_node* n = malloc(sizeof(struct rb_node));
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
struct rb_node *null = malloc(sizeof(struct rb_node));
null->left = null->right = null->parent = NULL;
null->color = 0;
null->key = 0;
null->s="";
t->leaf = null;
t->root = t->leaf;
return t;
}
//......................................................................................................................................................................
/*ROTAZIONI PER RIPRISTINARE LA PROPRIETA' DEI RED-BLACK CHE MI GARANTISCE IL BILANCIAMENTO
  
  Nota: Come nel caso degli avl, anche qui le rotazioni possono modificare la radice dell' albero... tuttavia in questo caso la radice di t sarà contenuta della struct dedicata, 
  dunque se la radice dovesse essere modificata, in quanto è il nodo che funge da perno nella rotazione, semplicemente aggiorniamo t->root


*/
void rb_left_rotate(struct RBT *t, struct rb_node *a) {
struct rb_node *c = a->right;
//c diventerà il nuovo a:
c->parent = a->parent;
//se a era la radice, ora c è la radice dell'albero
if(a->parent == t->leaf) { 
  t->root = c;
}
//altrimenti, c diventerà figlio del parent di a
else if(a == a->parent->left) { 
  a->parent->left = c;
}
else { 
  a->parent->right = c;
}

//a diventerà il figlio sx di c:
a->right = c->left;
if(c->left != t->leaf) {  //il padre del figlio sx di c diventerà a,
  c->left->parent = a;
}
c->left = a;
a->parent = c;
}






void rb_right_rotate(struct RBT *t, struct rb_node *a) {
struct rb_node *b = a->left;
//b diventerà il nuovo a:
b->parent = a->parent;
//se a era la radice, ora b è la radice dell'albero
if(a->parent == t->leaf) { 
  t->root = b;
}
//altrimenti, b diventerà figlio del parent di a
else if(a == a->parent->right) { 
  a->parent->right = b;
}
else { 
  a->parent->left = b;
}

//a diventerà il figlio dx di b:
a->left = b->right;
if(b->right != t->leaf) {  //il padre del figlio dx di b diventerà a,
  b->right->parent = a;
}
b->right = a;
a->parent = b;
}

//....................................................................................................................................................................
/*PROCEDURA PER RIPRISTINARE LE PROPRIETA' DEI RED-BLACK CHE GARANTISCONO IL BILANCIAMENTO DELL' ALBERO

  Come si può vedere nella procedura di inserimento rb_insert(), l'inserimento del nuovo nodo avviene praticamente come nei BST, il nuovo nodo viene poi colorato di rosso.
  Successivamente richiamiamo la procedura rbtFixup() a cui passiamo in input il nuovo nodo aggiunto, che sarà z, e l'albero.
  
  L'idea è la seguente
  -Ad ogni passo o avremmo risolto il problema o lo avremmo spostato più in alto nell'albero
  -Procediamo in modo diverso a seconda del caso in cui ci troviamo, finchè o risolviamo il problema o risaliamo l'albero fino in cima.

  Ad ogni passo, dato z, ci troveremo in uno dei 6 casi possibili:
  
  Caso 1: guarda sia su wikipedia che sulla pagina dei rbt


*/

void rbtFixup(struct RBT *t, struct rb_node *z) {
while(z->parent->color == 1) { /*  continuiamo finchè il padre di z sarà rosso, e quindi avrò rosso figlio di rosso.
                                   si noti che quando arriveremo alla radice dell' albero, il parent della foglia sarà del tipo
                                   t->leaf, ossia nero, dunque anche in quel caso usciremo dal while e ricoloreremo la radice di nero
                               */
  if(z->parent == z->parent->parent->left) { //CASO 1: il padre di z è figlio sx del nonno di z

    struct rb_node *y = z->parent->parent->right; //introduco y, lo zio di z

    if(y->color == 1) { //caso 1.1: lo zio è rosso
      z->parent->color = 0;
      y->color = 0;
      z->parent->parent->color = 1;
      z = z->parent->parent;
    }
    else { //caso 1.2: lo zio è nero
      //agisco diversamente a seconda che z sia figlio dx o sx 
      if(z == z->parent->right) { 
        z = z->parent; 
        rb_left_rotate(t, z);
      }
      
      z->parent->color = 0; 
      z->parent->parent->color = 1; 
      rb_right_rotate(t, z->parent->parent);
    }
  }


  else {   //CASO 2: il padre di z è figlio dx del nonno di z, simmetrico al precedente
    struct rb_node *y = z->parent->parent->left; 

    if(y->color == 1) {
      z->parent->color = 0;
      y->color = 0;
      z->parent->parent->color = 1;
      z = z->parent->parent;
    }
    else {
      if(z == z->parent->left) {
        z = z->parent; 
        rb_right_rotate(t, z);
      }
      z->parent->color = 0; 
      z->parent->parent->color = 1; 
      rb_left_rotate(t, z->parent->parent);
    }
  }
}
t->root->color = 0;
}
//....................................................................................................................................................................


void rb_insert(struct RBT *t, struct rb_node *z, struct rb_node *p, struct rb_node *trav) {  
   if (trav == t->leaf){  //quando arrivo ad una foglia, inserisco z, inizializzando i puntatori da e verso z
      z->parent = p;
      z->right = t->leaf;
      z->left = t->leaf;
      if(p==t->leaf){  //allora il nuovo nodo sarà il primo nodo nell' albero
         t->root=z;
      }//altrimenti z sarà il nuovo figlio di p, devo però scoprire se figlio dx o sx
      else if(z->key < p->key){ 
         p->left = z;
      }else{
         p->right = z;
      }
     rbtFixup(t, z);
      
   }


  else if (z->key < trav->key)
    rb_insert(t,z,trav,trav->left);
  else
    rb_insert(t,z,trav,trav->right);

}
//...................................................................................................................................................................
//PROCEDURA PER LA STAMPA
void rb_show(struct RBT *t, struct rb_node *trav)
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
      rb_show(t, trav->left);
      rb_show(t, trav->right);
    }
}

//....................................................................................................................................................................
void rb_destroyTree(struct RBT *t, struct rb_node *trav){
    if(trav == t->leaf){
        return;;
    }else{
    rb_destroyTree(t, trav->right);
    rb_destroyTree(t, trav->left);
    free(trav);
    }
}


//....................................................................................................................................................................
//PROCEDURA DI RICERCA
struct rb_node* rb_find(struct RBT *t, struct rb_node *trav, int k)
{
    if(trav==t->leaf || trav->key==k) 
        return trav;
    else if(k>trav->key) 
        return rb_find(t, trav->right, k);
    else 
        return rb_find(t, trav->left, k);
}
//......................................................................................................................................
/*
int main ()
{

  char op[20];  //stringa contenente l'operazione che voglio eseguire: insert find delete
  
     //-memorizzo le stringhe dei nodi in un array v.
     //-per come C( dimensionato possiamo al massimo inserire 100 nodi(dato che puC2 contenere al max 100 stringhe)
     //e ogni nodo puC2 contenere un stringa di al massimo 100 caratteri
   
  char v[100][100];
  int i = 0;
  struct RBT *t = new_red_black_tree();
  int k;
  
  //printf ("operazione da eseguire: \n");
  scanf("%s", op);  //ricavo l'operazione che voglio eseguire da stdin
 



  while (strcmp (op, "exit") != 0)
    {
      
      //inserimento
      if(strcmp(op, "insert")==0){
          scanf("%i", &k);
          scanf("%s", v[i]);   //se faccio un insert ricavo la stringa da passare al nodo da stdin
          struct rb_node *n= new_tree_node(k,v[i]);
          rb_insert(t,n,t->leaf,t->root);
	    
	  //ricerca
      }else if(strcmp(op, "find")==0){
           scanf("%i", &k);
           printf("%s \n",rb_find(t, t->root,k)->s);  //se voglio eseguire una ricerca cerco il nodo con chiave k e stampo la sua stringa
      }else{
           rb_show(t,t->root);
           printf("\n");
      }
      

      //printf ("operazione da eseguire: \n");
      scanf ("%s", op);
      i++;
    }



  rb_destroyTree(t, t->root); //deallocazione dell'albero
  free(t->leaf);
  free(t);
  return 0;
}

*/