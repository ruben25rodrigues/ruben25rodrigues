
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: mostra ficheiro\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Erro a abrir ficheiro");
        return 1;
    }

    char buffer[1024];
    ssize_t n;

    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }

    close(fd);
    return 0;
}
