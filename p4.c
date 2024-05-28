#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define MAXCARTAS 100
int valor (wchar_t carta){
    return ((carta-0x1F0A0)%16);
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

 //funçao que compara 2 constantes (para usar no qsort)
int cmp (const void * a, const void *b){return (*(int*)a - *(int*)b );}

void quickSort(int v[], int e, int d) {
    if (e < d) {
        int p = v[d];
        int i = e - 1;        
        for (int j = e; j < d; j++) {
            if (v[j] <= p) {
                i++;
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }        
        int temp = v[i + 1];
        v[i + 1] = v[d];
        v[d] = temp;       
        int partitionIndex = i + 1;
        quickSort(v, e, partitionIndex - 1);
        quickSort(v, partitionIndex + 1, d);
    }
}

int valoress (wchar_t s[], int v[]){           //funçao da p1 que guarda os valores das cartas numa array e da lhe sort 
    int size = wcslen(s);
    for (int i=0; i<size; i++){
        v[i] = valor(s[i]);
    }
    quickSort (v,0,size-1);
    return 0;
}

int conjunto (wchar_t s[]){                 //funcao da p1 que ve se é conjunto
    int r=1;
    int valorBase = valor(s[0]);
    for (int i=1; s[i]!='\0'; i++){
        if (valor (s[i]) != valorBase){
            r=0;
            break;
        }
    }
    return r;
}

int sequencia(int s[], int size){           //funcao da p1 que ve se é sequencia    
    int r=1;
    for (int i=1; i<size; i++){
        if (s[i] != s[i-1]+1){
            r=0;
            break;
        }
    }
    return r;
}

int duplaSequencia (int s[], int size){     
    int r=1;
    for (int i=0; i<size; i++){
        if ((i==size-2) && (s[i]==s[i+1])) break;
        else if ((s[i]==s[i+1]) && (s[i+2]==s[i+1]+1)) {i++;}
        else {
            r=0;
            break;
        }
    }
    return r;
}

int combValida (wchar_t s[]){
    int r=0;
       int size = wcslen (s);
        int val [size];
        valoress (s,val);
        if (conjunto (s) && size>=1 && size<=4) {r=1;}
        else if (sequencia (val,size) && size>=3) {r=1;}
        else if (duplaSequencia (val,size) && size/2>=3) {r=1;}  
        else (r=0);
    return r;
}


void organizaLinha (wchar_t s[]){                //funcao que organiza a linha por ordem crescente
    int size = wcslen (s);  
    for (int i=0; i<size-1; i++){     
        for (int j=0; j<size-i-1; j++){
            if (valor (s[j]) > valor (s[j+1])){        
                wchar_t temp;
                temp = s[j];
                s[j] = s[j+1];
                s[j+1] = temp;
            } else if (valor (s[j]) == valor (s[j+1]) && s[j] > s[j+1] ){
                wchar_t temp;
                temp = s[j];
                s[j] = s[j+1];
                s[j+1] = temp;
            }
        }
    }
}

int tipoCombinacao (wchar_t jogadaAnterior[]){
    int r=-1;
    wchar_t passo[] = L"PASSO";
    int size = wcslen(jogadaAnterior);
    int val [size];
    valoress (jogadaAnterior,val);
    if (wcscmp (jogadaAnterior,passo) == 0) {r=0;}
    else if (conjunto (jogadaAnterior) && size>=1 && size<=4) {r=1;}
    else if (sequencia (val,size) && size>=3) {r=2;}
    else if (duplaSequencia (val,size) && size/2>=3) {r=3;} 
    return r;
}

int maiorJ (wchar_t jogadarecente[], wchar_t jogada[]){
    wchar_t maiorJogada= valor(maior(jogada));
    wchar_t maiorRecente= valor(maior(jogadarecente));
    int r=0;
    if (maiorJogada>maiorRecente) r=1;
    else if (maiorJogada==maiorRecente && maior(jogada)>maior(jogadarecente)) r=1;
    return r;
}

int SoReis (int tamanhoUltima, wchar_t jogadarecente[]){
    int r=0;
    if (tamanhoUltima == 1 && valor(jogadarecente[0])==14) r=1;
    if (tamanhoUltima == 2 && valor(jogadarecente[0])==14 && valor(jogadarecente[1])==14) r=1;
    if (tamanhoUltima == 3 && valor(jogadarecente[0])==14 && valor(jogadarecente[1])==14 && valor(jogadarecente[2])==14) r=1;
    return r;
}

void guardaCombinacao(wchar_t combinacao[],wchar_t jogadas[][MAXCARTAS], int (*a)) {
    wcscpy (jogadas[*a], combinacao);
    (*a)++;
}

void gerador (wchar_t mao[], wchar_t aux[], int start,int sizeAux, int sizeJogada,wchar_t jogada[],wchar_t jogadas[][MAXCARTAS],int (*a)){
    if (sizeAux==sizeJogada){
        if (combValida(aux) && tipoCombinacao (aux)==tipoCombinacao(jogada)&& maiorJ(jogada,aux) ){
            guardaCombinacao (aux,jogadas,a);
        }
        return;
    }
    for (int i=start; mao[i]!='\0'; i++){
        aux[sizeAux] = mao[i];
        aux[sizeAux+1] = L'\0';
        gerador (mao,aux,i+1,sizeAux+1,sizeJogada,jogada,jogadas,a);
    }
}
//Funcoes usadas para gerar recursivamente as jogadas. 
//Basicamente, vai adicionando carta a carta da mao no aux, e cada vez que adiciona 1 nova carta, volta a chamar a funcao incrementando o sizeaux e o ponto de start da mao (para nao repetir)
//Quando o sizeaux for igual ao sizejogada, imprime (com os criterios estabelecidos)
void gerador2 (wchar_t mao[], wchar_t aux[], int start,int sizeAux, int sizeJogada,wchar_t jogada[],wchar_t jogadas[][MAXCARTAS],int (*a)){
    if (sizeAux==sizeJogada){
        if (combValida(aux)){
            int tamAux= wcslen(aux);
            int size = wcslen(jogada);
            if (size==1){
                if (tipoCombinacao (aux)==1){
                    if (tamAux==1 && maiorJ(jogada,aux)) guardaCombinacao (aux,jogadas,(a));
                    else if (tamAux==4) guardaCombinacao (aux,jogadas,(a));
                }
                else if (tipoCombinacao (aux)==3) guardaCombinacao (aux,jogadas,(a));
            }
            else if (size==2){
             
                if (tipoCombinacao (aux)==1 && tamAux==2 && maiorJ(jogada,aux)) guardaCombinacao (aux,jogadas,(a));
                else if (tipoCombinacao (aux)==3) guardaCombinacao (aux,jogadas,(a));
            }
            
            else if (size==3){
                if (tipoCombinacao (aux)==3) guardaCombinacao (aux,jogadas,(a));
            }
        }
        
        return;
    }
    for (int i=start; mao[i]!='\0'; i++){
        aux[sizeAux] = mao[i];
        aux[sizeAux+1] = L'\0';
        gerador2 (mao,aux,i+1,sizeAux+1,sizeJogada,jogada,jogadas,a);
    }
    
}
                                    
void TrocaMatrizes (wchar_t cartas[][MAXCARTAS], int x, int y){            //funcao que troca linhas do input 
    for (int j=0; cartas[x][j]!= '\0'; j++){
        wchar_t temp;
        temp = cartas[x][j];
        cartas[x][j] = cartas[y][j];
        cartas[y][j] = temp;
    }
}

void imprimeMatriz (wchar_t cartas[][MAXCARTAS], int nseq){
    for (int i=0; i<nseq; i++){
        int size = wcslen (cartas[i]);
            for (int j=0; cartas[i][j]!=L'\0'; j++){
                    wprintf (L"%lc", cartas[i][j]);
                    if (j!=size-1) wprintf (L" ");
            }
            wprintf (L"\n");
    }
}

void organizaMatriz (wchar_t cartas[][MAXCARTAS], int nseq,int max){
    if (nseq==1) {organizaLinha (cartas[0]);}
    for (int a=0; a<nseq; a++){
        organizaLinha (cartas[a]);
    }
    for (int cards=max; cards>=0; cards--){
        for (int i=0; i<nseq; i++){
            for (int j=1; j<nseq; j++){
                int size = wcslen (cartas[j]);
                int size2 = wcslen (cartas[j-1]);
                if (size == cards && size2 == cards){
                    int k=0;
                    while (k<cards && cartas[j][k] == cartas [j-1][k]) k++;
                    if (k<cards && valor (cartas[j][k])<valor(cartas[j-1][k])){
                        TrocaMatrizes (cartas,j,j-1);
                    } else if (k<cards && cartas[j][k]<cartas[j-1][k] && valor (cartas[j][k])==valor(cartas[j-1][k])){
                        TrocaMatrizes (cartas,j,j-1);
                    }
                }
            }
        }
    }
   
    imprimeMatriz (cartas,nseq);
}

//funcao verificaMatriz da p2, adaptada a possibilidade de existirem linhas de cartas com n elements diffs (caso 1 rei)
void verificaMatriz (wchar_t cartas[][MAXCARTAS], int nseq){
    if (nseq==0) wprintf (L"PASSO\n");
    int max=0;              //numero de cartas da maior linha gerada
    for (int i=0; i<nseq; i++){
        int cartaslinha = wcslen (cartas[i]);
        if (cartaslinha>max) max=cartaslinha;
    }
    organizaMatriz (cartas,nseq,max);
}

void Reis (wchar_t mao[],wchar_t aux[], wchar_t ultimaJogada[],wchar_t jogadas[][MAXCARTAS], int *a,int tamComb){
    if (tamComb==1) {
        gerador2 (mao,aux,0,0,1,ultimaJogada,jogadas,a);
        gerador2 (mao,aux,0,0,4,ultimaJogada,jogadas,a);      //gera comb de 4 cartas
        gerador2 (mao,aux,0,0,6,ultimaJogada,jogadas,a);      //6 cartas
    }
    else if (tamComb==2) {
            gerador2 (mao,aux,0,0,2,ultimaJogada,jogadas,a);
            gerador2 (mao,aux,0,0,8,ultimaJogada,jogadas,a);       //8 cartas
        }
    else if (tamComb==3) {
            gerador2 (mao,aux,0,0,10,ultimaJogada,jogadas,a);     //10 cartas
        }
}
int main() {
    setlocale (LC_CTYPE, "C.UTF-8");
    int testes;
    wscanf (L"%d", &testes);
    for (int i=0; i<testes; i++){
        wchar_t jogadas[5000][MAXCARTAS];      //da reset a matriz
        wprintf (L"Teste %d\n", i+1);
        wchar_t ultimaJogada[100];
        wchar_t mao[100];
        wscanf (L"%ls", &ultimaJogada);
        wscanf (L"%ls", &mao);
        int tamComb = wcslen(ultimaJogada); 
        wchar_t aux[MAXCARTAS];
        aux[0] = mao[0];
        int a=0;
        if (SoReis (tamComb,ultimaJogada)) {
            Reis (mao,aux,ultimaJogada,jogadas,&a,tamComb);
        }
        else gerador (mao,aux,0,0,tamComb,ultimaJogada,jogadas,&a);    //tamComb cartas (para so gerar comb com o mesmo n de cartas a jogada anterior)

        verificaMatriz (jogadas,a);
        a=0;
    }
    return 0;
}