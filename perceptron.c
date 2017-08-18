#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
//// Criado por : Thúlio Mattheus ////////////////////////////////////////////////////////////////
//// Objetivo : Perceptron para encontrar o peso ótimo de duas classes linearmente separáveis ////
//// Requisitos : Um arquivo texto para a entrada e um para os valores desejados /////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//////////////////////// Funções Auxiliares /////////////////////////
/////////////////////////////////////////////////////////////////////

// Função para contagem de linhas e espaços da entrada
void dimensionar_entradas(const char *caminho, int *N_amostras, int *N_entradas);

// Função para preenchimento da matriz de entrada
void preencher_entradas(const char *caminho, int *N_amostras, int *N_entradas, double x[*N_amostras][*N_entradas]);

// Função para mostrar na tela a matriz de entrada
void mostrar_entradas(int *N_amostras, int *N_entradas, double x[*N_amostras][*N_entradas]);

// Função para preenchimento da matriz de saidas desejadas
void preencher_desejados(const char *caminho, int *N_amostras, double d[*N_amostras]);

// Função para preenchimento do vetor de pesos
void preencher_vetor_randomico(int *tamanho, double x[*tamanho]);

// Função para inicializar o vetor de saída
void preencher_vetor_nulo(int *tamanho, double x[*tamanho]);

// Função para mostrar um vetor como vetor linha
void mostrar_vetor_linha(int *tamanho, char nome[], double x[*tamanho]);

// Função para mostrar um vetor como vetor coluna
void mostrar_vetor_coluna(int *tamanho, char nome[], double x[*tamanho]);

// Aplicação da função sinal à um vetor
void funcao_sinal(int *tamanho, double x[*tamanho]);

// e[n] = d[n] - y[n]
void calcular_erro(int *tamanho, double d[*tamanho], double y[*tamanho], double e[*tamanho]);

// Erro Médio Quadrático para um vetor
double mse(int *tamanho, double e[*tamanho]);


