#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

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

int cmp (const void * a, const void *b){            //funçao que compara 2 constantes (para usar no qsort)
    return (*(int*)a - *(int*)b );
}

int valoress (wchar_t s[], int v[]){           //funçao da p1 que guarda os valores das cartas numa array e da lhe sort 
    int size = wcslen(s);
    for (int i=0; i<size; i++){
        v[i] = valor(s[i]);
    }
    qsort (v,wcslen(s),sizeof(int),cmp);
    return 0;
}

int conjunto (wchar_t s[]){                 //funcao da p1 que ve se é conjunto
    int r=1;
    int valorBase = valor(s[0]);
    for (int i=1; s[i]!='\0'; i++){
        if (valor (s[i]) != valorBase) r=0;
    }
    return r;
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

int pertence (wchar_t mao[], wchar_t jogada[]){
    int size = wcslen (jogada);
    int aux = 0;
    int r=0;
    for (int i=0; jogada[i]!='\0'; i++){
        for (int j=0; mao[j]!='\0'; j++){
            if (jogada[i] == mao[j]) {
                aux++; break;
            } 
        }
    }
    if (aux == size) r=1;
    return r;
}

void novaMao (wchar_t mao[], wchar_t jogada[]){
    for (int i=0; jogada[i]!='\0'; i++){
        for (int j=0; mao[j]!='\0'; j++){
            if (jogada[i]==mao[j]) {
                int size = wcslen (mao);
                
                for (int z=j; z<=size-1; z++){
                  
                    mao[z] = mao[z+1];
                }
                mao[size-1] = '\0';
                
            } 
            
        }
    }
    organizaLinha (mao);
    for (int i=0; mao[i]!='\0'; i++){
        int size = wcslen (mao);
        wprintf (L"%lc", mao[i]); 
        if (i!=size-1) wprintf (L" ");
    }
}

void jogaPrimeiro (wchar_t mao[]){
    wchar_t jogada[1000];
    wscanf (L"%ls", &jogada);
    if (pertence (mao, jogada) && combValida(jogada)) novaMao (mao,jogada);
    else{ 
        organizaLinha (mao);
        for (int i=0; mao[i]!='\0'; i++){/
            int size = wcslen (mao);
            wprintf (L"%lc", mao[i]); 
            if (i!=size-1) wprintf (L" ");
        }
    }
    wprintf (L"\n");
}

int maiorJ (wchar_t jogadarecente[], wchar_t jogada[]){
    wchar_t maiorJogada= valor(maior(jogada));
    wchar_t maiorRecente= valor(maior(jogadarecente));
    int r=0;
    if (maiorJogada>maiorRecente) r=1;
    else if (maiorJogada==maiorRecente && maior(jogada)>maior(jogadarecente)) r=1;
    return r;
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

void imprime (wchar_t mao[]){
    organizaLinha (mao);
    for (int i=0; mao[i]!='\0'; i++){
        int size = wcslen (mao);
        wprintf (L"%lc", mao[i]); 
        if (i!=size-1) wprintf (L" ");
    }
}

void aux(wchar_t ptr[], wchar_t jogada[]){          //Aponta o ptr para a ultima jogada que nao é "PASSO"
    wchar_t passo[] = L"PASSO";
    if (wcscmp (jogada, passo)!=0){
        wcscpy (ptr,jogada);
    }
}

int auxReis (wchar_t jogadarecente[],wchar_t mao[],int size, wchar_t jogada[]){
    if (maiorJ(jogadarecente,jogada)&& size==1 && tipoCombinacao(jogada)==1){
        novaMao (mao,jogada);
        wprintf (L"\n");
        return 0;
    }
    if (valor(jogadarecente[0])==14 && size==4 && tipoCombinacao(jogada)==1){
        novaMao (mao,jogada);
        wprintf (L"\n");
        return 0;
    }
    //Caso haja Rei na ultima jogada e nos jogamos uma dupla sequencia de tamanho tres
    if (valor(jogadarecente[0])==14 && size==6 && tipoCombinacao(jogada)==3){
        novaMao (mao,jogada);
        wprintf (L"\n");
        return 0;
    } 
    else {
        imprime (mao);
        wprintf (L"\n");
        return 0;
    }
    return 0;
}

int Reis (int tamanhoUltima, wchar_t mao[], wchar_t jogadarecente[], int size, wchar_t jogada[]){
  //Caso haja Rei na ultima jogada e nos jogamoscomb com 4 cartas
    if (tamanhoUltima==1){ auxReis (jogadarecente,mao,size,jogada); return 0;}
    
    if (tamanhoUltima==2 && valor(jogadarecente[0])==14 && valor(jogadarecente[1])==14 && size==8 && tipoCombinacao(jogada)==3){
        novaMao (mao,jogada);
        wprintf (L"\n");
        return 0;
    } 
    //Caso haja um par de reis e nos jogamos uma dupla sequencia de tamanho 4
    if (tamanhoUltima==2 &&  maiorJ(jogadarecente,jogada)&& tipoCombinacao(jogada)==1){
        novaMao (mao,jogada);
        wprintf (L"\n");
        return 0;
    } 
    //Caso haja um trio de reis e nos jogamos uma dupla sequencia de tamanho 5
    if (tamanhoUltima==3 && valor(jogadarecente[0])==14 && valor(jogadarecente[1])==14 && valor(jogadarecente[2])==14 && size==10 && tipoCombinacao(jogada)==3){
        novaMao (mao,jogada);
        wprintf (L"\n");
        return 0;
    } 
    else {imprime(mao); wprintf (L"\n");}

    return 0;
}

int SoReis (int tamanhoUltima, wchar_t jogadarecente[]){
    int r=0;
    if (tamanhoUltima == 1 && valor(jogadarecente[0])==14) r=1;
    if (tamanhoUltima == 2 && valor(jogadarecente[0])==14 && valor(jogadarecente[1])==14) r=1;
    if (tamanhoUltima == 3 && valor(jogadarecente[0])==14 && valor(jogadarecente[1])==14 && valor(jogadarecente[2])==14) r=1;
    return r;
}


int casos (wchar_t mao[], wchar_t jogada[], int njogada, int combinacoes[], wchar_t jogadarecente[]){
    int tamanhoUltima = wcslen(jogadarecente);
    int size = wcslen(jogada);
    //Caso a minha jogada seja passar
    if (tipoCombinacao (jogada)==0) {imprime (mao); wprintf (L"\n");} 
    //Caso hajam 3 PASSO seguidos              
    else if (combinacoes[njogada-1]==0 && combinacoes[njogada-2]==0 && combinacoes[njogada-3]==0 ){
        if (combValida (jogada)){
            novaMao (mao, jogada);
            wprintf (L"\n");
        } else {
            imprime (mao);
            wprintf (L"\n");
        }
        
    }
    //Caso Reis
    else if (SoReis (tamanhoUltima, jogadarecente)){
        Reis (tamanhoUltima, mao, jogadarecente, size, jogada);
    }
    //Caso "normal"
    else if (pertence (mao,jogada) && combValida (jogada) && (tipoCombinacao(jogada)==tipoCombinacao (jogadarecente)) && (size==tamanhoUltima) && maiorJ (jogadarecente,jogada)){   
        novaMao (mao, jogada);
        wprintf (L"\n");
        
    }
    //Caso jogada invalida
    else
    {
        imprime (mao);
        wprintf (L"\n");
    }
    return 0;
}

void jogaNext (wchar_t mao[], int njogada, int combinacoes[]){
    wchar_t jogada[1000];
    wchar_t jogadarecente[1000];
    for (int i=0; i<njogada; i++){
        wchar_t jogadaAnterior[1000];
        wscanf (L"%ls", &jogadaAnterior);
        combinacoes[i] = tipoCombinacao (jogadaAnterior);  
        aux (jogadarecente,jogadaAnterior);
    }
    wscanf (L"%ls", &jogada);
    casos (mao,jogada,njogada,combinacoes, jogadarecente);
}  

int main(){
    setlocale (LC_CTYPE, "C.UTF-8");
    int testes;
    wscanf (L"%d", &testes);
    for (int i=0; i<testes; i++){
        wprintf (L"Teste %d\n", i+1);
        int nJogada;
        int comb[1000];
        wchar_t mao[1000];
        wscanf (L"%d", &nJogada);
        wscanf (L"%ls", &mao);
        if (nJogada == 0) (jogaPrimeiro (mao));
        else{
            jogaNext (mao,nJogada,comb);
        }
    }
    return 0;
}