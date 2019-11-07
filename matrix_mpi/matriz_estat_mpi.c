/* Grupo 5
Danilo Henrique Cordeiro         - 6791651
Eduardo Alves Baratela           - 10295270
Giovana Meloni Craveiro          - 9791264
Joao Vitor Nasevicius Ramos      - 9894540
Matheus Aparecido do Carmo Alves - 9791114

PCAM
-> Particionamento: Um processo é responsável para receber os calculos feitos
    e um outro é responsavel para realizar os calculos

-> Comunicacao: A comunicacao e realizada entre cada um dos processos e o processo
    principal insere os dados nos vetores de resposta

-> Aglomeracao: A aglomeracao e feita pelo numero de colunas processadas

-> Mapeamento: Cada processador recebe um processo para ser calculado

Para compilar: mpicc matriz_estat_par.c -o mpi -lm
Para executar: mpirun mpi -np 4 mpi < entrada.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "mpi.h"

enum{
    HI_TAG,
    LINE_TAG,
    COL_TAG,
    MATRIX_TAG,
    MEAN_TAG
};

//Quicksort adaptado de //https://www.geeksforgeeks.org/quick-sort/
int partition (double *arr, int low, int high, int C){
    int i, j;
    double pivot,swap;
    
    // pivot (Element to be placed at right position)
    pivot = arr[high*C];  
 
    i = (low - 1);  // Index of smaller element

    for (j = low; j <= high-1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j*C] <= pivot)
        {
            i++;    // increment index of smaller element
            
            // swap arr[i] and arr[j]
            swap = arr[i*C];
        arr[i*C] = arr[j*C];
        arr[j*C] = swap;
        }
    }
    
    //swap arr[i + 1] and arr[high]
    swap = arr[(i + 1)*C];
    arr[(i + 1)*C] = arr[high*C];
    arr[high*C] = swap;
    
    return (i + 1);
  
} // fim partition


/* low  --> Starting index,  high  --> Ending index */
void quicksort(double *arr, int low, int high, int C){
    int pi;
    
    if (low < high)  {
        /* pi is partitioning index, arr[pi] is now
           at right place */
        pi = partition(arr, low, high, C);

        quicksort(arr, low, pi - 1, C);  // Before pi
        quicksort(arr, pi + 1, high, C); // After pi
    }
    
} // fim quicksort

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot 
   https://www.geeksforgeeks.org/quick-sort/
*/

void ordena_colunas(double *matriz, int lin, int col) {
  int j;
  
  for (j = 0; j < col; j++) {
      //manda o endereco do primeiro elemento da coluna, limites inf e sup e a largura da matriz
      quicksort(&matriz[j], 0, lin-1, col);
  }
} 

void calcula_media(double *matriz, double *vet, int lin, int col){
    int i,j;
    double soma;
    for(i=0;i<col;i++){
        soma=0;
        for(j=0;j<lin;j++){
            soma+=matriz[j*col+i];
        }
        vet[i]=soma/j; 
    }   
}

void calcula_media_harmonica(double *matriz, double *vet, int lin, int col){
    int i,j;
    double soma;
    for(i=0;i<col;i++){
        soma=0;
        for(j=0;j<lin;j++){
            soma+=(1/(matriz[j*col+i]));
        }
        vet[i]=lin/soma; 
    }   
}

void calcula_mediana(double *matriz, double *vet, int lin, int col) {  
  int j;
  for (j = 0; j < col; j++) {
    vet[j] = matriz[((lin/2)*col)+j];
    if(!(lin%2))  {
      vet[j]+=matriz[((((lin-1)/2)*col)+j)];
      vet[j]*=0.5;
    } 
  } 
} 

//Adaptado de https://www.clubedohardware.com.br/forums/topic/1291570-programa-em-c-que-calcula-moda-media-e-mediana/
double moda_aux(double *matriz,int lin){
    int i, j; 
    double *cont;
    cont=(double*)malloc(lin*sizeof(double));
    float conta=0, moda;
    
    for(i=0;i<lin;i++){
        for(j=i+1;j<lin;j++){
            
            if(matriz[i]==matriz[j]){
                cont[i]++;
                    if(cont[i]>conta){
                        conta=cont[i];
                        moda=matriz[i];
                    }
            }

        }
        cont[i]=0;
    }
    free(cont);
    if(conta == 0){
        return -1;
    }
    else{
        return moda;
    }

}


void calcula_moda(double *matriz,double *moda,int lin, int col){
    int i,j;
    double *aux=(double *)malloc(lin*sizeof(double));
    for(i=0;i<col;i++){
        for(j=0;j<lin;j++)
        {
            aux[j]=matriz[j*col+i]; //Faz a transposta da linha para coluna
        }
        moda[i]=moda_aux(aux,lin);
    }
    free(aux);

}