///////////////////////////////////////////////////////////////////
///////////////////// Função Principal ////////////////////////////
///////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {

    /////////////////////////// ENTRADA ///////////////////////////

    // Número de entradas e amostras dependem do arquivo de entradas
    int N_entradas, N_amostras, epocas_maximas=5000;

    // Passo eta e erro máximo aceitáveis
    double eta=0.01, erro_maximo=0;

    // Passo dado como parâmetro pelo usuário
    printf("Digite o valor do passo 'eta': ");
    scanf("%lf", &eta);
    // Passo dado como parâmetro pelo usuário
    printf("Digite o erro maximo aceitavel: ");
    scanf("%lf", &erro_maximo);

    // O arquivo de entradas deve ser formatado como o seguinte ou dará errado
    //
    // Os dados devem estar no formato de uma matriz M x N, onde:
    // M é o número de amostras e N é o número de entradas
    //
    // x00        x01        x02        ...        x0N
    // x10        x11        x12        ...        x1N
    //  .          .          .          .          . 
    //  .          .          .          .          . 
    //  .          .          .          .          . 
    // xM0        xM1        xM2        ...        xMN


    // Caminho do arquivo de entradas
    char caminho_entrada[] = "prova_entradas.txt"; 

    // Função para pegar as dimensões da matriz de entrada
    dimensionar_entradas(caminho_entrada, &N_amostras, &N_entradas);

    // Declarando a matriz de entrada
    double x[N_amostras][N_entradas];

    // Função para preencher a matriz x
    preencher_entradas(caminho_entrada, &N_amostras, &N_entradas, x);

    printf(" ********************************************\n");
    printf(" ************* Matriz de Entrada ************\n");
    printf(" ********************************************\n");
    mostrar_entradas(&N_amostras, &N_entradas, x);


    /////////////////////////// SAÍDA DESEJADA ///////////////////////////

    // POR ALGUM MOTIVO DESCONHECIDO,
    // SE EU USAR strcpy(caminho_entrada, "desejados2.txt");
    // A VARIÁVEL N_amostras FICA IGUAL À 0
    // ENTÃO PRECISEI CRIAR UMA STRING caminho_desejado

    // O arquivo de desejados deve ser formatado como o seguinte ou dará errado
    //
    // Os dados devem estar no formato de um vetor 1 x M, onde:
    // M é o número de amostras
    //
    // d0        d1        d2        ...        dN

    //Vetor de saídas desejadas
    double d[N_amostras];

    // Caminho do arquivo de entradas
    char caminho_desejado[] = "prova_desejados.txt";

    // Função para preencher a matriz x
    preencher_desejados(caminho_desejado, &N_amostras, d);

    printf(" ********************************************\n");
    printf(" ************* Saidas Desejadas *************\n");
    printf(" ********************************************\n");
    mostrar_vetor_linha(&N_amostras, "d", d);

    /////////////////////////// PESOS ///////////////////////////

    // Declaração do vetor de pesos
    double w[N_entradas];

    // Preenchimento dos pesos
    preencher_vetor_randomico(&N_entradas, w);

    printf(" ********************************************\n");
    printf(" ********** Vetor de Pesos Inicial **********\n");
    printf(" ********************************************\n");
    mostrar_vetor_coluna(&N_entradas, "w", w);


    /////////////////////////// SAÍDA ///////////////////////////

    // Vetor de saídas real
    double y[N_amostras];

    // Inicializar o vetor todo com 0
    preencher_vetor_nulo(&N_amostras, y);

    // Multiplicar x por w'
    for(int i=0; i<N_amostras; i++){
        for(int j=0; j<N_entradas; j++){
            y[i] += x[i][j] * w[j];
        }
    }

    // Aplicar a função de transição na saída do somatório
    funcao_sinal(&N_amostras, y);

    // Declaração do vetor de erros
    double e[N_amostras];

    // Cálculo do vetor de erros
    calcular_erro(&N_amostras, d, y, e);

    // Erro Médio Quadrático
    double erro = mse(&N_amostras, e);

    // Vetores para serem usadas no reajuste dos pesos
    double auxiliar[N_amostras][N_entradas];
    double auxiliar2[N_entradas];

    // Quantidade de épocas
    int epocas = 0;

    while(erro>erro_maximo && epocas<epocas_maximas){

        //  Zerar o auxiliar à cada nova iteração
        preencher_vetor_nulo(&N_entradas, auxiliar2);

        ///////////////////////////////////////////////////////////////////
        ////////////////////// REAJUSTE DOS PESOS /////////////////////////
        ///////////////////////////////////////////////////////////////////
        ////////////   w(n) = ( w(n) + ( ( eta * x ) * e ) )    ///////////
        ///////////////////////////////////////////////////////////////////

        // Passo 1 : 
        // auxiliar = ( eta * x )

        for(int i=0; i<N_amostras; i++){
            for(int j=0; j<N_entradas; j++){
                auxiliar[i][j] = eta * x[i][j];
            }
        }

        // Passo 2 :
        // auxiliar2 = ( ( eta * x ) * e )
        // auxiliar2 = ( auxiliar * e )

        for(int i=0; i<N_amostras; i++){
            for(int j=0; j<N_entradas; j++){
                auxiliar2[j] += auxiliar[i][j] * e[i];
            }
        }


        // Passo 3 :
        // w = w + ( ( eta * x ) * e )
        // w = w + ( auxiliar * e )
        // w = w + auxiliar2

        for(int i=0; i<N_entradas; i++){
            w[i] += auxiliar2[i];
        }

        ///////////////////////////////////////////////////////////////////
        ///////////////// RECÁLCULO DA SAÍDA E DO ERRO ////////////////////
        ///////////////////////////////////////////////////////////////////

        for(int i=0; i<N_amostras; i++){
            for(int j=0; j<N_entradas; j++){
                y[i] += x[i][j] * w[j];
            }
        }


        // Aplicar a função de transição na saída do somatório
        funcao_sinal(&N_amostras, y);

        // Ajustar os valores dos erros individuais
        calcular_erro(&N_amostras, d, y, e);

        // Ajuste do mse
        erro = mse(&N_amostras, e);

        epocas++;

    }


    printf(" ********************************************\n");
    printf(" *********** Vetor de Pesos Final ***********\n", epocas);
    printf(" ********************************************\n");
    mostrar_vetor_coluna(&N_entradas, "w", w);

    printf(" ********************************************\n");
    printf(" *********** Saida apos %d epocas ***********\n", epocas);
    printf(" ********************************************\n");
    mostrar_vetor_linha(&N_amostras, "y", y);

    printf(" ********************************************\n");
    printf(" **************** Erro Final ****************\n");
    printf(" ********************************************\n");
    printf("%lf\n", erro);

    return 0;
}


void dimensionar_entradas(const char *caminho, int *N_amostras, int *N_entradas){
    FILE *entrada;
    entrada = fopen (caminho, "r");

    //Contando as amostras e o número de entradas
    int cont_Espacos = 0;
    int cont_Linhas = 0;
    char ch = 'x';
    char linha[50];

    //Caso o arquivo exista
    if(entrada!=NULL){
        
        //Loop para Contar o numero de espacos da primeira linha
        if(cont_Linhas==0){
            //Enquanto a linha não acabar..
            while(ch != '\n'){
                //Leia o próximo caractere
                ch = fgetc(entrada);
                //Se for espaço, incremente
                if(ch == ' '){
                    cont_Espacos++;
                }
            }
            //Acabou a primeira linha
            cont_Linhas++;
        }

        //Enquanto houver linhas, incremente
        while( fgets(linha, sizeof(linha), entrada) != NULL ){
            cont_Linhas++;
        }
    }
    else{
        printf("Erro ao Abrir o Arquivo\n\n\n");
    }

    // Espaços + primeiro numero + bias
    *N_entradas = cont_Espacos+2;
    *N_amostras = cont_Linhas;

    fclose(entrada);

}


