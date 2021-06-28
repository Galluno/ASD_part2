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
    struct avl_node *left;
    struct avl_node *right;
    struct avl_node *parent;
    int height;
};

struct avl_node *new_avl_node(int data, char *v)
{
    struct avl_node *n = (struct avl_node *)malloc(sizeof(struct avl_node));
    n->key = data;
    n->s = v;
    n->left = n->right = n->parent = NULL;
    n->height = 1;

    return n;
}

//......................................................................................................
int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int height(struct avl_node *n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

struct avl_node *avl_left_rotate(struct avl_node *root, struct avl_node *a)
{
    struct avl_node *c = a->right;
    //c diventerà il nuovo a:
    c->parent = a->parent;
    //se a era la radice, ora c è la radice dell'albero
    if (a->parent == NULL)
    {
        root = c;
    }
    //altrimenti, c diventerà figlio del parent di a
    else if (a == a->parent->left)
    {
        a->parent->left = c;
    }
    else
    {
        a->parent->right = c;
    }

    //a diventerà il figlio sx di c:
    a->right = c->left;
    if (c->left != NULL)
    { //il padre del figlio sx di c diventerà a,
        c->left->parent = a;
    }
    c->left = a;
    a->parent = c;

    a->height = 1 + max(height(a->left), height(a->right));
    c->height = 1 + max(height(c->left), height(c->right));

    return root;
}

struct avl_node *avl_right_rotate(struct avl_node *root, struct avl_node *a)
{
    struct avl_node *b = a->left;
    //b diventerà il nuovo a:
    b->parent = a->parent;
    //se a era la radice, ora b è la radice dell'albero
    if (a->parent == NULL)
    {
        root = b;
    }
    //altrimenti, b diventerà figlio del parent di a
    else if (a == a->parent->right)
    {
        a->parent->right = b;
    }
    else
    {
        a->parent->left = b;
    }

    //a diventerà il figlio dx di b:
    a->left = b->right;
    if (b->right != NULL)
    { //il padre del figlio dx di b diventerà a,
        b->right->parent = a;
    }
    b->right = a;
    a->parent = b;

    a->height = 1 + max(height(a->left), height(a->right));
    b->height = 1 + max(height(b->left), height(b->right));

    return root;
}

//.......................................................................................
int balance_factor(struct avl_node *n)
{
    if (n == NULL)
        return 0;
    return (height(n->left) - height(n->right));
}

struct avl_node *avl_insert(struct avl_node *root, struct avl_node *n, struct avl_node *p, struct avl_node *trav)
{

    if (trav == NULL)
    { //quando arrivo ad una foglia, inserisco z, inizializzando i puntatori da e verso z
        n->parent = p;
        n->right = NULL;
        n->left = NULL;
        if (p == NULL)
        { //allora il nuovo nodo sarà il primo nodo nell' albero
            root = n;
        } //altrimenti z sarà il nuovo figlio di p, devo però scoprire se figlio dx o sx
        else if (n->key < p->key)
        {
            p->left = n;
        }
        else
        {
            p->right = n;
        }

        struct avl_node *z = n;
        while (p != NULL)
        {
            p->height = 1 + max(height(p->left), height(p->right));

            struct avl_node *x = p->parent;

            if (balance_factor(x) <-1 || balance_factor(x) > 1)
            { 
                if (p == x->left)
                {
                    if (z == x->left->left) //case 1
                        root = avl_right_rotate(root, x);

                    else if (z == x->left->right)
                    { //case 3
                        root = avl_left_rotate(root, p);
                        root = avl_right_rotate(root, x);
                    }
                }
                else if (p == x->right)
                {
                    if (z == x->right->right) //case 2
                        root = avl_left_rotate(root, x);

                    else if (z == x->right->left)
                    { //case 4
                        root = avl_right_rotate(root, p);
                        root = avl_left_rotate(root, x);
                    }
                }
                break;
            }
            p = p->parent;
            z = z->parent;
        }

        return root;
    }

    else if (n->key < trav->key)
        avl_insert(root, n, trav, trav->left);
    else
        avl_insert(root, n, trav, trav->right);
}

//...................................................................................................................
struct avl_node *avl_find(struct avl_node *trav, int k)
{
    if (trav == NULL || trav->key == k)
        return trav;
    else if (k > trav->key)
        return avl_find(trav->right, k);
    else
        return avl_find(trav->left, k);
}

//...................................................................................................................................................................

//PROCEDURA PER LA STAMPA

void avl_show(struct avl_node *trav)

{

    if (trav == NULL)

    {

        printf("NULL ");
    }

    else

    {

        printf("%d:", trav->key); //stampa della chiave e della stringa del nodo, es: 5 lorenzo

        for (int i = 0; trav->s[i] != '\0'; i++)

	     {

	         printf ("%c", trav->s[i]);

	     }

      printf (":%d ", trav->height);

        //chiamata di stampa sul figlio sx e dx

        avl_show(trav->left);

        avl_show(trav->right);
    }
}
//..................................................................................................
//DEALLOCAZIONE DELL' ALBERO

void avl_destroyTree(struct avl_node *root ){
    if(root == NULL){
        return;
    }else{
    avl_destroyTree(root->right);
    avl_destroyTree(root->left);
    free(root);
    }
}



int main()
{

    char op[20]; //stringa contenente l'operazione che voglio eseguire: insert find delete

    //-memorizzo le stringhe dei nodi in un array v.
    //-per come C( dimensionato possiamo al massimo inserire 100 nodi(dato che puC2 contenere al max 100 stringhe)
    //e ogni nodo puC2 contenere un stringa di al massimo 100 caratteri

    char v[100][100];
    int i = 0;

    int k;
    struct avl_node *root = NULL;
    //printf ("operazione da eseguire: \n");
    scanf("%s", op); //ricavo l'operazione che voglio eseguire da stdin

    while (strcmp(op, "exit") != 0)
    {

        //inserimento
        if (strcmp(op, "insert") == 0)
        {
            scanf("%i", &k);
            scanf("%s", v[i]); //se faccio un insert ricavo la stringa da passare al nodo da stdin
            struct avl_node *n = new_avl_node(k, v[i]);
            root = avl_insert(root, n, NULL, root);

            //ricerca
        }
        else if (strcmp(op, "find") == 0)
        {
            scanf("%i", &k);

            printf("%s \n", avl_find(root, k)->s); //se voglio eseguire una ricerca cerco il nodo con chiave k e stampo la sua stringa
        }
        else
        {
            avl_show(root);
            printf("\n");
        }

        //printf ("operazione da eseguire: \n");
        scanf("%s", op);
        i++;
    }

    avl_destroyTree(root); //deallocazione dell'albero
    return 0;
}