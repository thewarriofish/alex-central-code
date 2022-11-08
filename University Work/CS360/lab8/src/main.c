//Alexander Yu - CS360
//Lab 8: Jshell Pt. 2
//November 3rd, 2022

/*This lab focuses on the use of pipe as an extension of implementing a simulation of a shell. It can take all commands as well as process <, >, >> and |.
 * Most of this program is copied over from lab 7's.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MAXLEN 1001

int main(int argc, char** argv) {
	char prompt[MAXLEN], cmd[MAXLEN], str[MAXLEN];
	char * fname;
	char **command;
	int i, status, pid, fd, pipefd[2], pstatus;
	/*Sets up the prompt; '-' results in no prompt, no arguments results in 'jsh:'*/
    if(argc == 1)
	{
		snprintf(prompt, MAXLEN - 1, "jsh: ");
	}
	else if(strcmp(argv[1], "-") != 0) 
	{
		snprintf(prompt, MAXLEN - 1, "%s: ", argv[1]);	
	}
	else
	{
		strcpy(prompt, "");
	}
	/*Takes in input; prints the prompt again if stdin is empty*/
	strcpy(cmd, "\n");
	while(strcmp(cmd, "\n") == 0)
	{
		printf("%s", prompt);
		if(fgets(cmd, MAXLEN, stdin) == NULL)
		{
			return 0;
		}
	}
	strcpy(str, cmd);
	command = (char **) malloc(sizeof(char *)*MAXLEN);
	while(1)
	{
		i = 0;
		command[i] = strtok(cmd, " '\n'");
		/*Ends if EOF is detected or 'exit' is typed*/
		if(command[0] == NULL || strcmp(command[0], "exit") == 0)
		{
			break;
		}
		if((pid = fork()) == 0)
		{
			while(1)
			{
				i++;
				command[i] = strtok(NULL, " $'\n'");
				if(command[i] == NULL)
				{
					break;
				}
				/*Whenever program encounters &, <, > or >>, it does not include it in command*/
				else if(strcmp(command[i], "&") == 0)
				{
					command[i] = NULL;
					break;
				}
				/*For <, > and >>, finds the filename using strtok, opens and dups it*/
				else if(strcmp(command[i], "<") == 0)
				{
					fname = strtok(NULL, " $'\n'");
					fd = open(fname, O_RDONLY, 0644);
					if(dup2(fd, 0) != 0 || fd < 0)
					{
						perror(fname);
						exit(1);
					}
					close(fd);
					command[i] = NULL;
					i--;
				}
				else if(strcmp(command[i], ">") == 0)
				{
					fname = strtok(NULL, " $'\n'");
					fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if(dup2(fd, 1) != 1)
					{
						perror(fname);
						exit(1);
					} 
					close(fd);
					command[i] = NULL;
					i--;
				}
				else if(strcmp(command[i], ">>") == 0)
				{
					fname = strtok(NULL, " $'\n'");
					fd = open(fname, O_WRONLY | O_CREAT | O_APPEND, 0644);
					if(dup2(fd, 1) != 1)
					{
						perror(fname);
						exit(1);
					} 
					close(fd);
					command[i] = NULL;
					i--;
				}
				/*Code added for lab 8.*/
				else if(strcmp(command[i], "|") == 0)
				{
					pstatus = pipe(pipefd)i;
					/*If program is working with a child process, opens pipe for writing.*/
					if((pid = fork()) == 0)
					{
						if(dup2(pipefd[1], 1) != 1)
						{
							perror("dup2(pipefd[1])");
							exit(1);
						}
						close(pipefd[1]);
						close(pipefd[0]);
						command[i] = NULL;
						i--;
						break;
					}
					/*Otherwise, waits for child process to end if & is present*/
					if(str[strlen(str) - 2] != '&')
					{
						while(wait(&pstatus) != pid);
					}
					/*Opens pipe for reading*/
					if(dup2(pipefd[0], 0) != 0)
					{
						perror("dup2(pipefd[0])");
						exit(1);
					}
					close(pipefd[1]);
					close(pipefd[0]);
					i = -1; //resets char** for new command
				}
			}
			status = execvp(command[0], command);
			perror(command[0]);
			exit(1);
		}
		if(str[strlen(str) - 2] != '&')
		{
			while(wait(&status) != pid);
		}
		/*Prompts user to input a command*/
		strcpy(cmd, "\n");
		while(strcmp(cmd, "\n") == 0)
		{
			printf("%s", prompt);
			if(fgets(cmd, MAXLEN, stdin) == NULL)
			{
				free(command);
				return 0;
			}
		}
		strcpy(str, cmd);
	}
	free(command);
	return 0;
}
