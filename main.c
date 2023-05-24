#include "main.h"


/**
 * main - Simple shell program
 *
 * Return: Always returns 0
 */
int main(void)
{
    char *line = NULL;
    size_t line_size = 0;
    ssize_t line_len = 0;
    char *args[BUFFER_SIZE];
    int status = 0;
    pid_t pid;

    while (1)
    {
        printf("$ ");
        line_len = getline(&line, &line_size, stdin);

        if (line_len == -1)
        {
            if (feof(stdin))
            {
                putchar('\n');
                exit(status);
            }
            else
            {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        char *token = strtok(line, " \t\n");
        int i = 0;

        while (token != NULL)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " \t\n");
        }

        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
        {
            if (args[1] != NULL)
               status = atoi(args[1]);
            free(line);
            exit(status);
        }

        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(&status);
        }

        free(line);
        line = NULL;
        line_size = 0;
    }

    return 0;
}
