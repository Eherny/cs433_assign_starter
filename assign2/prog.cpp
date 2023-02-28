#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{
    int count = 0;
    char *store = strtok(command, " ,\n");
    while (store != NULL)
    {
        if (*store == '<') // input redirection
        {
            char *input_file = strtok(NULL, " ,\n");
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                perror("Error opening input file");
                // return -1; // uncomment this line to terminate the program on error
            }
            else
            {
                dup2(fd, STDIN_FILENO); // redirect stdin to the input file
                close(fd);
            }
        }
        else if (*store == '>') // output redirection
        {
            char *output_file = strtok(NULL, " ,\n");
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if (fd < 0)
            {
                perror("Error opening output file");
                // return -1; // uncomment this line to terminate the program on error
            }
            else
            {
                dup2(fd, STDOUT_FILENO); // redirect stdout to the output file
                close(fd);
            }
        }
        else
        {
            args[count++] = store;
        }
        store = strtok(NULL, " ,\n");
    }
    args[count] = NULL;
    return count;
}



// TODO: Add additional functions if you need


/***************************************
 
******************************************/
void forking(char *args[])
{
    pid_t pid;
    pid = fork();

    if (pid < 0) // error has happened
        {
            printf("Fork Failure");
	
        } 
    else if (pid == 0) // child process
        {
            execvp(args[0], args);
            printf("Command not found\n");
	
        } 
    else
        {   //this is the parent process waiting for the child
            wait(NULL);
        }
}


/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[]) {
    char command[MAX_LINE];
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    vector<string> history;
  
    while (should_run) {
        printf("osh>");
        fflush(stdout);
        fgets(command, MAX_LINE, stdin);
        string copy = command;
        int num_args = parse_command(command, args);
      
        if(strcmp(command, "exit") == 0) {
            should_run = 0;
        } else if (strcmp(command, "!!") == 0) {
            if (history.empty()) {
                printf("No command history found.\n");
            } else {
                strcpy(command, history.back().c_str());
                int num_args = parse_command(command,args);
                forking(args);
            }
        } else {
            history.push_back(copy);
            forking(args);
        }

        // Handle the "&" character to run a command in the background
        bool background = false;
        if (num_args > 0 && strcmp(args[num_args - 1], "&") == 0) {
            background = true;
            args[num_args - 1] = NULL;
        }

        if (!background && strcmp(command, "!!") != 0) {
            wait(NULL);
        }
    }
    return 0;
}
