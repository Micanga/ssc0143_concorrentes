#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include "sort.h"
#include "calcula.h"

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
    int lin = 0, col = 0, i = 0, j = 0;
    double *matriz = NULL;
    double *mediana = NULL, *media = NULL, *media_har = NULL, *moda = NULL;
    double *variancia = NULL, *dp = NULL, *cv = NULL;

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
    calcula_media(matriz,media,lin,col);
    calcula_media_harmonica(matriz,media_har,lin,col);
    ordena_colunas(matriz,lin,col);
    calcula_mediana(matriz,mediana,lin,col);
    calcula_moda(matriz,moda,lin,col);
    calcula_variancia(matriz,media,variancia,lin,col);
    calcula_desvio_padrao(variancia,dp,col);
    calcula_coeficiente_variacao(media,dp,cv,col);

    // 3. Imprimindo os resultados obtidos
    print_result(media,media_har,mediana,moda,variancia,dp,cv,col);
    
    // 4. Desalocando a memoria utilizada
    free(matriz); //Desaloca a matriz
    free(media); //Desaloca o vetor de media
    free(media_har); //Desaloca o vetor de media_har
    free(mediana); //Desaloca vetor de mediana
    free(moda); //Desaloca vetor de moda
    free(variancia);  //Desaloca vetor de variância
    free(dp); //Desaloca vetor de desvio padrão
    free(cv); //Desaloca vetor de coeficiente de variação
}