#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int fd;
    char c;

    int linhas = 0, palavras = 0, caracteres = 0, in_word = 0;

    if (argc == 2) {
        // ler de ficheiro
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            perror("Erro");
            return 1;
        }
    } else {
        // ler de stdin (pipe)
        fd = STDIN_FILENO;
    }

    while (read(fd, &c, 1) > 0) {
        caracteres++;

        if (c == '\n')
            linhas++;

        if (isspace(c)) {
            in_word = 0;
        } else if (!in_word) {
            palavras++;
            in_word = 1;
        }
    }

    printf("Linhas: %d\nPalavras: %d\nCaracteres: %d\n",
           linhas, palavras, caracteres);

    if (argc == 2)
        close(fd);

    return 0;
}
