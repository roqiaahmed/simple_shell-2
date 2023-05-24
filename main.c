#include "main.h"

/**
* read_command - Reads a line of input from the console
* @line: A pointer to a string buffer where the input line will be stored
* @len: A pointer to a size_t variable where the length of the input
* Return: num of character
*/
ssize_t read_command(char **line, size_t *len)
{
return (getline(line, len, stdin));
}
/**
* is_blank_or_comment - Checks if a line of input is blank or a comment
* @line: A pointer to a string containingthe input line to check
* Return: 0
*/
int is_blank_or_comment(const char *line)
{
return (line[0] == '\n' || line[0] == '#');
}
/**
* parse_command - Parses a line of input into an array of command-line
* @line: A pointer to a string containing the input line to parse
* @args: An array of strings where the parsed arguments will be stored
*/
void parse_command(char *line, char **args)
{
int i = 0;
args[i] = strtok(line, " \t\n");
if (args[i] == NULL)
{
return;
}
while (args[i] != NULL)
{
i++;
args[i] = strtok(NULL, " \t\n");
}
}
/**
* is_builtin - Checks if a command is a built-in shell command
* @command: A string containing the command to check
* Return: num of character
*/
int is_builtin(const char *command)
{
return (strcmp(command, "exit") == 0 || strcmp(command, "env") == 0 ||
strcmp(command, "setenv") == 0 || strcmp(command, "unsetenv") == 0);
}
/**
* main - The main function of the shell program
* Return: null
*/

int main(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
char *args[100];
int interactive = isatty(STDIN_FILENO);
while (1)
{
if (interactive)
{
printf("$ ");
}
nread = read_command(&line, &len);
if (nread == -1)
{
perror("getline");
exit(0);
}
if (is_blank_or_comment(line))
{
continue;
}
parse_command(line, args);
if (is_builtin(args[0]))
{
execute_builtin(args);
}
else
{
execute_command(args);
}
}
free(line);
exit(EXIT_SUCCESS);
}
