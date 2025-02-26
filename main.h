//
// Created by jc on 31/10/20.
//

#ifndef TP_SOSD_MAIN_H
#define TP_SOSD_MAIN_H
#define MAXCOMANDO 4
/**
 * Operações disponiveis
 */
typedef enum {
    Erro, Mostra, Conta, Apaga, Informa, Acrescenta, Lista, Termina
} operation;

/**
 * Recebendo um argumento em formato arg deteta qual a operação a ser executada
 * @param arg
 * @return
 */
operation setCurrentOp(char *arg);

/**
 * Recebendo todos os argumentos, utiliza o setCurrentOp detetar qual a operação a ser executada
 * @param arg
 * @return
 */
operation getCurrentOperation(char *arg[]);

/**
 * Executa o programa
 */
int run(int op, char **argv);

/**
 * Recebendo o Nº de argumentos decide se o programa irá correr.
 * @param argc
 */
void canYouRunIt(int argc);

//void printError(operation currentOp, int errorNum);

int mostra(char **argv, bool devolveChar);

bool apagar(char *filepath);
bool lista(char *filepath);
void informa(char *filePath);
void acrescenta(char *origem, char *destino);



char **copyArgcv(int argc, char *argv[]);

#endif //TP_SOSD_MAIN_H