void calcula_variancia(double *matriz, double *media,double *variancia, int lin, int col)
{
    int i,j;
    double soma;
    for(i=0;i<col;i++){
        soma=0;
        for(j=0;j<lin;j++){
            soma+=pow((matriz[j*col+i]-media[i]),2);
        }
        variancia[i]=soma/(lin-1); 
    } 
}

void calcula_desvio_padrao(double *variancia,double *dp, int col)
{
    int i;
    for(i=0;i<col;i++){
        dp[i]=sqrt(variancia[i]);
    }  
}

void calcula_coeficiente_variacao(double *media,double *dp,double *cv, int col)
{
    int i;
    for(i=0;i<col;i++){
        cv[i]=dp[i]/media[i];
    }  
}

void print_dvec(double *vec, int size){
    int i;

    for(i = 0; i < size ; i++){
        printf("%lf ",vec[i]);
    }
    printf("\n");

    return;
}

void print_dmatrix(double *matrix, int lin, int col){
    int i, j;

    for(i = 0; i < lin ; i++){
        for(j = 0 ; j < col ; j++){
            printf("%lf ",matrix[(i*col) + j]);
        }
        printf("\n");
    }
    printf("\n");

    return;
}

void print_result(double *media, double *media_har, double *mediana,
	double *moda, double *variancia, double *dp, double *cv, int col){
	int i = 0;

    for(i = 0; i < col; i++)
        printf("%.1lf ",media[i]);
    printf("\n");

    for(i = 0; i < col; i++)
        printf("%.1lf ",media_har[i]);
    printf("\n");

    for(i = 0;i < col; i++)
       printf("%.1lf ",mediana[i]);
    printf("\n");

    for(i = 0; i < col; i++)
       printf("%.1lf ",moda[i]);
    printf("\n");

    for(i = 0; i < col; i++)
        printf("%.1lf ",variancia[i]);
    printf("\n");

    for(i = 0; i < col; i++)
        printf("%.1lf ",dp[i]);
    printf("\n");

    for(i = 0; i < col; i++)
        printf("%.1lf ",cv[i]);

    return;
}

