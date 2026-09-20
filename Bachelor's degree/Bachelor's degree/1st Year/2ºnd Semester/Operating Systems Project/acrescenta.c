#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: acrescenta origem destino\n");
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_WRONLY | O_APPEND);

    if (src < 0 || dest < 0) {
        fprintf(stderr, "Erro ao abrir ficheiros\n");
        return 1;
    }

    char buffer[1024];
    ssize_t n;

    while ((n = read(src, buffer, sizeof(buffer))) > 0) {
        write(dest, buffer, n);
    }

    close(src);
    close(dest);
    return 0;
}
