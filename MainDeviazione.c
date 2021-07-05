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

void plot(int treeType, double *y, int measurePrec, double mean, double deviation) //1-bst, 2-avl, 3-rbt
{
    char strTitle[16];

    switch (treeType)
    {
    case 1:
        strcat(strTitle, "set title \"BST\"");
        break;
    case 2:
        strcat(strTitle, "set title \"AVL\"");
        break;
    case 3:
        strcat(strTitle, "set title \"RBT\"");
        break;
    }

    char *commands[] = {
        strTitle,
        "set xlabel \"# iterazione\"",
        "set ylabel \"Tempo medio e ammortizzato\"",
        "set autoscale",
        "plot 'data' with points pointtype 7, 'mean' with lines, '+σ' with lines, '-σ' with lines"};

    FILE *temp = fopen("data", "w");
    FILE *temp1 = fopen("mean", "w");
    FILE *temp2 = fopen("+σ", "w");
    FILE *temp3 = fopen("-σ", "w");

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w"); //-----------------------------------------------

    double x[measurePrec];

    for (int i = 0; i < measurePrec; i++)
    {
        x[i] = i;
    }

    for (int i = 0; i < measurePrec; i++)
    {
        fprintf(temp, "%lf %lf \n", x[i], y[i]);
    }
    for (int i = 0; i < measurePrec; i++)
    {
        fprintf(temp1, "%lf %lf \n", x[i], mean);
    }
    for (int i = 0; i < measurePrec; i++)
    {
        fprintf(temp2, "%lf %lf \n", x[i], mean + deviation);
    }
    for (int i = 0; i < measurePrec; i++)
    {
        fprintf(temp3, "%lf %lf \n", x[i], mean - deviation);
    }

    for (int i = 0; i < 5; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commands[i]); //Send commands to gnuplot one by one.
    }
}

//TEMPI PER UN BST:
void bst()
{

    double n = 10000;      //numero di operazioni sull'albero
    int measurePrec = 1000; //indice di precisione della misura ~ 100

    double tn = 0; //MEDIA DEI TEMPI MEDI E AMMORTIZZATI MISURATI PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    double time_sum = 0;        //somma dei tempi medi eammortizzati misurati per eseguire n operazioni
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

        struct node *bstRootsVector[100]; ////array di supporto sovradimensionato per memoriazzare i puntatori alla radici degli alberi che creo nel do-while, in modo da deallocarli successivamente
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
        printf("%i   %lf\n", (int)floor(n), vector[w]);
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

    printf("La media è: %lf\n", tn); //Stampa a video della media dei tempi medi e ammortizzati, valore che restituimao in TimeMeasurement2.c per n=10000

    printf("La deviazione è: %lf\n", D); //Stampa a video la deviazione standard

    plot(1, vector, measurePrec, tn, D);
}

//...............................................................................................................................................................................

//TEMPI PER UN BST:
void avlTree()
{

    double n = 10000;      //numero di operazioni sull'albero
    int measurePrec = 1000; //indice di precisione della misura ~ 100

    double tn = 0; //MEDIA DEI TEMPI MEDI E AMMORTIZZATI MISURATI PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    double time_sum = 0;        //somma dei tempi medi eammortizzati misurati per eseguire n operazioni
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

        struct avl_node *avlRootsVector[100]; //array di supporto sovradimensionato per memoriazzare i puntatori alla radici degli alberi che creo nel do-while, in modo da deallocarli successivamente
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

        vector[w] = (tempo / k) / n;                    //TEMPO MEDIO E AMMORTIZZATO PER L'ESECUZIONE DI n OPERAZIONI(senza il tempo di deallocazione dell'albero creato)
        printf("%i   %lf\n", (int)floor(n), vector[w]); //stampa a video del tempo medio e ammortizzato
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

    printf("La media è: %lf\n", tn); //Stampa a video della media dei tempi medi e ammortizzati, valore che restituimao in TimeMeasurement2.c per n=10000

    printf("La deviazione è: %lf\n", D); //Stampa a video la deviazione standar

    plot(2, vector, measurePrec, tn, D);
}

//...............................................................................................................................................................................

//TEMPI PER UN RBT:
void rbt()
{

    double n = 10000;      //numero di operazioni sull'albero
    int measurePrec = 1000; //indice di precisione della misura ~ 100

    double tn = 0; //MEDIA DEI TEMPI MEDI E AMMORTIZZATI MISURATI PER ESEGUIRE n OPERAZIONI SULL ALBERO
    double D = 0;  //DEVIAZIONE STANDARD

    double time_sum = 0;        //somma dei tempi medi eammortizzati misurati per eseguire n operazioni
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

        struct RBT *rbtsVector[100]; //array di supporto sovradimensionato per memoriazzare i puntatori alla radici degli alberi che creo nel do-while, in modo da deallocarli successivamente
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
                if (rb_find(rbtsVector[k], rbtsVector[k]->root, key) == rbtsVector[k]->leaf)
                {
                    struct rb_node *n = new_tree_node(key, "");
                    rb_insert(rbtsVector[k], n, rbtsVector[k]->leaf, rbtsVector[k]->root);
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

        vector[w] = (tempo / k) / n;                    //TEMPO MEDIO E AMMORTIZZATO PER L'ESECUZIONE DI n OPERAZIONI(senza il tempo di deallocazione dell'albero creato)
        printf("%i   %lf\n", (int)floor(n), vector[w]); //stampa a video del tempo medio e ammortizzato
    }

    //CALCOLO TEMPO MEDIO E AMMORTIZZAT0 PER L'ESECUZIONE DI n OPERAZIONI, tn
    for (int z = 0; z < measurePrec; z++)
    {
        time_sum += vector[z];
    }
    tn = (time_sum / measurePrec); //media dei tempi medi e ammortizzati per l'esecuzione di n operazioni(valore che restituiamo in TimeMeasurement2.c)

    //CALCOLO DEVIAZIONE STANDARD TEMPO MEDIO E AMMORTIZZATO PER ESEGUIRE n OPERAZIONI
    double sommatoria = 0.00; //inizio del calcolo della deviazione

    for (int i = 0; i < measurePrec; i++)
    {
        sommatoria = sommatoria + pow((vector[i] - tn), 2);
    }
    D = sqrt(sommatoria / measurePrec); //DEVIAZIONE STANDARD

    printf("La media è: %lf\n", tn); //Stampa a video della media dei tempi medi e ammortizzati, valore che restituimao in TimeMeasurement2.c per n=10000

    printf("La deviazione è: %lf\n", D); //Stampa a video la deviazione standard

    plot(3, vector, measurePrec, tn, D);
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