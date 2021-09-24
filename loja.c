#include <stdio.h>
#include <stdlib.h>

typedef struct item Item;
struct item
{
    int altura;   
    int largura;    
    int comprimento; 
    int volume;
} ; 


//Função que recebe como parametros altura, largura e comprimento de um objeto/caixa e calcula o seu volume total(estes dados são armazenados na estrutura do objeto)
Item criaItem(int altura, int largura, int comprimento){
    Item objeto;
    objeto.altura = altura;
    objeto.largura = largura;
    objeto.comprimento = comprimento;
    objeto.volume = altura * largura * comprimento;
    return objeto;
}

//imprime dados de um objeto ou caixa como altura, largura, comprimento e volume
void imprimeStruct(Item itens){
    printf("altura = %d\nlargura = %d\ncomprimento = %d\nvolume = %d\n",itens.altura,itens.largura,itens.comprimento,itens.volume);
}


// Gira objetos e caixas para que seja mais facil comparar se é possivel coloca-los na caixa
// Se a altura de um objeto é maior que a altura da caixa não significa que esse objeto não possa ser colocado deitado nessa caixa
Item giraEixo(Item objeto){
    int auxiliar;
    int vetor[3];
    Item novoObjeto;
    vetor[0] = objeto.altura;
    vetor[1] = objeto.largura;
    vetor[2] = objeto.comprimento;
    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            if (vetor[j] > vetor[j + 1]) {
                auxiliar = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = auxiliar;
            }
        }
    }
    //printf("%d %d %d\n",vetor[2],vetor[1],vetor[0]);
    novoObjeto = criaItem(vetor[2],vetor[1],vetor[0]);
    return novoObjeto;
}


// Testa se um objeto cabe na caixa baseado nos seus parametros altura largura e comprimento
// Dependendo do objeto, mesmo que seu volume seja menor que o da caixa suas dimensoes podem ultrapassar alguma dimensao da caixa
int cabeNaCaixa(Item objeto, Item caixa){
    Item caixaRotacionada;
    caixaRotacionada = giraEixo(caixa);
    //imprimeStruct(itemRotacionado);
    Item itemRotacionado;
    itemRotacionado = giraEixo(objeto);
    //imprimeStruct(itemRotacionado);

    if(itemRotacionado.altura < caixaRotacionada.altura && itemRotacionado.largura < caixaRotacionada.largura && itemRotacionado.comprimento < caixaRotacionada.comprimento){
        return 1;
    }

    return 0;
}

// Tenta inserir todos objetos na caixa ao mesmo tempo retorna 1 se for possivel e 0 se não for
int colocaTodosNaCaixa(Item *itens,int n, Item caixa){
    int volumeDosItens = 0;
    for(int i = 0;  i < n; i++){
        if(!cabeNaCaixa(itens[i],caixa)){
            return 0;
        }
        volumeDosItens += itens[i].volume;
    }
    
    if(volumeDosItens < caixa.volume){
        printf("Dimensoes da Caixa: %d %d %d\nVolume da Caixa Utilizada: %d",caixa.altura,caixa.largura,caixa.comprimento,caixa.volume);
        return 1;
    }
    return 0;
}


int retornaVolumeDosItens(Item *itens,int n){
    int volumeDosItens = 0;
     for(int i = 0;  i < n; i++){
        volumeDosItens += itens[i].volume;
    }
    //printf("Volume total dos Itens: %d\n",volumeDosItens);
    return volumeDosItens;
}



int menorLado(Item objeto){
    //utiliza a função giraEixo para algo diferente dessa vez
    // A função gira eixo sempre deixa no novo Objeto rotacionado a menor dimensao no comprimento
    // Baseado nisso retorna um valor pra identificar qual a menor dimensão da caixa Base para a menor Caixa possivel
    Item objetoRotacionado = giraEixo(objeto);
    if(objeto.altura == objetoRotacionado.comprimento)
        return 0;
    else if(objeto.largura == objetoRotacionado.comprimento)
            return 1;
    else if(objeto.comprimento ==  objetoRotacionado.comprimento)
            return 2;
    else
        printf("Houve um erro nas funcoes anteriores");
    return -1;
}

