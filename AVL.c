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

//..............................................................................................................................................................................................................
/*ROTAZIONI E PROCEDURE DI SUPPORTO PER RIPRISTINARE LA PROPRIETA' DEGLI AVL CHE MI GARANTISCE IL BILANCIAMENTOù

  Nota: le rotazioni posso modificare la radice dell' albero, il puntatore alla quale è fondamentale per poter inserire correttamente il nodo successivo, cercare un nodo e deallocare l'albero,
  dunque le procedure che effetuano una rotazione a dx o sx, ogni volta mi restituiscono il puntatore alla radice dell' albero, che portrà essere la stesso che gli passiamo in input se questa non viene 
  modificata, o uno nuovo, nel caso in cui il nodo che funge da perno nella rotazione sia proprio la radice.

*/

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

    a->height = max(height(a->left), height(a->right)) + 1;
    b->height = max(height(b->left), height(b->right)) + 1;

    return root;
}

//..................................................................................................................................................................................................
int balance_factor(struct avl_node *n)
{
    if (n == NULL)
        return 0;
    return (height(n->left) - height(n->right));
}

struct avl_node *avl_insert(struct avl_node *root, struct avl_node *n, struct avl_node *p, struct avl_node *trav)
{

    if (trav == NULL)
    { //quando arrivo ad una foglia, inserisco n, inizializzando i puntatori da e verso n
        n->parent = p;
        n->right = NULL;
        n->left = NULL;
        if (p == NULL)
        { //allora il nuovo nodo sarà il primo nodo nell' albero
            root = n;
        } //altrimenti n sarà il nuovo figlio di p, devo però scoprire se figlio dx o sx
        else if (n->key < p->key)
        {
            p->left = n;
        }
        else
        {
            p->right = n;
        }

        /*RIPRISTINO DELLA PROPRIETA' CHE GARANTISCE IL BILANCIAMENTO:
        Dopo aver inserito normalmente n, come in un bst, ci occupiamo di ripristinare la proprietà che garantisce 
        il bilanciamento dell' albero.
        Dopo l'inserimento di un nuovo nodo, sono 2 le cose da fare:
        -risalire l'albero per aggiornare le altezze degli antenati del nodo aggiunto
        -verificare se l'inserimento del nuovo nodo ha sbilanciato l'albero, controllando tra gli antenati, se il fattore di sbilanciamento dei nodi è diventato +2 o -2.
         In tal caso, a seconda del tipo di situazione in cui ci troviamo, applichiamo diverse combinazioni di rotazioni per sistemare il problema.
         
        */
        struct avl_node *x = n; //Sia x il nodo su cui sto operando in una generica iterazione del while e in principio, il nuovo nodo appena inserito, n
        while (p != NULL)       //Sia p il parent di x
        {
            p->height = 1 + max(height(p->left), height(p->right)); //Come prima cosa sistemo l'altezza di p

            struct avl_node *grandParent = p->parent; //l'inserimento di n non causerà uno sbilanciamento del genitore di n, bensi del nonno, grandParent

            if (balance_factor(grandParent) < -1 || balance_factor(grandParent) > 1)  //verifichiamo se il nonno di x è sbilanciato:
            {
                /* In tal caso, cioè se è sbilanciato, per ribilanciare l'albero abbiamo 4 modi diversi di procerdere, a seconda del caso in cui ci troviamo
                   Caso 1: Dato p=x->parent e grandParent=p->parent, p è figlio sx e n è stato inserito nel sottoalbero sx di p
                           Eseguiamo una rotazione a dx con perno GrandParent 
                   Caso 2, caso simmetrico al precedente: Dato p=x->parent e grandParent=p->parent, p è figlio dx e n è stato inserito nel sottoalbero dx di p
                           In maniera simmetrica eseguimao una rotazione a sx con perno sempre GrandParent
                   Caso 3: Dato p e grandParent=p->parent, p=x->parent è figlio sx e n è stato inserito nel sottoalbero dx di p
                          -eseguiamo una rotazione a sx con perno p
                          -eseguiamo una rotazione a dx con perno GrandPArent
                   Caso 4, caso simmetrico al precedente: Dato p=x->parent e grandParent=p->parent, p è figlio dx e n è stato inserito nel sottoalbero sx di p
                           -eseguiamo una rotazione a dx con perno p
                          -eseguiamo una rotazione a sx con perno GrandPArent
                
                    Successivamente iteriamo verso l'alto con x=p e p=p->parent

                */
                if (p == grandParent->left)
                { 
                    if (x == grandParent->left->left)  //caso 1
                        root = avl_right_rotate(root, grandParent);

                    else if (x == grandParent->left->right)
                    {
                        root = avl_left_rotate(root, p);  //caso 3
                        root = avl_right_rotate(root, grandParent);
                    }
                }
                else if (p == grandParent->right)
                {
                    if (x == grandParent->right->right)  //caso 2
                        root = avl_left_rotate(root, grandParent);

                    else if (x == grandParent->right->left)
                    {
                        root = avl_right_rotate(root, p);  //caso 4
                        root = avl_left_rotate(root, grandParent);
                    }
                }
                break;
            }
            
            //risaliamo l'albero
            p = p->parent;
            x = x->parent;
        }

        return root;
    }

    else if (n->key < trav->key)
        return avl_insert(root, n, trav, trav->left);
    else
        return avl_insert(root, n, trav, trav->right);
}

//................................................................................................................................................................................................................
//RICERCA:
struct avl_node *avl_find(struct avl_node *trav, int k)
{
    if (trav == NULL || trav->key == k)
        return trav;
    else if (k > trav->key)
        return avl_find(trav->right, k);
    else
        return avl_find(trav->left, k);
}

//.............................................................................................................................................................................................................

//PROCEDURA PER LA STAMPA:

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

            printf("%c", trav->s[i]);
        }

        printf(":%d ", trav->height);

        //chiamata di stampa sul figlio sx e dx

        avl_show(trav->left);

        avl_show(trav->right);
    }
}
//...............................................................................................................................................................................................................
//DEALLOCAZIONE DELL' ALBERO

void avl_destroyTree(struct avl_node *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        avl_destroyTree(root->right);
        avl_destroyTree(root->left);
        free(root);
    }
}

//................................................................................................................................................................................................................
/*
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
*/