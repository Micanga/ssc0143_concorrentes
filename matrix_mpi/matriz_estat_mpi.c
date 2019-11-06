#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "mpi.h"

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
    void *buffer = NULL;
    int count, src, dst, rank, my_rank, n_proc, root;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Status status;

    // 1. Lendo o problema de entrada
    // a. numero de linhas e colunas da matriz de entrada
    fscanf(stdin, "%d %d\n", &lin, &col); 

    // b. alocando memoria para as variaveis definidas
    matriz=(double *)malloc(lin*col * sizeof(double)); //Aloca a matriz
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
        char mst_msg[50] = "Let's start the routine!";
        MPI_Send(mst_msg, 50, MPI_CHAR, dst, my_rank, MPI_COMM_WORLD);
        printf("Master sent: '%s'\n",mst_msg);

        // ii. aguardando resposta
        char mst_ans[50];
        printf("Master waiting for answer...\n");
        MPI_Recv(mst_ans, 50, MPI_CHAR, MPI_ANY_SOURCE, dst, MPI_COMM_WORLD, &status);
        printf("Master received: '%s'\n",mst_ans);

        /*for(rank = 1; rank < n_proc; rank++){
            // i. aguardando mensagem desbloquente para receber resposta
            MPI_Recv(buffer, 1, MPI_DOUBLE, MPI_ANY_SOURCE, rank, MPI_COMM_WORLD, &status);
            
            // ii. recebendo a resposta
            MPI_Get_count(&status, MPI_DOUBLE, &count);
            src = status.MPI_SOURCE;

            // iii. montando resposta final
            for(int i=0; i < count; i++)
                vet[(src-1) + i * (n_proc-1)] = buffer[i];
        }*/
    }
    // b. processos escravos
    else{
        // i. aguardando ordem do mestre
        char slv_msg[50];
        printf("Slaves waiting for message...\n");
        MPI_Recv(slv_msg, 50, MPI_CHAR, MPI_ANY_SOURCE, root, MPI_COMM_WORLD, &status);
        printf("Slaves received: '%s'\n",slv_msg);

        // ii. respondendo para iniciar processo de calculo
        char ok_msg[50] = "Ok, master!";
        printf("Slaves sending confirmation...\n");
        MPI_Send(ok_msg, 50, MPI_CHAR, root, my_rank, MPI_COMM_WORLD);
        printf("Slaves sent: '%s'\n",ok_msg);

        /*if(rank == 1){
            ordena_colunas(matriz,lin,col);

            MPI_Send(vet, elem, MPI_FLOAT, 0, my_rank, MPI_COMM_WORLD);
        }
        else{
            MPI_Recv();

            calcula_media(matriz,media,lin,col);
            calcula_media_harmonica(matriz,media_har,lin,col);
            calcula_mediana(matriz,mediana,lin,col);
            calcula_moda(matriz,moda,lin,col);

            calcula_variancia(matriz,media,variancia,lin,col);
            calcula_desvio_padrao(variancia,dp,col);
            calcula_coeficiente_variacao(media,dp,cv,col);
        }*/
    }

    // 3. Imprimindo os resultados obtidos
    //print_result(media,media_har,mediana,moda,variancia,dp,cv,col);
    
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