#include "main.h"
/**
*execute_builtin - Executes a built-in shell command
*@args: An array of strings containing the command and its arguments
*/
void execute_builtin(char **args)
{
    if (strcmp(args[0], "exit") == 0)
    {
        int status = 0;
        if (args[1] != NULL)
        {
            status = atoi(args[1]);
        }
        exit(status);
    }
    else if (strcmp(args[0], "env") == 0)
    {
        char **env = environ;
        while (*env != NULL)
        {
            printf("%s\n", *env);
            env++;
        }
    }
    else if (strcmp(args[0], "setenv") == 0)
    {
        if (args[1] != NULL && args[2] != NULL)
        {
            setenv(args[1], args[2], 1);
        }
        else
        {
            printf("Usage: setenv VAR VALUE\n");
        }
    }
    else if (strcmp(args[0], "unsetenv") == 0)
    {
        if (args[1] != NULL)
        {
            unsetenv(args[1]);
        }
        else
        {
            printf("Usage: unsetenv VAR\n");
        }
    }
}
/**
*execute_command - Executes a command in a new child process
*@args: An array of strings containing the command and its arguments
*/
void execute_command(char **args)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        // Child process
        execvp(args[0], args);
        perror(args[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exitstatus = WEXITSTATUS(status);
            printf("Program exited with status %d\n", exitstatus);
        }
    }
}
