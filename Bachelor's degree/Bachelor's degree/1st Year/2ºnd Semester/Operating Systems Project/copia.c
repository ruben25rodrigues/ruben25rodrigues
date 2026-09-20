#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: copia origem destino\n");
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src < 0) {
        perror("Ficheiro origem não existe");
        return 1;
    }

    int dest = open(argv[2], O_CREAT | O_EXCL | O_WRONLY, 0644);
    if (dest < 0) {
        perror("Ficheiro destino já existe");
        close(src);
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