void preencher_entradas(const char *caminho, int *N_amostras, int *N_entradas, double x[*N_amostras][*N_entradas]){
    FILE *entrada;
    entrada = fopen (caminho, "r");

    // contador i
    int cont_Espacos = 0;
    // contador j
    int cont_Linhas;

    char linha[50];
    char *auxiliar;


    //Caso o arquivo exista
    if(entrada!=NULL){
        //Enquanto houver linhas, incremente
        while( fgets(linha, sizeof(linha), entrada) != NULL ){

            //Em cada linha, reinicie a coluna
            cont_Linhas = 0;

            //Primeiro elemento à ser gravado naquela linha
            x[cont_Espacos][cont_Linhas] = -1;

            //Incrementador para usar no loop seguinte
            cont_Linhas++;
            auxiliar = strtok(linha, " ");

            //Enquanto não acabar a linha
            while(auxiliar != NULL){

                //Salvando os outros valores
                x[cont_Espacos][cont_Linhas] = strtod(auxiliar,NULL);

                //Incrementando os contadores e a string
                cont_Linhas++;
                auxiliar = strtok(NULL, " ");
            }

            cont_Espacos++;
        }
    }
    else{
        printf("Erro ao Abrir o Arquivo !!!\n\n\n");
    }

    fclose(entrada);

}


void mostrar_entradas(int *N_amostras, int *N_entradas, double x[*N_amostras][*N_entradas]){
    //Mostrando o valor de x
    for(int i=0; i<*N_amostras; i++){
        for(int j=0; j<*N_entradas; j++){
            printf("x[%d][%d] = %.2lf  ", i, j, x[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void preencher_desejados(const char *caminho, int *N_amostras, double d[*N_amostras]){
    FILE *desejado;
    desejado = fopen (caminho, "r");

    int cont_Espacos;

    char linha[50];
    char *auxiliar;


    //Caso o arquivo exista
    if(desejado!=NULL){

        // Insira os dados na string linha
        fgets(linha, sizeof(linha), desejado);

            //Em cada linha, reinicie a coluna
            cont_Espacos = 0;

            //Primeiro elemento à ser gravado naquela linha
            auxiliar = strtok(linha, " ");
            d[cont_Espacos] = strtod(auxiliar,NULL);

            //Incrementador para usar no loop seguinte
            cont_Espacos++;
            auxiliar = strtok(NULL, " ");

            //Enquanto não acabar a linha
            while(auxiliar != NULL){

                //Salvando os outros valores
                d[cont_Espacos] = strtod(auxiliar, NULL);

                //Incrementando o contador e a string
                cont_Espacos++;
                auxiliar = strtok(NULL, " ");
            }

    }
    else{
        printf("Erro ao Abrir o Arquivo !!!\n\n\n");
    }

    fclose(desejado);

}


void preencher_vetor_randomico(int *tamanho, double x[*tamanho]){

    // Inicializa o gerador de números randômicos
    srand((unsigned)time(NULL));


    // Preenchimento do vetor de pesos
    for(int i=0; i<*tamanho; i++){
        x[i] = (rand()%99+1) / 100.0;
    }

}


void preencher_vetor_nulo(int *tamanho, double x[*tamanho]){

    // Preenchimento do vetor de pesos
    for(int i=0; i<*tamanho; i++){
        x[i] = 0;
    }

}


void mostrar_vetor_coluna(int *tamanho, char nome[], double x[*tamanho]){

    // Amostragem do vetor de pesos
    for(int i=0; i<*tamanho; i++){
        printf("%s[%d] = %.2lf\n", nome, i, x[i]);
    }
    printf("\n");
}


void mostrar_vetor_linha(int *tamanho, char nome[], double x[*tamanho]){

    //Mostrando o valor de d
    for(int i=0; i<*tamanho; i++){
        printf("%s[%d] = %.2lf  ", nome, i, x[i]);
    }
    printf("\n\n");

}


void funcao_sinal(int *tamanho, double x[*tamanho]){
    for(int i=0; i<*tamanho; i++){
        if(x[i] < 0){
            x[i] = -1;
        }
        else{
            x[i] = 1;
        }
    }
}


void calcular_erro(int *tamanho, double d[*tamanho], double y[*tamanho], double e[*tamanho]){
    for(int i=0; i<*tamanho; i++){
        e[i] = d[i] - y[i];
    }
}


double mse(int *tamanho, double e[*tamanho]){
    double retorno = 0;
    for(int i=0; i<*tamanho; i++){
    	retorno+= e[i]*e[i];
    }

    retorno/=*tamanho;

    return retorno;
}
