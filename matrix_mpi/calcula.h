#ifndef __CALCULA_H__
#define __CALCULA_H__

#include <math.h>

void calcula_media(double *, double *, int , int );
void calcula_media_harmonica(double *, double *, int , int );
void calcula_mediana(double *, double *, int , int );
double moda_aux(double *,int );
void calcula_moda(double *,double *moda, int , int );
void calcula_variancia(double *, double *,double *, int , int );
void calcula_desvio_padrao(double *,double *, int );
void calcula_coeficiente_variacao(double *,double *,double *, int );

#endif