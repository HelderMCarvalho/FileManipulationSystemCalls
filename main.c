#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include "main.h"


//#define ANSI_COLOR_RED     "\x1b[31m"
//#define ANSI_COLOR_RESET   "\x1b[0m"

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

char *getOp(operation op) {
    switch (op) {
        case Erro:
            return "Erro";
        case Mostra:
            return "Mostra";
        case Conta:
            return "Conta";
        case Apaga:
            return "Apaga";
        case Informa:
            return "Informa";
        case Acrescenta:
            return "Acrescenta";
        case Lista:
            return "Lista";
        case Termina:
            return "Termina";
        default:
            return "Sem OP";
    }
}

operation setCurrentOp(char *arg) {

    // Format arg to avoid strmp problems
    arg = goingUp(arg);
//    fprintf(stdout, "\n");
//    int i;
//    for (i = 0; i < strlen(arg); ++i) {
//        fprintf(stdout, "%c ", arg[i]);
//    }
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

char **copyArgcv(int argc, char *argv[]) {
    char **aux;
    aux = malloc(MAXCOMANDO * sizeof(char *));
    for (int i = 0; i < argc; ++i) {
        aux[i] = strdup(argv[i]);
    }
    return aux;
}

void informa(char *filePath) {

    struct stat properties;

    if (stat(filePath, &properties) == -1) {
        perror("Stat");
        exit(EXIT_FAILURE);
    }

    printf("\nTipo Ficheiro: ");

    switch (properties.st_mode & S_IFMT) {
        case S_IFBLK:
            printf("Dispositivo orientado ao Bloco\n");
            break;
        case S_IFCHR:
            printf("Dispositivo orientado ao Caracter\n");
            break;
        case S_IFDIR:
            printf("Directoria\n");
            break;
        case S_IFIFO:
            printf("FIFO/pipe\n");
            break;
        case S_IFLNK:
            printf("Link\n");
            break;
        case S_IFREG:
            printf("Ficheiro normal\n");
            break;
        case S_IFSOCK:
            printf("Socket\n");
            break;
        default:
            printf("Desconhecido\n");
            break;
    }

    printf("I-node:        %ld\n", (long) properties.st_ino);
    printf("Dono:          %s\n", getpwuid(properties.st_uid)->pw_name);
}

void acrescenta(char *origem, char *destino) {
    int fd1, fd2;
    char c;
    fd1 = open(origem, O_RDONLY);
    fd2 = open(destino, O_APPEND | O_WRONLY);
    if (fd1 < 0 || fd2 < 0) {
        perror("Acrescenta");
        exit(EXIT_FAILURE);
    }

    while (read(fd1, &c, sizeof(c))) {
        write(fd2, &c, sizeof(c));
    }
    close(fd1);
    close(fd2);
}

int run(int op, char **argv) {

    bool termina = false;
    int child_pid = -1, child_status = -1;

    child_pid = fork();
    switch (child_pid) {
        case -1:
            // Codigo se o Fork der erro.
            perror("Fork Error");
            exit(EXIT_FAILURE);
        case 0:
            // Codigo do processo filho
            switch (op) {
                case 0:
                    fprintf(stderr, "Kaboom! (ERRO)");
                    exit(30);
                case 1:
                    //Mostra
                    mostra(argv, false);
                    break;
                case 2:
                    //Conta
                    printf("Numero de chars: %d", mostra(argv, true));
                    break;
                case 3:
                    //Apaga
                    apagar(argv[2]);
                    break;
                case 4:
                    informa(argv[2]);
                    break;//Informa
                case 5:
                    acrescenta(argv[2], argv[3]);
                    break;//Acrescenta
                case 6:
                    lista(argv[2]);
                    break;//Lista
                case 7:
                    fprintf(stdout, "Escolheu sair.");
                    exit(30);
                default:
                    exit(30);
            }
            exit(EXIT_SUCCESS);
        default:
            // Codigo do processo pai
            wait(&child_status);
            // Numero escolhido para terminar o programa
            if (WEXITSTATUS(child_status) == 30)
                exit(0);
            return WEXITSTATUS(child_status);
    }
}

bool apagar(char *filepath) {
    if (!unlink(filepath)) {
        printf("\n\t-> %s\n", filepath);
    } else {
        perror("File doesnt exist");
        exit(errno);
    }
}

char *getCurrentDirectory() {
    char *res = NULL;
    char *buff = malloc(sizeof(char *) * 1000);
    getcwd(buff, 1000);
    res = strdup(buff);
    free(buff);
    return res;
}

bool lista(char *filepath) {
    if (!filepath) {
        filepath = getCurrentDirectory();
    }
    DIR *dir = opendir(filepath);
    // If dir exist
    if (dir) {
        printf("\n\tPasta atual: %s\n", filepath);
        printf("\t%-40s%-15s\n", "Nome", "Tipo");

        struct dirent *dirReader;
        while ((dirReader = readdir(dir)) != NULL) {
            if (!strcmp(dirReader->d_name, ".") || !strcmp(dirReader->d_name, ".."))
                continue;
            switch (dirReader->d_type) {
                case DT_DIR:
                    printf("\t%-40s%-15s\n", dirReader->d_name, "Directoria");

                    break;
                case DT_REG:
                    printf("\t%-40s%-15s\n", dirReader->d_name, "Ficheiro Normal");
                    break;

                case DT_UNKNOWN:
                    printf("\t%-40s%-15s\n", dirReader->d_name, "Desconhecido");
                    break;
            }
        }
        closedir(dir);
    } else {
        perror("Lista");
        exit(EXIT_FAILURE);
    }
}

int mostra(char **argv, bool devolveChar) {


    char *filePath = argv[2];
    int fd, leitura, numC = 0;
    char buffer[1025];

    fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        perror("Open File");
        exit(EXIT_FAILURE);
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
        perror("Read File");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return numC;
}

//void printError(operation currentOp, int errorNum) {
//    fprintf(stdout, ANSI_COLOR_RED"%s: %s\n"ANSI_COLOR_RESET,getOp(currentOp),strerror( errorNum ));
//
//    printf("Pressione uma tecla para continuar.\n\n");
//    int c = getchar();
//    system("clear");
//}

int main(int argc, char *argv[]) {

    // 1º Passo

    // Copy the values of argc and argv to allow accessing memory beyond the bounds of argv
    int argCounter = argc;
    char **argVector = copyArgcv(argc, argv); // Comando mais longo tem 4 argumentos

    // Do we have arguments? If not exit.
    canYouRunIt(argCounter);

    // -------------------
    // We have arguments!
    // -------------------
    operation currentOp;



    // 2º Passo
    // "Endless LOOP"
    while (1) {


////         Print all arguments
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
        printf("Terminou o comando %s com o código: %d", getOp(currentOp), run(currentOp, argVector));

//        if(errno){
//            printError(currentOp,errno);
//        }

        // Lê o proximo comando
        char *comando = NULL;
        size_t len = 0;

        printf("\n\n%%");
        getline(&comando, &len, stdin);

        // Retira o \n do final
        comando[strlen(comando) - 1] = '\0';

        char *token = strtok(comando, " ");

        int i = 1;
        while (token != NULL && i < MAXCOMANDO) {
            argVector[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        argCounter = i;
        argVector[i] = NULL;
    }
}