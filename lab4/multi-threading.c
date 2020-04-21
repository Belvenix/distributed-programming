#include <windows.h>
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
    DWORD id; // identyfikator watku
    HANDLE* wateczki;
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

    wateczki = (HANDLE*) malloc(nThreads * sizeof(HANDLE));
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
        wateczki[i] = CreateThread(
            NULL , // atrybuty bezpieczenstwa
            0, // inicjalna wielkosc stosu
            funkcyjka, // funkcja watku
            (void *) &dane[i],// dane dla funkcji watku
            0, // flagi utworzenia
            &id
        );
        if( wateczki[i] != INVALID_HANDLE_VALUE ){
            printf ("Utworzylem watek o id %x\n", id);
            // ustawienie priorytetu
            SetThreadPriority( wateczki[i], THREAD_PRIORITY_NORMAL);
        }
        else{
            printf("Wystapil blad!\n");
        }
    }

    //--------------------------WAIT_FOR_THREADS--------------------------

    DWORD dwEvent = WaitForMultipleObjects(
        nThreads, // number of objects in array
        wateczki, // array of objects
        TRUE, // wait for all objects
        10000 // ten-second wait
    );

    switch (dwEvent){
        case WAIT_OBJECT_0:
            printf("Wszystkie watki skonczyly swoja prace!\n");
            break;

        case WAIT_TIMEOUT:
            printf("Wait timed out.\n");
            break;

        default:
            printf("Wait error: %d\n", GetLastError());
            ExitProcess(0);
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


DWORD WINAPI funkcyjka(void * argumenty){

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