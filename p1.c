#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

//determinar o valor de cada carta 1-14 (1-Ás, 14-Rei)
int valor (wchar_t carta){
    wchar_t simbolos= 0x1F0A0;
    int valor = (carta - simbolos)%16; //para dar um intervalo de 1-14
    return valor;
}

wchar_t maior(wchar_t s[]) { 
    wchar_t maior = s[0]; 
    for (int i = 1; s[i] != '\0'; i++) { 
        if (valor(s[i]) > valor(maior)) { 
            maior = s[i];
        } 
        else if (valor (s[i]) == valor (maior) && s[i]>maior){
            maior = s[i];
        } 
    }
    return maior;
}   
int conjunto (wchar_t s[]){
    int r=1;
    int valorBase = valor(s[0]);
    for (int i=1; s[i]!='\0'; i++){
        if (valor (s[i]) != valorBase) r=0;
    }
    return r;
}

int cmp (const void * a, const void *b){            //funçao que compara 2 constantes (para usar no qsort)
    return (*(int*)a - *(int*)b );
}

int valoress (wchar_t s[], int v[]){
    int size = wcslen(s);
    for (int i=0; i<size; i++){
        v[i] = valor(s[i]);
    }
    qsort (v,wcslen(s),sizeof(int),cmp);
    return 0;
}

int sequencia(int s[], int size){
    int r=1;
    for (int i=1; i<size; i++){
        if (s[i] != s[i-1]+1) r=0;
    }
    return r;
}

int duplaSequencia (int s[], int size){     
    int r=1;
    for (int i=0; i<size; i++){
        if ((i==size-2) && (s[i]==s[i+1])) break;
        else if ((s[i]==s[i+1]) && (s[i+2]==s[i+1]+1)) {i++;}
        else {r=0;}
    }
    return r;
}

int main(){
    setlocale (LC_CTYPE, "C.UTF-8");
    int numInputs;
    wscanf (L"%d", &numInputs);
    wchar_t s[100];
    for (int i=0; i<numInputs; i++){
        wscanf (L"%ls", &s);
        int val[wcslen (s)];        //array que vai servir para guardar os valores de cada carta
        int size = wcslen (s);      //n de cartas no input
        valoress (s,val);


        if (conjunto(s) && size>=1 && size<=4) {
            wprintf(L"conjunto com %d cartas onde a carta mais alta é %lc\n",size, maior(s));
        } 
          else if (sequencia(val,size) && size>=3) {
        wprintf(L"sequência com %d cartas onde a carta mais alta é %lc\n", size, maior (s));
        }
        else if (duplaSequencia (val, size) && size/2>=3){
        wprintf (L"dupla sequência com %d cartas onde a carta mais alta é %lc\n", size/2, maior(s));
        }
        else wprintf (L"Nada!\n");

    }
    return 0;
}