Item calculaMenorCaixa(Item *itens, int n){
    int volumeDosItens,volumeDaCaixa;
    int altura,largura,comprimento;
    volumeDosItens = retornaVolumeDosItens(itens,n);
    altura = 0;
    largura = 0;
    comprimento = 0;
    
    //Calcula as dimensões dos maiores lados de altura, largura e comprimento
    for(int i = 0; i < n; i++){
        if(itens[i].altura > altura)
            altura = itens[i].altura;
        if(itens[i].largura > largura)
            largura = itens[i].largura;
        if(itens[i].comprimento > comprimento)
            comprimento = itens[i].comprimento;        
    }
    //printf("altura base da caixa: %d\nlargura base da caixa: %d\ncomprimento base da caixa: %d\n",altura,largura,comprimento);
    
    //Cria uma caixa com tamanho base inicial de acordo com o tamanho dos objetos que irão dentro da caixa
    Item caixaBase = criaItem(altura,largura,comprimento);
    int numMenorDimensao =  menorLado(caixaBase);

    volumeDaCaixa = altura * largura * comprimento;
    int primeiraVez = 1;


    while(volumeDaCaixa < volumeDosItens){
        //Adiciona 2cm para cada dimensao da caixa Base na primeira vez que entra no while, para se ter um espaço minimo para que os objetos sejam colocados
        if(primeiraVez){
            altura +=2;
            largura+=2;
            comprimento+=2;
            primeiraVez = 0;
        } else{

            //Continua adicionando +1 cm para aquela que era a menor dimensao da caixa ate que o volume dela seja suficiente para que sejam possíveis colocar todos os objetos dentro dela
            /*Por exemplo
             Se tivermos 5 objetos
             20 30 10
             40 30 20
             50 20 10
             20 20 40
             30 30 30
             Teremos maiores dimensoes medidas dessa maneira
             objeto de maior altura = 50
             objeto de maior largura = 30
             objeto de maior comprimento = 40
             Adicionamos +2cm em cada dimensao para se ter um espaço minimo para os objetos
             Novas dimensoes = 52 32 42
             Volume total dos objetos = 83000
             volume da caixa base = 69888
             Ainda não é suficiente
             Considerando que podemos girar os objetos ao colocar na caixa e que eles não precisam ir sempre com suas alturas,larguras e comprimentos corretamente
             Sabemos que todos objetos se girados com que seu maior lado represente altura ainda terao altura 2cm menor que a altura base da caixa
             Se girarmos fazendo com que a segunda maior dimensao dos objetos nesse caso exemplo seja comprimento a segunda maior dimensao tambem tera 2cm a menos
             agora aumentando 1cm de cada vez do menor lado e calculando o novoVolume ate que esse calculo de volume seja maior que o volume total dos objetos então
             conseguiremos a menor caixa possível que sirva para resolver o problema.
            
             */
            if(numMenorDimensao == 0)
                altura++;
            else if(numMenorDimensao == 1)
                largura++;
            else if(numMenorDimensao == 2)
                comprimento++;
            else
                printf("UM ERRO MUITO GRANDE ACONTECEU");
            //Espera-se que ultimo else nunca aconteça

        }
        volumeDaCaixa = altura * comprimento * largura;
    }
    return criaItem(altura,largura,comprimento);
}


int main(){
    int altura, largura, comprimento,i;
    FILE *arq;

    //caixas criadas
    Item caixa1 = criaItem(30,40,80);
    Item caixa2 = criaItem(80,50,40);
    Item caixa3 = criaItem(50,80,60);
    Item menorCaixa;
    //recebe o nome do arquivo de entrada e testa se ele existe
    arq = fopen("entrada.txt", "rt");
    if (arq == NULL){
        printf("Erro na LEITURA do arquivo\n");
        return 0;
    }   


    Item *itens;
    itens = (Item *) malloc(sizeof(struct item));
    i = 0;
    while(!feof(arq)){
        itens = (Item *) realloc(itens,(i+1)*sizeof(struct item));
        fscanf(arq,"%d %d %d",&altura,&largura,&comprimento);
        //printf("%d %d %d\n",altura,largura,comprimento);
        itens[i] = criaItem(altura,largura,comprimento);
        //imprimeStruct(itens[i]);
        i++;
    };
    //imprimeStruct(itens[0]);
    //printf("\n");            ,,,,,,,,,,,,,,    mb h bb  878888 nm9999999999
    
    //testa se é possivel colocar todos objetos na primeira caixa 
    // senao for possivel tenta colocar nas proximas, senao for possivel colocar os objetos em nenhuma caixa fecha o programa
    printf("Volume Total dos Itens: %d\n",retornaVolumeDosItens(itens,i));
    printf("---------------------- Calculo da Menor Caixa ----------------------\n");
    menorCaixa = calculaMenorCaixa(itens,i);
    printf("Dimensoes da Menor Caixa: %d %d %d\nVolume da Menor Caixa: %d\n",menorCaixa.altura,menorCaixa.largura,menorCaixa.comprimento,menorCaixa.volume);
    printf("---------------------- Calculo de Caixas Fabricadas ----------------------\n");
    if(!colocaTodosNaCaixa(itens,i,caixa1)){
        printf("Nao foi possivel colocar os produtos na Caixa 1\n");
        if(!colocaTodosNaCaixa(itens,i,caixa2)){
            printf("Nao foi possivel colocar os produtos na Caixa 2\n");
            if(!colocaTodosNaCaixa(itens,i,caixa3))
                printf("Nao ha uma caixa pre fabricada que sirva para colocar todos itens ao mesmo tempo!\n");
        }
    }
            
    free(itens);
    fclose(arq);
    
    return 0;
}