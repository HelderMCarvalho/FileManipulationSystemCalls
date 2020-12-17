#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include "main.h"

void apagar(char **argv);

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
        fprintf(stdout, "Insira argumentos pfv.");
        exit(1);
    }
}

operation setCurrentOp(char *arg) {

    // Format arg to avoid strmp problems
    arg = goingUp(arg);
    fprintf(stdout, "\n");
    int i = 0;
    for (i = 0; i < strlen(arg); ++i) {
//        fprintf(stdout, "%c ", arg[i]);
    }
//    fprintf(stdout, "%d", i);


//    fprintf(stdout, "\nRecebi: %s\n", arg);
    if (strcmp(arg, "MOSTRA") == 0) {
        fprintf(stdout, "Operação: Mostra\n");
        return Mostra;
    }
    if (strcmp(arg, "CONTA") == 0) {
        fprintf(stdout, "Operação: Conta\n");
        return Conta;
    }
    if (strcmp(arg, "APAGA") == 0) {
        fprintf(stdout, "Operação: Apaga\n");
        return Apaga;
    }
    if (strcmp(arg, "INFORMA") == 0) {
        fprintf(stdout, "Operação: Informa\n");
        return Informa;
    }
    if (strcmp(arg, "ACRESCENTA") == 0) {
        fprintf(stdout, "Operação: Acrescenta\n");
        return Acrescenta;
    }
    if (strcmp(arg, "LISTA") == 0) {
        fprintf(stdout, "Operação: Lista\n");
        return Lista;
    }
    if (strcmp(arg, "TERMINA") == 0) {
        fprintf(stdout, "Operação: Termina\n");
        return Termina;
    }
    return Erro;
}

int run(int op, char **argv) {
//    fprintf(stdout, "\nExecutei o codigo bla bla bla\n");

    int retstatus;
    int pid = fork();

    if (pid == -1) {
        perror("erro no fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) // filho
    {
        // Child
//        printf("Child has x = %d\n", getpid());
        switch (op) {
            case 0:
                fprintf(stderr, "Kaboom! (ERRO)");
                exit(1);
                break; //ERRO
            case 1:
                return mostra(argv, false);
                break;//Mostra
            case 2:
                printf("Numero de chars: %d", mostra(argv, true));
                return true;
                break;//Conta
            case 3:
                apagar(argv);

                break;//Apaga
            case 4:
                break;//Informa
            case 5:
                break;//Acrescenta
            case 6:
                break;//Lista
            case 7:
                fprintf(stdout, "Escolheu sair.");
                exit(0);
                break; //EXIT
            default:
                fprintf(stderr, "Kaboom! (ERRO)");
                exit(1);
                break;
        }
        perror("erro no exec");
        exit(EXIT_FAILURE);
    }

    wait(&retstatus);
    return WEXITSTATUS(retstatus);
}

void apagar(char **argv) {
    printf("Correu bem? (0): %d",unlink(argv[2]));
}

int mostra(char **argv, bool devolveChar) {

    if (!argv[2]) {
        printf("Não existe argumento para o caminho do ficheiro.");
        return false;
    }
    char *filePath = argv[2];
    int fd, leitura, numC = 0;
    char buffer[1025];

    fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        perror("erro na abertura do ficheiro");
        return false;
    }

    // ficheiro disponivel no descritor fd
    leitura = read(fd, buffer, 1024);

    while (leitura > 0) {
        buffer[leitura] = '\0';
        if (devolveChar) {
            numC += leitura;
        } else {
            puts(buffer);
        }
        leitura = read(fd, buffer, 1024);
    }

    // -1 houve erro
    if (leitura == -1) {
        perror("Erro na leitura do ficheiro: ");
        return false;
    }
    close(fd);
    return numC;
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

    // "Endless LOOP"
    while (1) {
//        fprintf(stdout, "\nCount: %d", argCounter);
//        for (int i = 0; i < argCounter; ++i) {
//            fprintf(stdout, "\nargv[%d]: %s", i, argVector[i]);
//        }

        currentOp = getCurrentOperation(argVector);
        // > 0 : tudo OK!
        // = 0 : ERROR!
        // = 7 : Sair!

//        fprintf(stdout, "NUM OP: %d", currentOp);


        // Executa o comando
        run(currentOp, argVector);

        // Lê o proximo comando
        char *comando = NULL;
        size_t len = 0;

        fprintf(stdout, "\n%%");
        getline(&comando, &len, stdin);

        // Retira o \n do final
        comando[strlen(comando) - 1] = '\0';
//        fprintf(stdout, "Comando:");
//        puts(comando);

        char *token = strtok(comando, " ");

        int i = 1;
        while (token != NULL && i < MAXCOMANDO) {
            argVector[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        argCounter = i;
        argVector[i] = NULL; //argVector ends with NULL
//        fprintf(stdout, "\nnum args: %d", i);
    }
}