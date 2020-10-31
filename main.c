#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "main.h"

char *goingUp(char *string) {
    unsigned char *aux = (unsigned char *) string;

    while (*aux) {
        *aux = toupper((unsigned char) *aux);
        aux++;
    }

    return string;
}

operation getCurrentOperation(char *argVector[]) {
    return setCurrentOp(argVector[1]);
}

void canYouRunIt(int argCounter) {
    // 1: sem argumentos (só o comando em si)
    // >1 : tem argumentos
    if (argCounter == 1) {
        printf("Insira argumentos pfv.");
        exit(1);
    }
}

operation setCurrentOp(char *arg) {

    // Format arg to avoid strmp problems
    arg = goingUp(arg);
    printf("\n");
    int i = 0;
    for (i = 0; i < strlen(arg); ++i) {
        printf("%c ", arg[i]);
    }
    printf("%d", i);


    printf("\nRecebi: %s\n", arg);
    if (strcmp(arg, "MOSTRA") == 0) {
        printf("Operação: Mostra");
        return Mostra;
    }
    if (strcmp(arg, "CONTA") == 0) {
        printf("Operação: Conta");
        return Conta;
    }
    if (strcmp(arg, "APAGA") == 0) {
        printf("Operação: Apaga");
        return Apaga;
    }
    if (strcmp(arg, "INFORMA") == 0) {
        printf("Operação: Informa");
        return Informa;
    }
    if (strcmp(arg, "ACRESCENTA") == 0) {
        printf("Operação: Acrescenta");
        return Acrescenta;
    }
    if (strcmp(arg, "LISTA") == 0) {
        printf("Operação: Lista");
        return Lista;
    }
    if (strcmp(arg, "TERMINA") == 0) {
        printf("Operação: Termina");
        return Termina;
    }
    return Erro;
}

void run() {
    printf("\nExecutei o codigo bla bla bla\n");
}

char **copyArgcv(int argc, char *argv[]) {
    char **aux = malloc(sizeof(char) * MAXCOMANDO);
    for (int i = 0; i < argc; ++i) {
        aux[i] = argv[i];
    }
    return aux;
}

int main(int argc, char *argv[]) {

    // Copy the values of argc and argv to allow accessing memory beyond the bounds of argv
    int argCounter = argc;
    char **argVector = copyArgcv(argc, argv); // Comando mais longo tem 4 argumentos

    // Do we have arguments? If not exit.
    canYouRunIt(argCounter);

    // -------------------
    // We have arguments!
    // -------------------
    operation currentOp;
    do {
        printf("\nCount: %d", argCounter);
        for (int i = 0; i < argCounter; ++i) {
            printf("\nargv[%d]: %s", i, argVector[i]);
        }

        currentOp = getCurrentOperation(argVector);
        // > 0 : tudo OK!
        // = 0 : ERROR!
        // = 7 : Sair!

        if (currentOp > 0) {
            // Executa o comando
            run();
        } else if (currentOp == 7) {
            printf("Escolheu sair.");
            exit(0);
        } else {
            printf("Kaboom! (ERRO)");
        }

        // Lê o proximo comando
        char *comando = NULL;
        size_t len = 0;

        printf("\n%%");
        getline(&comando, &len, stdin);

        // Retira o \n do final
        comando[strlen(comando)-1] = '\0';
        printf("Comando:");
        puts(comando);

        char *token = strtok(comando, " ");

        int i = 1;
        while (token != NULL && i < MAXCOMANDO) {
            argVector[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        argCounter = i;
        argVector[i] = NULL; //argVector ends with NULL
        printf("\nnum args: %d", i);

    } while (currentOp != Termina);

    return -1;
}