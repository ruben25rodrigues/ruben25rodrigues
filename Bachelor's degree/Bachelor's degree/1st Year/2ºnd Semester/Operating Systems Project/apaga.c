#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: apaga ficheiro\n");
        return 1;
    }

    if (unlink(argv[1]) < 0) {
        perror("Erro ao apagar");
        return 1;
    }

    return 0;
}
