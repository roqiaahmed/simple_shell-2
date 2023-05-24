#include "main.h"

void execute_command(char **args, int *status)
{
    pid_t pid = fork();

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
        wait(status);
    }
}

void read_commands(FILE *file, int *status)
{
    char *line = NULL;
    size_t line_size = 0;
    ssize_t line_len = 0;
    char *args[BUFFER_SIZE];

    while ((line_len = getline(&line, &line_size, file)) != -1)
    {
        char *token = strtok(line, " \t\n");
        int i = 0;

        while (token != NULL)
        {
            if (token[0] != '\0')
            {
                args[i] = token;
                i++;
            }
            token = strtok(NULL, " \t\n");

            // Skip over any remaining whitespace
            while (token != NULL && token[0] == '\0')
            {
                token = strtok(NULL, " \t\n");
            }
        }

        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
        {
            if (args[1] != NULL)
                *status = atoi(args[1]);
            free(line);
            fclose(file);
            exit(*status);
        }

        execute_command(args, status);

        line = NULL;
        line_size = 0;
    }

    free(line);
}

void read_commands_from_file(char *filename, int *status)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    read_commands(file, status);

    fclose(file);
}

void read_commands_from_stdin(int *status)
{
    char *line = NULL, *token;
    size_t line_size = 0;
    ssize_t line_len = 0;
    char *args[BUFFER_SIZE];
    int i;
    while (1)
    {
        printf("$ ");
        line_len = getline(&line, &line_size, stdin);

        if (line_len == -1)
        {
            if (feof(stdin))
            {
                putchar('\n');
                exit(*status);
            }
            else
            {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(line, " \t\n");
        i = 0;

        while (token != NULL)
        {
            if (token[0] != '\0')
            {
                args[i] = token;
                i++;
            }
            token = strtok(NULL, " \t\n");

            // Skip over any remaining whitespace
            while (token != NULL && token[0] == '\0')
            {
                token = strtok(NULL, " \t\n");
            }
        }

        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        if (strcmp(args[0], "exit") == 0)
        {
            if (args[1] != NULL)
                *status = atoi(args[1]);
            free(line);
            exit(*status);
        }

        execute_command(args, status);

        line = NULL;
        line_size = 0;
    }

    free(line);
}

/**
 * main - Simple shell program
 *
 * @argc: The number of command-line arguments
 * @argv: An array of command-line argument strings
 *
 * Return: Always returns 0
 */
int main(int argc, char **argv)
{
    int status = 0;

    if (argc > 1)
    {
        read_commands_from_file(argv[1], &status);
    }
    else
    {
        read_commands_from_stdin(&status);
    }

    return 0;
}
