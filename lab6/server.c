#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <winsock.h>

#define BUFFER_SIZE 120
#define MAX_CLIENTS 5

typedef struct dane_dla_watku{
    SOCKET s;
} dane_dla_watku;

DWORD WINAPI waitforClient(void * argumenty){

    dane_dla_watku * moje_dane = (dane_dla_watku*) argumenty;
    SOCKET si;
    struct sockaddr_in sc;
    int lenc;
    for (;;)
    {
        lenc = sizeof (sc);   
        si = accept (moje_dane->s, ( struct sockaddr FAR *) &sc , &lenc);
        char buf[BUFFER_SIZE];
        if(recv(si,buf, BUFFER_SIZE, 0) > 0){
            printf("\nNowy klient %s wszedl do chatu.\n", buf);
        }
        while (recv(si , buf , BUFFER_SIZE, 0) > 0){
            char *dot = strrchr(buf, ':');
            if (dot && !strcmp(dot, ":KONIEC")){
                closesocket(si);
                WSACleanup();
                return 0;
            }
            printf ("\n%s",buf);
            
        };
    }
}

void main(){
    DWORD id;

    int nThreads = 5;
    HANDLE* wateczki;
    dane_dla_watku* dane;
    wateczki = (HANDLE*) malloc(nThreads * sizeof(HANDLE));
    dane = (dane_dla_watku*) malloc(nThreads * sizeof(dane_dla_watku));
    WSADATA wsas;
    int result;
    WORD wersja;
    wersja = MAKEWORD (1, 1);
    result = WSAStartup (wersja , & wsas);
    SOCKET s = socket( AF_INET , SOCK_STREAM , 0);

    
    struct sockaddr_in sa;
    memset (( void *) (& sa), 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(10000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    result = bind(s, (struct sockaddr FAR *)&sa , sizeof (sa));

    result = listen(s, MAX_CLIENTS);
    printf("Serwer wystartowal. Nasluchuje na klientow.\n");

    for (int i = 0; i < nThreads; i++){
        {
            dane[i].s = s;
        }
        wateczki[i] = CreateThread(
            NULL , // atrybuty bezpieczenstwa
            0, // inicjalna wielkosc stosu
            waitforClient, // funkcja watku
            (void *) &dane[i],// dane dla funkcji watku
            0, // flagi utworzenia
            &id
        );
        if( wateczki[i] != INVALID_HANDLE_VALUE ){
            //printf ("Utworzylem watek o id %x\n", id);
            // ustawienie priorytetu
            SetThreadPriority( wateczki[i], THREAD_PRIORITY_NORMAL);
        }
        else{
            printf("Wystapil blad!\n");
        }
    }
    
    DWORD dwEvent = WaitForMultipleObjects(
        nThreads, // number of objects in array
        wateczki, // array of objects
        FALSE, // wait for all objects
        INFINITE // ten-second wait
    );

    switch (dwEvent){
        case WAIT_TIMEOUT:
            printf("Wait timed out.\n");
            break;

        case WAIT_FAILED:
            printf("Wait error: %d\n", GetLastError());
            break;

        default:
            printf("\nZakonczono chat\n");
            ExitProcess(0);
    }


}