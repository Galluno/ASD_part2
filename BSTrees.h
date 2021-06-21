#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
  int key;
  char *s;
  struct node *left, *right;
};

//................................................................................................................................
//CREAZIONE DI UN NODO
struct node *newNode (int data, char *v)
{
  struct node *n = (struct node *) malloc (sizeof (struct node));
  n->s = v;			//s punta alla stessa locazione di v, ossia al primo carattere della stringa
  n->key = data;
  n->left = n->right = NULL;
  return n;
}

//................................................................................................................................
//PROCEDURA PER LA STAMPA
void show(struct node *root)
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
      printf (" ");
      
      
      //chiamata di stampa sul figlio sx e dx
      show(root->left);
      show(root->right);
    }
}

//................................................................................................................................
//PROCEDURA PER L' INSERIMENTO
struct node *insert (struct node *node, int key, char *v)
{
   
  if (node == NULL){
    return newNode (key, v);
  }
  if (key < node->key){
    node->left = insert (node->left, key, v);
  }
  else if (key > node->key){
    node->right = insert (node->right, key, v);
  }


  return node;
}

//.................................................................................................................................
//PROCEDURA PER L'ELIMINAZIONE DELL' ALBERO E LIBERAZIONE SPAZIO DI MEMORIA OCCUPATA

void destroyTree(struct node *root){
    if(root == NULL){
        return;
    }else{
    destroyTree(root->right);
    destroyTree(root->left);
    free(root);
    }
}

//................................................................................................................................
//PROCEDURA DI RICERCA
struct node* find(struct node *root, int k)
{
    if(root==NULL || root->key==k){ 
        return root;
    }
    else if(k>root->key){ 
        return find(root->right, k);
    }
    else{ 
        return find(root->left, k);
    }
}
//......................................................................................................................................
/*
  Per inserimento: insert 5 lol
  Per terminazione: exit 5 lol




int main ()
{

  char op[20];  //stringa contenente l'operazione che voglio eseguire: insert find delete
  
   //  -memorizzo le stringhe dei nodi in un array v.
   //  -per come C( dimensionato possiamo al massimo inserire 100 nodi(dato che puC2 contenere al max 100 stringhe)
   //  e ogni nodo puC2 contenere un stringa di al massimo 100 caratteri
   
  char v[100][100];
  int i = 0;

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
          root=insert(root, k, v[i]);
	      
	  //ricerca
      }else if(strcmp(op, "find")==0){
           scanf("%i", &k);
           printf("%s \n",find(root,k)->s);  //se voglio eseguire una ricerca cerco il nodo con chiave k e stampo la sua stringa
      }else{
           show(root);
           printf("\n");
      }
      

      //printf ("operazione da eseguire: \n");
      scanf ("%s", op);
      i++;
    }



  destroyTree(root); //deallocazione dell'albero
  return 0;
}

*/

