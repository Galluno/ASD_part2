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
int main()
{

    double B = exp((log(1000000) - log(A)) / 99); //1 mln = numero massimo operazioni sull' albero = Nmaxn   1000000
    double n;                                     //numero di operazioni sull'albero
    int measurePrec = 100;                          //indice di precisione della misura ~ 100

    double tn = 0; //TEMPO MEDIO MISURATO PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    double *destroyTime; //ARRAY CONTENENTE I TEMPI MEDI MISURATI PER ELIMINARE UN ALBERO, AL VARIARE DI n
    //destroyTime = TreeDestructionTime(tipo di albero);                   //destroyTime[j] = TEMPO MEDIO MISURATO PER ELIMINARE UN ALBERO CREATO CON n=floor(A*B^j) OPERAZIONI

    //Menu
    int scelta;
    printf("Scegli il tipo di albero:\n");
    printf("1-BST\n");
    printf("2-AVL\n");
    printf("3-RBT\n");
    printf("Albero: ");
    scanf("%d", &scelta);

    for (int j = 0; j <= 99; j++)
    {
        n = A * pow(B, j);          //DETERMINO IL NUMERO DI OPERAZIONI DA ESEGURE IN UN ITERAZIONE DEL FOR PIU ESTERNO
        double time_sum = 0;        //somma dei tempi medi misurati per eseguire n operazioni
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
            //STIMIAMO IL TEMPO PER ELIMINARE UN ALBERO COSTRUITO CON n OPERAZIONI(salviamo le misure stimate al variare di n in un array di 100 elemeni, destroyTime[])
            clock_gettime(CLOCK_MONOTONIC, &start);

            do
            {
                struct node *bst_root = NULL;
                struct avl_node *avl_root = NULL;
                struct RBT *t = NULL; 
                
                switch (scelta)
                {

                  case 1:

                    //INIZIALIZZO ROOT DEL MIO ALBERO
                    

                    for (int i = 0; i < n; i++)
                    {
                        //printf("lol\n");
                        //GENERO LA CHIAVE K IN MANIERA PSEUDO-CASUALE
                        int k = rand();
                        //CERCO K NELL'ALBERO
                        if (find(bst_root, k) == NULL)
                        {
                            bst_root = insert(bst_root, k, ""); //SE NON LA TROVO LA INSERISCO
                        }
                    }

                    clock_gettime(CLOCK_MONOTONIC, &end);
                    //DESTROY TREE: DEVO ELIMINARE L'ALBERO CHE HO CREATO
                    destroyTree(bst_root);

                    break; //end case1

                  case 2:
                    //INIZIALIZZO ROOT DEL MIO ALBERO
                    

                    for (int i = 0; i < n; i++)
                    {
                        
                        //GENERO LA CHIAVE K IN MANIERA PSEUDO-CASUALE
                        int k = rand();
                        //CERCO K NELL'ALBERO
                        if (avl_find(avl_root, k) == NULL)
                        {
                            avl_root = avl_insert(avl_root, k, ""); //SE NON LA TROVO LA INSERISCO
                        }
                    }

                    clock_gettime(CLOCK_MONOTONIC, &end);
                    //DESTROY TREE: DEVO ELIMINARE L'ALBERO CHE HO CREATO
                    avl_destroyTree(avl_root);

                    break; //end case2

                  case 3:

                    
                    t = new_red_black_tree();
                    for (int i = 0; i < n; i++)
                    {
                        //printf("lol\n");
                        //GENERO LA CHIAVE K IN MANIERA PSEUDO-CASUALE
                        int k = rand();
                        //CERCO K NELL'ALBERO
                        if (rb_find(t, t->root,k) == NULL)
                        {
                            struct rb_node *n = new_tree_node(k, "");
                            rb_insert(t, n);
                        }
                    }
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    rb_destroyTree(t, t->root); //deallocazione dell'albero
                    free(t->leaf);
                    free(t);

                    break; //end case3
                  


                   default:
                        printf("Valore non valido.\n");
                        break;
                    

                 
                }
                
                k++;
               
                tempo = ((end.tv_sec - start.tv_sec) *(double)BILLION)  + (end.tv_nsec - start.tv_nsec); //TEMPO IN NANO-SECONDI
            } while (tempo < ((R / Emax) + R));

            vector[w] = ((tempo / k)- destroyTime[j]) / n; //TEMPO MEDIO E AMMORTIZZATO PER L'ESECUZIONE DI n OPERAZIONI
            
        }

        //CALCOLO TEMPO MEDIO E AMMORTIZZAT0 PER L'ESECUZIONE DI n OPERAZIONI, tn
        for (int z = 0; z < measurePrec; z++)
        {
            time_sum += vector[z];
        }
        tn = (time_sum / measurePrec); //MEDIA DEI TEMPI MEDI E AMMORTIZZATI PER L'ESECUZIONE DI n OPERAZIONI(VALORE CHE RESTITUIAMO)

        //CALCOLO DEVIAZIONE STANDARD TEMPO MEDIO E AMMORTIZZATO PER ESEGUIRE n OPERAZIONI
        double sommatoria = 0.00; //inizio del calcolo della deviazione
        
        for (int i = 0; i < measurePrec; i++)
        {
            sommatoria = sommatoria + pow((vector[i] - tn), 2);
        }
        D = sqrt(sommatoria / measurePrec); //DEVIAZIONE STANDARD

        printf("%i   %lf   %lf\n", (int)floor(n), tn, D); //stampa a video la combinata della lunghezza della stringa e del tempo impiegato a processarla
    }

    return 0;
}