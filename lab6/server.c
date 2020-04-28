#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <winsock.h>

#define BUFFER_SIZE 120
#define MAX_CLIENTS 5

void receiveChat(){
;
}

void sendChat(){
;
}

void main(){
    WSADATA wsas;
    int result;
    WORD wersja;
    wersja = MAKEWORD (1, 1);
    result = WSAStartup (wersja , & wsas);
    SOCKET s = socket ( AF_INET , SOCK_STREAM , 0);

    
    struct sockaddr_in sa;
    memset (( void *) (& sa), 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(10000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    result = bind(s, (struct sockaddr FAR *)&sa , sizeof (sa));

    result = listen(s, MAX_CLIENTS);

    SOCKET si;
    struct sockaddr_in sc;
    int lenc ;
    printf("Serwer wystartowal. Nasluchuje na klientow.\n");
    for (;;)
    {
        lenc = sizeof (sc);   
        si = accept (s, ( struct sockaddr FAR *) &sc , &lenc );
        printf("Zaakceptowalem klienta.\n");
        char buf[BUFFER_SIZE];
        while ( recv(si , buf , BUFFER_SIZE, 0) > 0 )
        {
            char* copy_buf;
            strcpy(copy_buf, buf);
            char * ptr = strtok(copy_buf, ":");
            if(ptr != NULL){
                ptr = strtok(NULL, ":");
                if(ptr != NULL){
                    if( strcmp(ptr , "KONIEC") == 0)
                    {
                        closesocket (si);
                        WSACleanup();
                        return;
                    }
                    printf ("\n%s",buf);
                }
            }
        };
    }
}