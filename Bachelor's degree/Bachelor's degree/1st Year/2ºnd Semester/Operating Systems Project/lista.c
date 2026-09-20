#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char *dir_name = (argc == 2) ? argv[1] : ".";

    DIR *dir = opendir(dir_name);
    if (!dir) {
        perror("Erro");
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR)
            printf("[DIR] %s\n", entry->d_name);
        else
            printf("[FILE] %s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
