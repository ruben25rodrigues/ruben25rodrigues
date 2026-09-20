#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 20

void parse(char *linha, char **args) {
    int i = 0;
    args[i] = strtok(linha, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

int main() {
    char linha[1024];
    char *args[MAX_ARGS];

    while (1) {
        printf("%% ");
        fflush(stdout);  // IMPORTANTE

        if (fgets(linha, sizeof(linha), stdin) == NULL)
            break;

        linha[strcspn(linha, "\n")] = 0;

        if (strcmp(linha, "termina") == 0)
            break;

        parse(linha, args);

        // procurar pipe
        int pipe_pos = -1;
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "|") == 0) {
                pipe_pos = i;
                break;
            }
        }

        if (pipe_pos != -1) {
            // -------- PIPE --------

            args[pipe_pos] = NULL;

            char *cmd1[MAX_ARGS];
            char *cmd2[MAX_ARGS];

            int i = 0;
            while (args[i] != NULL) {
                cmd1[i] = args[i];
                i++;
            }
            cmd1[i] = NULL;

            int j = 0;
            i = pipe_pos + 1;
            while (args[i] != NULL) {
                cmd2[j++] = args[i++];
            }
            cmd2[j] = NULL;

            int fd[2];
            if (pipe(fd) < 0) {
                perror("Erro pipe");
                continue;
            }

            pid_t p1 = fork();
            if (p1 == 0) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);

                execvp(cmd1[0], cmd1);
                perror("Erro exec cmd1");
                exit(1);
            }

            pid_t p2 = fork();
            if (p2 == 0) {
                dup2(fd[0], STDIN_FILENO);
                close(fd[1]);
                close(fd[0]);

                execvp(cmd2[0], cmd2);
                perror("Erro exec cmd2");
                exit(1);
            }

            // PAI (MUITO IMPORTANTE!)
            close(fd[0]);
            close(fd[1]);

            int status1, status2;
            waitpid(p1, &status1, 0);
            waitpid(p2, &status2, 0);

            printf("Terminou pipe com código %d\n", WEXITSTATUS(status2));

        } else {
            // -------- SEM PIPE --------

            pid_t pid = fork();

            if (pid == 0) {
                for (int i = 0; args[i] != NULL; i++) {
                    if (strcmp(args[i], ">") == 0) {
                        int fd = open(args[i+1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        args[i] = NULL;
                        break;
                    }

                    if (strcmp(args[i], "<") == 0) {
                        int fd = open(args[i+1], O_RDONLY);
                        dup2(fd, STDIN_FILENO);
                        close(fd);
                        args[i] = NULL;
                        break;
                    }
                }

                execvp(args[0], args);
                perror("Erro exec");
                exit(1);
            } else {
                int status;
                waitpid(pid, &status, 0);

                printf("Terminou comando %s com código %d\n",
                       args[0], WEXITSTATUS(status));
            }
        }
    }

    return 0;
}
