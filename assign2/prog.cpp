/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Eric Hernandez,Chris Mead (TODO: your name)
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

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
 	int i = 0;   // TODO: implement this function
	char *token = strtok(command, " ");
	while(token!=NULL)
{
	args[i]=token;
	token = strtok(NULL," ");
	i++;
}	
	args[i]=NULL;

	return i;
}

// TODO: Add additional functions if you need
void redirect_input(char* filename)
{
	int fd=open(filename,O_RDONLY);
	dup2(fd,STDIN_FILENO);
	close(fd);

}
void redirect_output(char* filename)
{
	int fd = open(filename, O_CREAT| O_WRONLY| O_TRUNC|O_APPEND,0644);
	dup2(fd,STDOUT_FILENO);
	close(fd);
}
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */
    vector<string> history;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        // Read the input command
        fgets(command, MAX_LINE, stdin);
        //remove newline charater
        command[strcspn(command,"\n")]=0;

        if(strcmp(command,"!!")==0)
        {
            if(history.size()==0)
            {
                printf("No commands in history.\n");
                continue;
            }
            strcpy(command,history[history.size()-1].c_str());
            printf("%s\n",command);
        }
        else
        {
            history.push_back(command);
        }

        // Parse the input command
        int num_args = parse_command(command, args);
        int i=0;
        char *input_file=NULL;
        char *output_file=NULL;

        while(args[i]!=NULL)
        {
            if(strcmp(args[i],"<")==0)
            {
                input_file=args[i+1];
                args[i]=NULL;
            }
            else if(strcmp(args[i],">")==0)
            {
                output_file= args[i+1];
                args[i]=NULL;
            }
            i++;
        }

        //fork a child process
        pid_t pid=fork();

        if( pid < 0 )
        {
            fprintf(stderr,"Fork failed\n");
            return 1;
        }
        else if(pid ==0)
        { //child process
            if(input_file!=NULL)
            {
                redirect_input(input_file);
            }
            if(output_file !=NULL)
            {
                redirect_output(output_file);
            }

            int ret =execvp(args[0],args);
            if(ret==-1)
            {
                fprintf(stderr,"Invalid command\n");
                return 1;
            }
        }
        else
        {
            // Parent process
            if (args[num_args-1][0] != '&')
            {
                wait(NULL);
            }
        }

        // Reset input/output file pointers
        input_file = NULL;
        output_file = NULL;
    }
    return 0;
}