int main(int argc,char **argv){
    // 0. Definindo as variaveis do problema
    // a. variaveis comuns
    int lin = 0, col = 0, i = 0, j = 0;
    double *matriz = NULL;
    double *mediana = NULL, *media = NULL, *media_har = NULL, *moda = NULL;
    double *variancia = NULL, *dp = NULL, *cv = NULL;

    // b. variaveis mpi
    int count, src, dst, rank, my_rank, n_proc, root;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Status status;

    //printf("Character %d of %d is on stage.\n",my_rank+1,n_proc);
    // 1. Lendo o problema de entrada
    // a. numero de linhas e colunas da matriz de entrada
    fscanf(stdin, "%d %d\n", &lin, &col); 

    // b. alocando memoria para as variaveis definidas
    matriz=(double *)malloc((lin*col) * sizeof(double)); //Aloca a matriz
    media=(double *)malloc(col * sizeof(double)); //Aloca o vetor de media
    media_har=(double *)malloc(col * sizeof(double)); //Aloca o vetor de media harmônica
    mediana=(double *)malloc(col * sizeof(double)); //Aloca o vetor de mediana
    moda=(double *)malloc(col * sizeof(double)); //Aloca o vetor de moda
    variancia=(double *)malloc(col * sizeof(double)); //Aloca o vetor de variância
    cv=(double *)malloc(col * sizeof(double)); //Aloca o vetor de coeficiente de variação
    dp=(double *)malloc(col * sizeof(double)); //Aloca o vetor de desvio padrão

    // c. matriz de entrada
    for(i = 0; i < lin; i++)
        for(j = 0; j < col; j++)
            fscanf(stdin, "%lf ",&(matriz[i*col+j]));

    // 2. Realizando os calculos solicitados
    src = root = 0;
    dst = 1;
    // a. processo mestre
    if(my_rank == root){
        // i. enviando mensagem para iniciar o trabalho
        char mst_msg[50] = "Let's wake up, son! Help your mom!";
        MPI_Send(mst_msg, 50, MPI_CHAR, dst, HI_TAG, MPI_COMM_WORLD);
        //printf("Mommy sent: '%s'\n",mst_msg);

        // ii. aguardando resposta
        char mst_ans[50];
        //printf("Mommy waiting for answer...\n");
        MPI_Recv(mst_ans, 50, MPI_CHAR, dst, HI_TAG, MPI_COMM_WORLD, &status);
        //printf("Mommy heard: '%s'\n",mst_ans);

        // iii. enviando a matriz para ordenacao
        //printf("Mommy is sending a matrix to sort...\n");
        MPI_Send(&lin, 1, MPI_INT, dst, LINE_TAG, MPI_COMM_WORLD);
        MPI_Send(&col, 1, MPI_INT, dst, COL_TAG, MPI_COMM_WORLD);
        MPI_Ssend(matriz, (lin*col), MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD);

        /// iv. aguardando a matriz ordenada
        //printf("Mommy is waiting for the sorted matrix...\n");
        MPI_Recv(matriz, (lin*col), MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD, &status);
        //printf("Mommy is received (she seems happy):\n");

        // v. realizando o calculo das medianas e moda
        //printf("Happy mommy helps her son to finish the tasks...\n");
        calcula_mediana(matriz,mediana,lin,col);
        //printf("| Mommy calculated the median. \n| | ");
        //print_dvec(mediana,col); 

        calcula_moda(matriz,moda,lin,col);
        //printf("| Mommy calculated the fashion. \n| | ");
        //print_dvec(moda,col);

        //printf("Now, Mommy is resting and waiting for her son to finish his obligations...\n");
        MPI_Recv(media, col, MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(media_har, col, MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(variancia, col, MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(dp, col, MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(cv, col, MPI_DOUBLE, dst, MATRIX_TAG, MPI_COMM_WORLD, &status);

        // iii. imprimindo os resultados obtidos
        //printf("Oh, he finished it fast. :)\n=== FINAL RESULT ===\n");
        print_result(media,media_har,mediana,moda,variancia,dp,cv,col);
    }
    // b. processos escravos
    else{
        // i. aguardando ordem do mestre
        char slv_msg[50];
        //printf("Son %d is sleeping...\n", my_rank);
        MPI_Recv(slv_msg, 50, MPI_CHAR, src, HI_TAG, MPI_COMM_WORLD, &status);
        //printf("Son %d wake up with: '%s'\n",my_rank, slv_msg);

        // ii. respondendo para iniciar processo de calculo
        char ok_msg[50] = "Ok, master... I mean, Mommy!";
        //printf("Son %d is confirming...\n",my_rank);
        MPI_Send(ok_msg, 50, MPI_CHAR, src, HI_TAG, MPI_COMM_WORLD);
        //printf("Son %d said: '%s'\n",my_rank, ok_msg);

        // iii. recebendo a matriz para ordenacao
        int l[1], c[1];
        //printf("Son %d is waiting for the matrix to sort...\n",my_rank);
        MPI_Recv(l, 1, MPI_INT, src, LINE_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(c, 1, MPI_INT, src, COL_TAG, MPI_COMM_WORLD, &status);

        double buffer[(l[0]*c[0])];
        MPI_Recv(buffer, (l[0]*c[0]), MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD, &status);
        //printf("Son %d received:\n",my_rank);

        // iv. ordenando a matriz por colunas
        ordena_colunas(buffer,l[0],c[0]);
        //printf("| Son %d sorted the matrix.\n",my_rank);

        //printf("Son %d is sending the sorted matrix to your master...\n",my_rank);
        MPI_Ssend(buffer, (l[0]*c[0]), MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD);
        
        // v. calculando as medias das colunas
        double mean[c[0]];
        calcula_media(buffer,mean,l[0],c[0]);
        //printf("| Slave %d calculated and sent the mean. \n| | ",my_rank);
        MPI_Send(mean, c[0], MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD);
        //print_dvec(mean,c[0]);
        
        double mh[c[0]];
        calcula_media_harmonica(buffer,mh,l[0],c[0]);
        //printf("| Slave %d calculated and sent the harmonic mean. \n| | ",my_rank);
        MPI_Send(mh, c[0], MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD);
        //print_dvec(mh,c[0]);

        double var[c[0]];
        calcula_variancia(buffer,mean,var,l[0],c[0]);
        //printf("| Son %d calculated and sent the variance. \n| | ",my_rank);
        MPI_Send(var, c[0], MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD);
        //print_dvec(var,c[0]);

        double stddev[c[0]];
        calcula_desvio_padrao(var,stddev,c[0]);
        //printf("| Son %d calculated and sent the standard deviation. \n| | ",my_rank);
        MPI_Send(stddev, c[0], MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD);
        //print_dvec(stddev,c[0]);

        double varcoef[c[0]];
        calcula_coeficiente_variacao(mean,stddev,varcoef,c[0]);
        //printf("| Slave %d calculated the mean. \n| | ",my_rank);
        MPI_Send(varcoef, c[0], MPI_DOUBLE, src, MATRIX_TAG, MPI_COMM_WORLD);
        //print_dvec(varcoef,c[0]);
    }
    
    // 4. Desalocando a memoria utilizada
    free(matriz); //Desaloca a matriz
    free(media); //Desaloca o vetor de media
    free(media_har); //Desaloca o vetor de media_har
    free(mediana); //Desaloca vetor de mediana
    free(moda); //Desaloca vetor de moda
    free(variancia);  //Desaloca vetor de variância
    free(dp); //Desaloca vetor de desvio padrão
    free(cv); //Desaloca vetor de coeficiente de variação

    // 5. Finalizando o processo MPI
    MPI_Finalize();
}