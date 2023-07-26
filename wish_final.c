#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
char error_message[30] = "An error has occurred\n";

/*
Function that parses input
parameters: input to be parsed, commands to be executed, specified outfile, command quantity?
*/
void parse_input(char *input, char **commands, char *outfile)
{
    char *token;       // Current parse piece
    int arg_index = 0; // Command counter

    // Clear the commands and outfile arrays
    memset(commands, 0, sizeof(char *) * MAX_ARGS);
    memset(outfile, 0, sizeof(char) * MAX_INPUT_SIZE);

    // Tokenize the input
    token = strtok(input, " \t\n");

    while (token != NULL)
    { // While there's an input token to process

        /*  Output file instruction detected
         *   Should redirect stdout to the designated file
         *   If file does not exist, create it
         */
        if (strcmp(token, ">") == 0)
        { // redirect output to file

            token = strtok(NULL, " \t\n");
            strcpy(outfile, token);
            token = strtok(NULL, " \t\n");
            continue;
        }

        commands[arg_index] = strdup(token); // Log command
        arg_index++;

        token = strtok(NULL, " \t\n"); // Next token
    }
}

void execute_command(char **commands, char *outfile)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process

        if (outfile[0] != '\0')
        {
            // Redirect output to the specified file
            freopen(outfile, "a+", stdout);
        }

        execvp(commands[0], commands);
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    else
    {
        // Parent process
    }
}

int main(int argc, char *argv[])
{
    char input[MAX_INPUT_SIZE];
    char *commands[MAX_ARGS];
    char outfile[MAX_INPUT_SIZE];

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++) // BATCH MODE
        {
            FILE *file = fopen(argv[i], "r"); // Open the file to be executed
            if (file == NULL)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            char line[MAX_INPUT_SIZE];
            while (fgets(line, sizeof(line), file)) // Process lines in file
            {
                // Remove newline character at the end of the line, if present
                size_t len = strlen(line);
                if (line[len - 1] == '\n')
                    line[len - 1] = '\0';

                parse_input(line, commands, outfile);
                execute_command(commands, outfile);
            }

            fclose(file);
        }
    }
    else // INTERACTIVE MODE
    {

        while (1)
        {
            printf("wish> ");
            fflush(stdout);

            // Read user input
            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                break;
            }

            // Remove newline character
            input[strcspn(input, "\n")] = '\0';

            // Exit the loop if the user inputs "exit"
            if (strcmp(input, "exit") == 0)
            {
                break;
            }

            // Parallel command structure encapsulates the command logic
            char *begin = input;
            char *end = input;
            while (begin != NULL)
            {
                // Parse the input
                strsep(&end, "&");
                parse_input(begin, commands, outfile);

                if (strcmp(commands[0], "cd") == 0)
                {
                    // Change directory command
                    if (commands[1] != NULL)
                    {
                        chdir(commands[1]);
                    }
                    else
                    {
                        printf("Missing directory argument for cd command.\n");
                    }
                }
                else if (strcmp(commands[0], "path") == 0)
                {
                    /* Update path command
                    * TODO: Implement path update logic here
                    * If you're reading this I've given up on this. I may have designed too far without
                    * this to retroactively add it.
                    */
                }
                else
                {
                    // Execute command
                    execute_command(commands, outfile);
                }
                begin = end;
            }
        }
    }
    return 0;
}