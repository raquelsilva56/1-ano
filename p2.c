#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

int valor (wchar_t carta){
    wchar_t simbolos= 0x1F0A0;
    int valor = (carta - simbolos)%16; //para dar um intervalo de 1-14
    return valor;
}

int conjunto (wchar_t s[]){                 //funcao da p1 que ve se é conjunto
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

int sequencia(int s[], int size){           //funcao da p1 que ve se é sequencia    
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
int valoress (wchar_t s[], int v[]){           //funçao da p1 que guarda os valores das cartas numa array e da lhe sort 
    int size = wcslen(s);
    for (int i=0; i<size; i++){
        v[i] = valor(s[i]);
    }
    qsort (v,wcslen(s),sizeof(int),cmp);
    return 0;
}

int difCombinacao (wchar_t s[][15], int nseq){
    int combinacao[nseq];             //var associada a que tipo de combinaçao é
    int r=0;
    for (int a=0; a<nseq;a++){
       int size = wcslen (s[a]);
        int val [size];
        valoress (s[a],val);
        if (conjunto (s[a]) && size>=1 && size<=4) {combinacao[a] = 1;}
        else if (sequencia (val,size) && size>=3) {combinacao[a] = 2;}
        else if (duplaSequencia (val,size) && size/2>=3) {combinacao[a] = 3;}  
        else (combinacao[a] = 0);
    }
    for (int i=1; i<nseq; i++){
        if (combinacao[i]!=combinacao[i-1] || combinacao[i]==0) r=1;
    }
    return r;
}


void organizaLinha (wchar_t s[]){                //funcao que organiza a linha por ordem crescente
    int size = wcslen (s);
    for (int i=0; i<size-1; i++){                  
        for (int j=0; j<size-i-1; j++){
            if (valor (s[j]) > valor (s[j+1])){         //bubble sort que demos em PI 
                wchar_t temp[size];
                temp[j] = s[j];
                s[j] = s[j+1];
                s[j+1] = temp[j];
            } else if (valor (s[j]) == valor (s[j+1]) && s[j] > s[j+1] ){
                wchar_t temp[size];
                temp[j] = s[j];
                s[j] = s[j+1];
                s[j+1] = temp[j];
            }
        }
    }
}
    
void armazenaCartas (int nseqs, wchar_t cartas[][15]){              //armazenar o scanf de cada linha de cartas num espaco na matriz
    for (int i=0; i<nseqs; i++){
        wscanf (L"%ls", cartas[i]);
    }
}

int contacartas (int nseqs, wchar_t cartas[][15]){                  //conta as cartas na matriz toda (todos os inputs juntos)
    int soma=0;
    for (int i=0; i<nseqs; i++){
        for (int j=0; cartas[i][j] != L'\0'; j++){
            soma++;
        }
    }
    return soma;
}

                                    
void TrocaMatrizes (wchar_t cartas[][15], int x, int y){            //funcao que troca linhas do input 
    for (int j=0; cartas[x][j]!= '\0'; j++){
        wchar_t temp[100][14];
        temp[0][j] = cartas[x][j];
        cartas[x][j] = cartas[y][j];
        cartas[y][j] = temp[0][j];
    }
}

void verificaMatriz (wchar_t cartas[][15], int nseq){
    int cartaslinha = (contacartas(nseq, cartas)/nseq);
    for (int a=0; a<nseq; a++){
        organizaLinha (cartas[a]);
    }
    for (int i = 0; i < nseq - 1; i++) {
        for (int j = 1,x=0; j < nseq; j++) {     
            if ((valor (cartas[j][x]))< (valor (cartas[j-1][x]))){
                    TrocaMatrizes (cartas, j,j-1);
                } else if (((valor (cartas[j][x])) == (valor (cartas[j-1][x])))&& ((cartas[j][cartaslinha-1]) < (cartas[j-1][cartaslinha-1]))){
                    TrocaMatrizes (cartas, j,j-1);
            }
        }
    }
        for (int i=0; i<nseq; i++){
            for (int j=0; cartas[i][j]!=L'\0'; j++){
                    wprintf (L"%lc", cartas[i][j]);
                    if (j!=cartaslinha-1) wprintf (L" ");
            }
            wprintf (L"\n");
        }
}

int cartaspLinha (int nseq, wchar_t cartas[][15]){              //conta o numero de cartas por linha, e vê se são diferentes (pode se juntar com o contacartas mais tarde, por agora n mexer)
    int ncartas[nseq];
    int r=0;
    for (int i=0; i<nseq; i++){
        ncartas[i]=0;
        for (int j=0; cartas[i][j] != L'\0'; j++){
            ncartas[i]++;
        }
    }
    for (int i=1; i<nseq; i++){
        if (ncartas[i] != ncartas[i-1]) r=1;
    }
    return r;
}

int main(){
    setlocale (LC_CTYPE, "C.UTF-8");
    int testes, nseq;
    wscanf (L"%d", &testes);
    for (int i=0; i<testes; i++){
        wprintf (L"Teste %d\n", i+1);
        wscanf (L"%d", &nseq);
        wchar_t cartas[1000][15];
        armazenaCartas (nseq, cartas);
        if ((cartaspLinha(nseq,cartas)) || (difCombinacao (cartas,nseq))){ 
            wprintf (L"Combinações não iguais!\n");
        }
        else {
            verificaMatriz(cartas,nseq);     //funcao que verifica se esta tudo por ordem crescente e imprime os chars
        }
    }
    return 0;     
}


