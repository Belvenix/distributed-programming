#include <pthread.h>
#include <stdio.h>
#include <math.h>

//--------------------------------------------------------------------------
#pragma argsused
typedef struct dane_dla_watku{
    double minRange;
    double maxRange;
    double step;
    double sum;
    double (*func)(double x);
} dane_dla_watku;


// deklaracja funkcji watku
DWORD WINAPI funkcyjka (void * argumenty);
double sinus(double x);

//--------------------------------------------------------------------------

int main (int argc , char ** argv )
{
    //--------------------------INITIALIZE--------------------------
    int nThreads = 4;
    double minRange = 0.0, maxRange = 10.0, step = 0.001;
    //DWORD id; // identyfikator watku
    pthread_t* wateczki;
    dane_dla_watku* dane;
    printf("Program sluzy do obliczania calki z sinusa przy pomocy dowolnej liczby watkow (maksymalnie 10)\n");
    printf("Wraz z mozliwoscia wybrania podania wlasnego zakresu calki.\n");
    printf("Prosze podac liczbe watkow:");
    scanf("%d", &nThreads);
    fflush(stdin);
    if (nThreads <= 0 || nThreads > 20){
        nThreads = 4;
    }
    printf("\nProsze dolna granice zakresu: ");
    scanf("%lf", &minRange);
    fflush(stdin);
    printf("\nProsze gorna granice zakresu: ");
    scanf("%lf", &maxRange);
    fflush(stdin);
    printf("\n");

    printf("Wpisana liczba watkow to %d\nWpisany zakres calki <%lf,%lf>\n", nThreads, minRange, maxRange);

    wateczki = (pthread_t*) malloc(nThreads * sizeof(pthread_t));
    dane = (dane_dla_watku*) malloc(nThreads * sizeof(dane_dla_watku));
    for (int i = 0; i < nThreads; i++){
        {
            double min = minRange + (i * ((maxRange-minRange)/nThreads));
            double max = minRange + ((i + 1) * ((maxRange-minRange)/nThreads));
            dane[i].sum = 0.0;
            dane[i].step = step;
            dane[i].minRange = min;
            dane[i].maxRange = max;
            dane[i].func = sinus;
        }

        //TODO - DANE
        pthread_create(&wateczki[i],
                        NULL,
                        funkcyjka,
                      (void *) &dane[i]);

    }

    //--------------------------WAIT_FOR_THREADS--------------------------



    for(int i = 0; i < nThreads; i++){
      pthread_join(wateczki[i], NULL);
    }

    //--------------------------SHOW_RESULTS--------------------------

    double sum = 0.0;
    for(int i = 0; i < nThreads; i++){
        sum += dane[i].sum;

    }

    printf("Suma z wszystkich watkow = %f\n", sum);

    //--------------------------FREE_SPACE--------------------------

    for (int i = 0; i < nThreads; i++){
        CloseHandle(wateczki[i]);
    }

    free(dane);
    free(wateczki);
    return 0;
}


void* funkcyjka(void * argumenty){

    printf("Jestem w watku o id %x\n", GetCurrentThreadId());

    dane_dla_watku * moje_dane = (dane_dla_watku*) argumenty;

    double step = moje_dane->step;
    double max = moje_dane->maxRange;

    for (double d = moje_dane->minRange; d < max; d += step){
        if(d + step <= max){
            double rect = moje_dane->func(d + step/2) * step;
            moje_dane->sum += rect;
        }
        else{
            double dif = max - d;
            double rect = moje_dane->func(d + dif/2) * dif;
        }
    }
    printf("Suma %f, Przedzial dolny %f, Przedzial gorny %f\n", moje_dane->sum, moje_dane->minRange, moje_dane->maxRange);
    printf("Skonczylem pracowac w watku o id %x\n", GetCurrentThreadId());
}

double sinus(double x){
    //Sinus from math library
    return sin(x);
}
