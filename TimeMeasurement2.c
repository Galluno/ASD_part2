#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "BSTrees.h"
#include "RedBlack.c"
#include "AVL.c"
#define A 1000    //A=numero di operazioni minime sull' albero= Nmin    1000
#define Emax 0.01 //Errore relativo massimo
#define BILLION 1000000000L
//...................................................................................................................................................................
long getResolution()
{
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do
    {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while (start.tv_nsec == end.tv_nsec);
    return (end.tv_nsec - start.tv_nsec);
}
//...............................................................................................................................................................................

//TEMPI PER UN BST:
void bst()
{

    double B = exp((log(1000000) - log(A)) / 99); //1 mln = numero massimo operazioni sull' albero = Nmaxn   1000000
    double n;                                     //numero di operazioni sull'albero
    int measurePrec = 100;                        //indice di precisione della misura ~ 100

    double tn = 0; //MEDIA DEI TEMPI MEDI E AMMORTIZZATI MISURATI PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    for (int j = 0; j <= 99; j++)
    {
        n = A * pow(B, j);          //DETERMINO IL NUMERO DI OPERAZIONI DA ESEGURE IN UN ITERAZIONE DEL FOR PIU ESTERNO
        double time_sum = 0;        //somma dei tempi medi e ammortizzati misurati per eseguire n operazioni
        double vector[measurePrec]; //OGNI ELEMENTO E' LA MISURA DEL TEMPO IMPIEGATO PER ESEGUIRE n OPERAZIONI(ESCLUDIAMO IL TEMPO PER ELIMINARE L'ALBERO)

        for (int w = 0; w < measurePrec; w++)
        {
            /*
                RIPETIAMO measurePrec VOLTE LA MISURA DEL TEMPO MEDIO PER ESEGUIRE n OPERAZIONI DI RICERCA E INSERIMENTO
                POI CALCOLIAMO LA MEDIA. QUESTO CI PERMETTE SIA DI AVERE UNA STIMA PIU PRECISA DEL TEMPO IMPIEGATO PER ESEGUIRE n OPERAZIONI SULL ALBERO
                MA ANCHE DI AVERE A DISPOSIZIONE LA MEDIA PER CALCOLARE LA DEVIAZIONE STANDARD DEL TEMPO MEDIO E AMMORTIZZATO
                */

            struct timespec start, end;

            long R = getResolution(); //RISOLUZIONE IN NANO-SECONDI
            int k = 0;

            double tempo = 0;
            
            struct node *bstRootsVector[100];  //array di supporto sovradimensionato per memoriazzare i puntatori alla radici degli alberi che creo nel do-while, in modo da deallocarli successivamente
            clock_gettime(CLOCK_MONOTONIC, &start);

            do
            {

                //INIZIALIZZO ROOT DEL MIO ALBERO
                bstRootsVector[k] = NULL;
                for (int i = 0; i < n; i++)
                {

                    //GENERO LA CHIAVE K IN MANIERA PSEUDO-CASUALE
                    int key = rand();
                    //CERCO K NELL'ALBERO
                    if (find(bstRootsVector[k], key) == NULL)
                    {
                        bstRootsVector[k] = insert(bstRootsVector[k], key, ""); //SE NON LA TROVO LA INSERISCO
                    }
                }

                clock_gettime(CLOCK_MONOTONIC, &end);

                k++;

                tempo = ((end.tv_sec - start.tv_sec) * (double)BILLION) + (end.tv_nsec - start.tv_nsec); //TEMPO IN NANO-SECONDI
            } while (tempo < ((R / Emax) + R));


            //DEALLOCAZIONE DEGLI ALBERI CREATI NEL doWhile():

            for (int q = 0; q < k; q++)
            { 
                destroyTree(bstRootsVector[q]);
            }

            vector[w] = (tempo / k) / n; //TEMPO MEDIO E AMMORTIZZATO PER L'ESECUZIONE DI n OPERAZIONI(senza il tempo di deallocazione dell'albero creato)
        }

        //CALCOLO TEMPO MEDIO E AMMORTIZZAT0 PER L'ESECUZIONE DI n OPERAZIONI, tn
        for (int z = 0; z < measurePrec; z++)
        {
            time_sum += vector[z];
        }
        tn = (time_sum / measurePrec); //media dei tempi medi e ammortizzati per l'esecuzione di n operazioni

        //CALCOLO DEVIAZIONE STANDARD TEMPO MEDIO E AMMORTIZZATO PER ESEGUIRE n OPERAZIONI
        double sommatoria = 0.00; //inizio del calcolo della deviazione

        for (int i = 0; i < measurePrec; i++)
        {
            sommatoria = sommatoria + pow((vector[i] - tn), 2);
        }
        D = sqrt(sommatoria / measurePrec); //DEVIAZIONE STANDARD

        printf("%i   %lf   %lf\n", (int)floor(n), tn, D); //stampa a video la combinata della lunghezza della stringa e del tempo impiegato a processarla
    }
}

//...............................................................................................................................................................................

//TEMPI PER UN BST:
void avlTree()
{

    double B = exp((log(1000000) - log(A)) / 99); //1 mln = numero massimo operazioni sull' albero = Nmaxn   1000000
    double n;                                     //numero di operazioni sull'albero
    int measurePrec = 100;                        //indice di precisione della misura ~ 100

    double tn = 0; //MEDIA DEI TEMPI MEDI E AMMORTIZZATI MISURATI PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    for (int j = 0; j <= 99; j++)
    {
        n = A * pow(B, j);          //DETERMINO IL NUMERO DI OPERAZIONI DA ESEGURE IN UN ITERAZIONE DEL FOR PIU ESTERNO
        double time_sum = 0;      //somma dei tempi medi e ammortizzati misurati per eseguire n operazioni
        double vector[measurePrec]; //OGNI ELEMENTO E' LA MISURA DEL TEMPO IMPIEGATO PER ESEGUIRE n OPERAZIONI(ESCLUDIAMO IL TEMPO PER ELIMINARE L'ALBERO)

        for (int w = 0; w < measurePrec; w++)
        {
            /*
                RIPETIAMO measurePrec VOLTE LA MISURA DEL TEMPO MEDIO PER ESEGUIRE n OPERAZIONI DI RICERCA E INSERIMENTO
                POI CALCOLIAMO LA MEDIA. QUESTO CI PERMETTE SIA DI AVERE UNA STIMA PIU PRECISA DEL TEMPO IMPIEGATO PER ESEGUIRE n OPERAZIONI SULL ALBERO
                MA ANCHE DI AVERE A DISPOSIZIONE LA MEDIA PER CALCOLARE LA DEVIAZIONE STANDARD DEL TEMPO MEDIO E AMMORTIZZATO
                */

            struct timespec start, end;

            long R = getResolution(); //RISOLUZIONE IN NANO-SECONDI
            int k = 0;

            double tempo = 0;
            
            struct avl_node *avlRootsVector[100];  //array di supporto sovradimensionato per memoriazzare i puntatori alla radici degli alberi che creo nel do-while, in modo da deallocarli successivamente
            clock_gettime(CLOCK_MONOTONIC, &start);

            do
            {

                //INIZIALIZZO ROOT DEL MIO ALBERO
                avlRootsVector[k] = NULL;
                for (int i = 0; i < n; i++)
                {

                    //GENERO LA CHIAVE K IN MANIERA PSEUDO-CASUALE
                    int key = rand();
                    //CERCO K NELL'ALBERO
                    if (avl_find(avlRootsVector[k], key) == NULL)
                    {
                        struct avl_node *n = new_avl_node(k, "");
                        avlRootsVector[k] = avl_insert(avlRootsVector[k], n, NULL, avlRootsVector[k]); //SE NON LA TROVO LA INSERISCO
                    }
                }

                clock_gettime(CLOCK_MONOTONIC, &end);

                k++;

                tempo = ((end.tv_sec - start.tv_sec) * (double)BILLION) + (end.tv_nsec - start.tv_nsec); //TEMPO IN NANO-SECONDI
            } while (tempo < ((R / Emax) + R));


            //DEALLOCAZIONE DEGLI ALBERI CREATI NEL doWhile():

            for (int q = 0; q < k; q++)
            { 
                avl_destroyTree(avlRootsVector[q]);
            }

            vector[w] = (tempo / k) / n; //TEMPO MEDIO E AMMORTIZZATO PER L'ESECUZIONE DI n OPERAZIONI(senza il tempo di deallocazione dell'albero creato)
        }

        //CALCOLO TEMPO MEDIO E AMMORTIZZAT0 PER L'ESECUZIONE DI n OPERAZIONI, tn
        for (int z = 0; z < measurePrec; z++)
        {
            time_sum += vector[z];
        }
        tn = (time_sum / measurePrec); //media dei tempi medi e ammortizzati per l'esecuzione di n operazioni

        //CALCOLO DEVIAZIONE STANDARD TEMPO MEDIO E AMMORTIZZATO PER ESEGUIRE n OPERAZIONI
        double sommatoria = 0.00; //inizio del calcolo della deviazione

        for (int i = 0; i < measurePrec; i++)
        {
            sommatoria = sommatoria + pow((vector[i] - tn), 2);
        }
        D = sqrt(sommatoria / measurePrec); //DEVIAZIONE STANDARD

        printf("%i   %lf   %lf\n", (int)floor(n), tn, D); //stampa a video la combinata della lunghezza della stringa e del tempo impiegato a processarla
    }
}

//...............................................................................................................................................................................

//TEMPI PER UN RBT:
void rbt()
{

    double B = exp((log(1000000) - log(A)) / 99); //1 mln = numero massimo operazioni sull' albero = Nmaxn   1000000
    double n;                                     //numero di operazioni sull'albero
    int measurePrec = 100;                        //indice di precisione della misura ~ 100

    double tn = 0; //MEDIA DEI TEMPI MEDI E AMMORTIZZATI MISURATI PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    for (int j = 0; j <= 99; j++)
    {
        n = A * pow(B, j);          //DETERMINO IL NUMERO DI OPERAZIONI DA ESEGURE IN UN ITERAZIONE DEL FOR PIU ESTERNO
        double time_sum = 0;        //somma dei tempi medi e ammortizzati misurati per eseguire n operazioni
        double vector[measurePrec]; //OGNI ELEMENTO E' LA MISURA DEL TEMPO IMPIEGATO PER ESEGUIRE n OPERAZIONI(ESCLUDIAMO IL TEMPO PER ELIMINARE L'ALBERO)

        for (int w = 0; w < measurePrec; w++)
        {
            /*
                RIPETIAMO measurePrec VOLTE LA MISURA DEL TEMPO MEDIO PER ESEGUIRE n OPERAZIONI DI RICERCA E INSERIMENTO
                POI CALCOLIAMO LA MEDIA. QUESTO CI PERMETTE SIA DI AVERE UNA STIMA PIU PRECISA DEL TEMPO IMPIEGATO PER ESEGUIRE n OPERAZIONI SULL ALBERO
                MA ANCHE DI AVERE A DISPOSIZIONE LA MEDIA PER CALCOLARE LA DEVIAZIONE STANDARD DEL TEMPO MEDIO E AMMORTIZZATO
                */

            struct timespec start, end;

            long R = getResolution(); //RISOLUZIONE IN NANO-SECONDI
            int k = 0;

            double tempo = 0;
         
            struct RBT *rbtsVector[100];  //array di supporto sovradimensionato per memoriazzare i puntatori agli alberi che creo nel do-while, in modo da deallocarli successivamente
            clock_gettime(CLOCK_MONOTONIC, &start);

            do
            {

                //Salvo il puntatore all' albero rb che andrò a costruire nel mio array di supporto, in modo da andarlo poi a deallocare fuori dal do-while
                rbtsVector[k] = new_red_black_tree();
                for (int i = 0; i < n; i++)
                {
                    
                        //GENERO LA CHIAVE K IN MANIERA PSEUDO-CASUALE
                        int key = rand();
                        //CERCO K NELL'ALBERO
                        if (rb_find(rbtsVector[k], rbtsVector[k]->root,key) == rbtsVector[k]->leaf )
                        {
                            struct rb_node *n = new_tree_node(key, "");
                            rb_insert(rbtsVector[k], n,rbtsVector[k]->leaf,rbtsVector[k]->root);
                        }
                }

                clock_gettime(CLOCK_MONOTONIC, &end);

                k++;

                tempo = ((end.tv_sec - start.tv_sec) * (double)BILLION) + (end.tv_nsec - start.tv_nsec); //TEMPO IN NANO-SECONDI
            } while (tempo < ((R / Emax) + R));


            //DEALLOCAZIONE DEGLI ALBERI CREATI NEL doWhile():

            for (int q = 0; q < k; q++)
            { 
                
                    rb_destroyTree(rbtsVector[q], rbtsVector[q]->root); //deallocazione dell'albero
                    free(rbtsVector[q]->leaf);
                    free(rbtsVector[q]);
            }

            vector[w] = (tempo / k) / n; //TEMPO MEDIO E AMMORTIZZATO PER L'ESECUZIONE DI n OPERAZIONI(senza il tempo di deallocazione dell'albero creato)
        }

        //CALCOLO TEMPO MEDIO E AMMORTIZZAT0 PER L'ESECUZIONE DI n OPERAZIONI, tn
        for (int z = 0; z < measurePrec; z++)
        {
            time_sum += vector[z];
        }
        tn = (time_sum / measurePrec); //media dei tempi medi e ammortizzati per l'esecuzione di n operazioni

        //CALCOLO DEVIAZIONE STANDARD TEMPO MEDIO E AMMORTIZZATO PER ESEGUIRE n OPERAZIONI
        double sommatoria = 0.00; //inizio del calcolo della deviazione

        for (int i = 0; i < measurePrec; i++)
        {
            sommatoria = sommatoria + pow((vector[i] - tn), 2);
        }
        D = sqrt(sommatoria / measurePrec); //DEVIAZIONE STANDARD

        printf("%i   %lf   %lf\n", (int)floor(n), tn, D); //stampa a video la combinata della lunghezza della stringa e del tempo impiegato a processarla
    }
}
//..................................................................................................................................................................................................................
int main()
{
    int scelta;
    printf("Scegli il tipo di albero:\n");
    printf("1-BST\n");
    printf("2-AVL\n");
    printf("3-RBT\n");
    printf("Albero: ");
    scanf("%d", &scelta);

    switch (scelta)
    {

    case 1:
        bst();
        break; //end case1

    case 2:
        avlTree();
        break; //end case2

    case 3:
        rbt();
        break; //end case3

    default:
        printf("Valore non valido.\n");
        break;
    }
    return 0;
}