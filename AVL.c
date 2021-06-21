#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 PREMESSA:
  balanced factor=differenza fra l'alatezza del figlio dx e l'altezza di quello sx

*/
struct avl_node
{
  int key;
  char *s;
  int height;
  struct avl_node *left, *right;
};


//................................................................................................................................
//CREAZIONE DI UN NODO
struct avl_node *newNode (int data, char *v)
{
  struct avl_node *n = (struct avl_node *) malloc (sizeof (struct avl_node));
  n->s = v;			//s punta alla stessa locazione di v, ossia al primo carattere della stringa
  n->key = data;
  n->height = 1;
  n->left = n->right = NULL;
  return n;
}

//................................................................................................................................
//PROCEDURA PER LA STAMPA
void avl_show(struct avl_node *root)
{
  if (root == NULL)
    { 
      printf ("NULL ");
    }
  else
    {
      printf ("%d:", root->key);	//stampa della chiave e della stringa del nodo, es: 5 lorenzo

      for (int i = 0; root->s[i] != '\0'; i++)
	     {
	         printf ("%c", root->s[i]);
	     }
      printf (":%d ", root->height);
      
      
      //chiamata di stampa sul figlio sx e dx
      avl_show(root->left);
      avl_show(root->right);
    }
}
//......................................................................................................................................
//PROCEDURE DI ROTAZIONE PER RIPRISTINARE LA PROPRIETA' DI ESSERE UN AVL
// procedura di supporto, calcolo dell' altezza
int height(struct avl_node *n) {
  if (n == NULL)
    return 0;
  return n->height;
}
//procedura di supporto: massimo
int max(int a, int b){
    if(a>b)
       return a;
    else
       return b;
}
//rotazione a sx
struct avl_node *avl_leftRotate(struct avl_node *x){
    struct avl_node *y= x->right;
    struct avl_node *B= y->left;
    y->left =x;
    x->right=B;
    //sistemo le altezze
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

//rotazione a dx:
struct avl_node *avl_rightRotate(struct avl_node *y){
    struct avl_node *x= y->left;
    struct avl_node *B= x->right;
    y->left =B;
    x->right=y;
    //sistemo le altezze
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

//................................................................................................................................
//PROCEDURA PER L' INSERIMENTO

int balanceFactor(struct avl_node *n){  //fattore di bilanciamento di un nodo
    if(n==NULL)
       return 0;
    else
       return height(n->left) - height(n->right);
}


struct avl_node *avl_insert(struct avl_node *node, int k, char *v)
{
  //se l'albero è vuoto creo e aggiungo il nodo con chiave k e stringa v
  if (node == NULL)
    return newNode (k, v);

  //altrimenti scendo a dx o sx del nodo in cui mi trovo, a seconda del risulatato del confronto tra k e node.key
  if (k < node->key){
    node->left = avl_insert (node->left, k, v);
  }else if (k > node->key){
    node->right = avl_insert (node->right, k, v);
  }else{ 
    return node;
  }
  //aggiornamento dei valori di bilanciamente dei nodi
  node->height = 1 + max(height(node->left),height(node->right));
  int balanceF=balanceFactor(node);
  if (balanceF > 1 && k < node->left->key){
    return avl_rightRotate(node);
  }
  if (balanceF < -1 && k > node->right->key){
    return avl_leftRotate(node);
  }

  if (balanceF > 1 && k > node->left->key) {
    node->left = avl_leftRotate(node->left);
    return avl_rightRotate(node);
  }

  if (balanceF < -1 && k < node->right->key) {
    node->right = avl_rightRotate(node->right);
    return avl_leftRotate(node);
  }


  return node;
    
}

//.................................................................................................................................

void avl_destroyTree(struct avl_node *root){
    if(root == NULL){
        return;
    }else{
    avl_destroyTree(root->right);
    avl_destroyTree(root->left);
    free(root);
    }
}

//................................................................................................................................
//PROCEDURA DI RICERCA
struct avl_node* avl_find(struct avl_node *root, int k)
{
    if(root==NULL || root->key==k) 
        return root;
    else if(k>root->key)
        return avl_find(root->right, k);
    else 
        return avl_find(root->left, k);
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

  int k;
  struct avl_node *root = NULL;
  //printf ("operazione da eseguire: \n");
  scanf("%s", op);  //ricavo l'operazione che voglio eseguire da stdin
 


      
  while (strcmp (op, "exit") != 0)
    {
      
      //inserimento
      if(strcmp(op, "insert")==0){
          scanf("%i", &k);
          scanf("%s", v[i]);   //se faccio un insert ricavo la stringa da passare al nodo da stdin
          root = avl_insert(root, k, v[i]);
	      
	  //ricerca
      }else if(strcmp(op, "find")==0){
           scanf("%i", &k);
           printf("%s \n",avl_find(root,k)->s);  //se voglio eseguire una ricerca cerco il nodo con chiave k e stampo la sua stringa
      }else{
           avl_show(root);
           printf("\n");
      }
      

      //printf ("operazione da eseguire: \n");
      scanf ("%s", op);
      i++;
    }

     
   
  avl_destroyTree(root); //deallocazione dell'albero
  return 0;
}
*/








