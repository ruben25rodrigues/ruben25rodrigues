#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd;
    char *padrao;

    if (argc == 3) {
        // procurar em ficheiro
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            perror("Erro");
            return 1;
        }
        padrao = argv[2];

    } else if (argc == 2) {
        // procurar em stdin (pipe)
        fd = STDIN_FILENO;
        padrao = argv[1];

    } else {
        fprintf(stderr, "Uso: procura ficheiro padrao OU procura padrao\n");
        return 1;
    }

    char c;
    char linha[1024];
    int i = 0, linha_num = 1, encontrou = 0;

    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            linha[i] = '\0';

            if (strstr(linha, padrao)) {
                printf("Linha %d: %s\n", linha_num, linha);
                encontrou = 1;
            }

            i = 0;
            linha_num++;
        } else {
            linha[i++] = c;
        }
    }

    if (!encontrou) {
        printf("Nenhuma linha encontrada\n");
    }

    if (argc == 3)
        close(fd);

    return 0;
}
