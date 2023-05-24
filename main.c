#include "main.h"
/**
 * main - Simple shell program
 *
 * Return: Always returns 0
 */
int main(void)
{
char *line = NULL, *token;
size_t line_size = 0;
ssize_t line_len = 0;
char *args[BUFFER_SIZE];
int status = 0, i;
pid_t pid;
extern char **environ;
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
token = strtok(line, " \t\n");
i = 0;
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
exit(status);
}
else if (strcmp(args[0], "env") == 0)
{
char **env = environ;
while (*env)
{
printf("%s\n", *env);
env++;
}
status = 0;
}
else if (strcmp(args[0], "setenv") == 0)
{
if (args[1] == NULL || args[2] == NULL)
printf("Usage: setenv <variable> <value>\n");
else
setenv(args[1], args[2], 1);
status = 0;
}
else if (strcmp(args[0], "unsetenv") == 0)
{
if (args[1] == NULL)
printf("Usage: unsetenv <variable>\n");
else
unsetenv(args[1]);
status = 0;
}
else
{
pid = fork();
if (pid < 0)
{
perror("fork");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
if (execvp(args[0], args) == -1)
{
perror(args[0]);
exit(EXIT_FAILURE);
}
}
else
{
wait(&status);
}
}
}
free(line);
return (0);
}
