
/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Eric Hernandez & Christopher Mead
 * @brief This is the main function of a simple UNIX Shell. You may add
 * additional functions in this file for your implementation
 * @version 0.1
 */


//
// * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
// * @version 0.1
// */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

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
bool in=false;// global command if there is a  <
bool out=false;//global command if there is a >
bool ampersand=false; //global command if there is a & present
char* file;// name of file being created
bool piping=false;
int pipe_index = -1; // index of pipe character in args array

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{
    int count = 0; // will be used to count the amount of arguments in the command given by user
    char *store = strtok(command, " ,\n"); // beginning of the parsing. Puts the first argument into store
    
    while (store != NULL) // runs while the character array value put into store, is NOT NULL
    {
        if (*store == '<') // input redirection
        {
            file = strtok(NULL, " ,\n"); // char* input_file will hold the token given from strtok(the next argument)
            in = true; // set in to true
        }
        else if (*store == '>') // output redirection
        {
            file = strtok(NULL, " ,\n"); // char* output_file will hold the token given from strtok(the next argument)
            out = true; // set out to true
        }
        else if (*store == '|') // pipe redirection
        {
            args[count++] = NULL; // terminate the first command with NULL
            piping = true; // set piping to true
            pipe_index = count; // save the index of the pipe character
        }
        else // special cases have been dealt with
        {
            args[count++] = store; // stores the captured argument into the args array
        }
        store = strtok(NULL, " ,\n"); // sets store to the next argument given by command
    } // end of while loop

    if (strcmp(args[count - 1], "&") == 0) // if the last argument in the array is ampersand
    {
        args[count - 1] = NULL; // remove the ampersand by making it equal to NULL
        ampersand = true; // set ampersand to true;
    }
    else
    {
        if (store != NULL)
        {
            args[count++] = store; // set last argument to the command
        }
        args[count] = NULL; // else making the last equal to NULL to be used for execvp
        ampersand = false; // make sure that ampersand is false
    }
    return count; // return count
}

/***************************************
 this function takes in the parsed arguements and forks them into parent and child

char* args[] is a character array in which each entry points to a parsed token
******************************************/
void forking(char* args[])
{
    if(piping) //if piping is found and is true
    {
        int fd[2]; //creates an array to hold file descriptors for read and write ends of the pipe
        pid_t pid1, pid2; //create variables to hold process ID'S

        if (pipe(fd) == -1) //creatrre a pipe and check for errors
        {
            fprintf(stderr, "Pipe failed");
            return;
        }

        pid1 = fork(); //fork the first child process

        if (pid1 < 0) //chjeck for errors when forking
        {
            fprintf(stderr, "Fork failed");
            return;
        } 
        else if (pid1 == 0)
        {
            close(fd[0]); //close the read end of the pipe
            dup2(fd[1], STDOUT_FILENO); //redirect standard output to the write end of the pipe
            close(fd[1]); //close the write end of pipe

            if (execvp(args[0], args) < 0)  //execute first command
            {
                printf("Command not found\n");
                exit(1);
            }
        } 
        else //parent processs
        {
            pid2 = fork(); //for the second child processs

            if (pid2 < 0) //check for errors in forking
            {
                fprintf(stderr, "Fork failed");
                return;
            } 
            else if (pid2 == 0)//child process
{
    close(fd[1]); //close thje read end of pipe
    dup2(fd[0], STDIN_FILENO); //redirevt to write
    close(fd[0]); //close the write

    // Find index of the second command in args
    int i = 0;
    while (args[i] != NULL)
    {
        i++;
    }
    i++;

    if (execvp(args[i], &args[i]) < 0) //execute the second command
    {
        printf("Command not found\n");
        exit(1);
    }
}
            else//parent process
            {
                close(fd[0]);//close both ends of pipe
                close(fd[1]);

                if (!ampersand)//if ampersand not detected
                {
                    waitpid(pid1, NULL, 0);
                    waitpid(pid2, NULL, 0);
                }
                else//run in background
                {
                    ;
                }
            }
        }
    }
    else //no pipe detected
    {
        pid_t pid; //initiling the process ID
        pid = fork(); //forks the pid

        if (pid < 0) //error happened
        {
            printf("Fork Failure");
        } 
        else if (pid == 0) //child process
        {
            if(in)
            {
                int fd = open(file, O_RDONLY);//open a file reading only
                dup2(fd,STDIN_FILENO); //writing to standard in file
                close(fd); //close
            }
            else if(out)
            {
                int fd=open(file,O_CREAT |O_TRUNC|O_WRONLY);//creates a new file with the name file
                dup2(fd,STDOUT_FILENO); //writing to standard out file the value of open file
                close(fd);
            }

            if (execvp(args[0], args) < 0) //process the arguments stored in the args[] array
            {
                printf("Command not found\n"); 
                exit(1);
            }
        } 
        else
        {
            if (!ampersand) //if ampersand is not present, wait for child process to complete
            {
                wait(NULL);//waits
            }
            else
            {
                ;//ampersand is present, dont wait for child process to complete
            }
        }
    }
}




/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];  // the command that was entered
    char *args[MAX_LINE / 2 + 1]; //parsed out command line arguments
    int should_run = 1; //flag to determin when to exit program
    vector<string> history; //history vector for storing past history

    while (should_run)
    {
        printf("osh>"); //prints out osh for starting point
        fflush(stdout);

        fgets(command, MAX_LINE, stdin);

        

        string copy = command; //gets command for copying into history

        for(int i=0; i<strlen(command); i++)
        {
            if((command[i] == '&') && (command[i-1] != ' ')) // i=symbol, index before doesnt have space, so add space
            {
                command[i] = ' ';
                command[i+1] = '&';
                command[i+2] = '\0';
                ampersand=true;//sets ampersand to true if the & is found
            }
        }

        int num_args = parse_command(command, args); //calls the parse command to seperate the commandsa

        if(strcmp(command, "exit") == 0)
        {
            should_run = 0;
        }
        else if (strcmp(command, "!!") == 0) //user has requested to duplicate last request
        {
            if(history.empty())//history vector is empty
            {
                printf("No command history found");
            }
            else
            {
                strcpy(command, history.back().c_str());//copies the last command to the vector
                printf("%s",command);    
                int num_args = parse_command(command,args); //parses command into args
                forking(args); //runs command through forking process
            }
        }
        else
        {
            history.push_back(copy); //pushes the command into the vertex
            forking(args);
        }
        piping=false;
        cout << endl;
    }
    return 0;
}

