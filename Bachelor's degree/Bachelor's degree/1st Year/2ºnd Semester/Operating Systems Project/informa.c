#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>

void print_perms(mode_t mode) {
    char perms[10];
    perms[0] = (mode & S_IRUSR) ? 'r' : '-';
    perms[1] = (mode & S_IWUSR) ? 'w' : '-';
    perms[2] = (mode & S_IXUSR) ? 'x' : '-';
    perms[3] = (mode & S_IRGRP) ? 'r' : '-';
    perms[4] = (mode & S_IWGRP) ? 'w' : '-';
    perms[5] = (mode & S_IXGRP) ? 'x' : '-';
    perms[6] = (mode & S_IROTH) ? 'r' : '-';
    perms[7] = (mode & S_IWOTH) ? 'w' : '-';
    perms[8] = (mode & S_IXOTH) ? 'x' : '-';
    perms[9] = '\0';
    printf("Permissões: %s\n", perms);
}

int main(int argc, char *argv[]) {
    struct stat sb;

    if (argc != 2) {
        fprintf(stderr, "Uso: informa ficheiro\n");
        return 1;
    }

    if (stat(argv[1], &sb) < 0) {
        perror("Erro");
        return 1;
    }

    printf("Inode: %lu\n", sb.st_ino);
    printf("Tamanho: %ld bytes\n", sb.st_size);

    struct passwd *pw = getpwuid(sb.st_uid);
    printf("Dono: %s\n", pw->pw_name);

    print_perms(sb.st_mode);

    printf("Última modificação: %s", ctime(&sb.st_mtime));

    return 0;
}
