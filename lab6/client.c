#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <winsock.h> 
#include <string.h>

#define BUFFER_SIZE 120

void main(){
    int dlug_login;
    const char* argv = "127.0.0.1";
    char * arr;
    SOCKET s;
    struct sockaddr_in sa;
    WSADATA wsas;
    WORD wersja;
    wersja = MAKEWORD (2 ,0);
    WSAStartup (wersja , & wsas );
    {
        char login[BUFFER_SIZE];
        printf("Witamy w kliencie SuperChatu! Najpierw prosimy o podanie swojego loginu:\n");
        fgets(login , BUFFER_SIZE, stdin );
        dlug_login = strlen (login);
        arr = (char*) malloc(dlug_login * sizeof(char) + 1);
        for(int i = 0; i < dlug_login; i++){
            arr[i] = login[i];
        }
        //printf("Wpisany login to:'%s'\n", arr);
        arr[dlug_login++ - 1] = ':'; arr[dlug_login++-1] = '\0';
        //printf("Wpisany login to:'%s'\n", arr);
    }
    s = socket ( AF_INET , SOCK_STREAM , 0);
    memset (( void *) (& sa), 0, sizeof (sa));
    sa.sin_family = AF_INET ;
    sa.sin_port = htons (10000) ;
    sa.sin_addr.s_addr = inet_addr(argv);

    printf ("\nProbuje sie polaczyc z serwerem.") ;
    int result ;
    result = connect (s, ( struct sockaddr FAR *) &sa , sizeof (sa));
    if( result == SOCKET_ERROR )
    {
        printf ("\nBlad polaczenia!") ;
        return ;
    }

    int dlug;
    char buf [BUFFER_SIZE-dlug_login];
    printf ("\nJestem podlaczony do servera! Prosze wpisac KONIEC aby zakonczyc chat.\nJezeli chcecz wyjsc z chatu prosze wpisac WYCHODZE\n");
    for (;;)
    {
        fgets (buf , BUFFER_SIZE-dlug_login, stdin );
        dlug = strlen (buf); buf[dlug-1] = '\0';
        char message [BUFFER_SIZE];
        strcpy(message, arr);
        strcat(message, buf);
        message[dlug + dlug_login - 1] = '\0';
        printf("Bufor ma: '%s', login ma: '%s', wiadomosc ma: '%s'\n", buf, arr, message);
        send(s, message , dlug + dlug_login - 1, 0); 
        if( strcmp (buf , "KONIEC") == 0 || strcmp (buf , "WYCHODZE") == 0) break ;
    }
    free(arr);

    closesocket(s);
    WSACleanup();
}