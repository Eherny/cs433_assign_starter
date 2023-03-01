/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author ??? (TODO: your name)
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

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
bool ampersand=false; //implemented to hold a value in the case a command has & as a arguement
/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command
 * @param args
 * @return int
 */
int parse_command(char command[], char *args[])
{
    int count = 0; //will be used to count the amount of arguments in the command given by user
    char *store = strtok(command, " ,\n");//beginning of the parsing. Puts the first argument into store
    while (store != NULL)//runs while the character array value put into store, is NOT NULL
    {
        if (*store == '<') // input redirection
        {
            char *input_file = strtok(NULL, " ,\n"); //char* input_file will hold the token given from strtok(the next argument)
            int fd = open(input_file, O_RDONLY);//creates a new file with the name of the token given to input_file
            if (fd < 0)//an error occured when making/opening a file
            {
                perror("Error opening input file");
            }
            else//file was created or opened without a problem
            {
                dup2(fd, STDIN_FILENO); //copy the contents of the arguement into this file
                close(fd);//close the file
            }
        }
        else if (*store == '>') // output redirection
        {
            char *output_file = strtok(NULL, " ,\n"); //char* output_file will hold the token given from strtok(the next argument)
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);//opens a file which we will get arguments from
            if (fd < 0) //an error occured when opening a file
            {
                perror("Error opening output file");
            }
            else //file was created or opened without a problem
            {
                dup2(fd, STDOUT_FILENO); //copy the contents of the file to act as an argument
                close(fd); //closes the file
            }
        }
        else //special cases have been dealt with
        {
            args[count++] = store;//stores the captured argument into the args array
        }
        store = strtok(NULL, " ,\n");//sets store to the next argument given by command
    }//end of while loop
	if(strcmp(args[count - 1], "&") == 0) //if the last argument in the array is ampersand
    {
      args[count-1]=NULL; //remove the ampersand by making it equal to NULL
	    ampersand=true; //set ampersand to true;
    }
  else
    {
    	 args[count] = NULL;//else making the last equal to NULL to be used for execvp
	     ampersand=false;//make sure that ampersand is false
    }
  return count;//return count
}

/***************************************
 this function takes in the parsed arguements and forks them into parent and child

char* args[] is a character array in which each entry points to a parsed token
******************************************/
void forking(char* args[])
{
    pid_t pid;//initializing the process IDs
    pid = fork();//forks the pid 

    if (pid < 0) // error has happened
    {
      printf("Fork Failure");
    } 
    else if (pid == 0) // child process
    {
      execvp(args[0], args);//processes the arguments stored in the args[] array
      printf("Command not found\n");//the given arguments aren't of those native to the system or those which we created
    } 
    else
    {
        if (!ampersand) // if ampersand is not present, wait for child process to complete
        {
          wait(NULL);//waits for child to process
        }
        else // ampersand is present, don't wait for child process to complete
        {
          ;
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
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */
    vector<string> history;
    // TODO: Add additional variables for the implementation.

  
    while (should_run)
    {
        printf("osh>"); //prints 
        fflush(stdout);
        // Read the input command
        fgets(command, MAX_LINE, stdin);

        string copy = command; //copies the command so it can be put into history
	for(int i=0; i<strlen(command); i++)
      {
        if((command[i] == '&') && (command[i-1] != ' ')) // i=symbol, index before doesnt have space, so add space
        {
          command[i] = ' '; //
          command[i+1] = '&'; //
          command[i+2] = '\0'; //
        }
      }
        // Parse the input command
        int num_args = parse_command(command, args); //calls the parse command to separate the command into args
	 ampersand= false;
      
        // TODO: Add your code for the implementation
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
      
       if(strcmp(command, "exit") == 0)
          {
            should_run = 0;
          }
      else if (strcmp(command, "!!") == 0)//user has requested to duplicate last request
        {
          if(history.empty())//history vector is empty
            {
              printf("No command history found");
            }
          else //history
            {
              strcpy(command, history.back().c_str()); //copies the last command in the history to the 'char command'1
              printf("%s",command);    
              int num_args = parse_command(command,args); //parses command into args 
              forking(args);//runs command through the forking process.
            }
        }
      else
        {
          history.push_back(copy);//pushes the command into the vertex   
          forking(args);
        }
      cout << endl; //makes the program look pretty
    }
  return 0;
}
