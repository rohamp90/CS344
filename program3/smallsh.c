#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <signal.h>
#include <fcntl.h>

#define INPUTLENGTH 2048

//Function Prototypes
void catchSIGINT(int signo);
void prompt();
void exitCmd();		
void cdCmd(char * word);
void statusCmd(int status);



void catchSIGINT(int signo)
{
	char* message = "SIGINT. Use CTRL-Z to Stop.\n";
	write(STDOUT_FILENO, message, 28);
}

//Commentting out the prompt function for now. going to work with the main function 1:30pm 8/8/19

void prompt()
{
	//From userinput_adv.c; From lecture notes 3.3
    struct sigaction SIGINT_action = {0};
	SIGINT_action.sa_handler = catchSIGINT;
	sigfillset(&SIGINT_action.sa_mask);
	//SIGINT_action.sa_flags = SA_RESTART;
	sigaction(SIGINT, &SIGINT_action, NULL);

	int numCharsEntered = -5; // How many chars we entered
	int currChar = -5; // Tracks where we are when we print out every char
	size_t bufferSize = 0; // Holds how large the allocated buffer is
	char* lineEntered = NULL; // Points to a buffer allocated by getline() that holds our entered string + \n + \0

	int exitStatus = 0;
	
	while(1)
	{
		// Get input from the user
		while(1)	
		{
			printf(":");
			numCharsEntered = getline(&lineEntered, &bufferSize, stdin); // Get a line from the user
			if (numCharsEntered == -1)
				clearerr(stdin);
			else
				break; // Exit the loop - we've got input
		}

		lineEntered[strcspn(lineEntered, "\n")] = '\0'; // Remove the trailing \n that getline adds

		char* str1 = NULL;				//Will use to split up lineEntered string
		str1 = strdup(lineEntered); 	//copy lineEntered to str1
		char* token1 = strtok(str1, " ");	//split str1 into tokens delimiter is whitespace
		

		if(strcmp(lineEntered, "exit") == 0) //User Entered Exit into prompt
		{
			exitCmd();
		}
		else if(strcmp(str1, "cd") == 0)	//User Entered cd into prompt; Using str1 instead of lineEntered b/c we just want the first part of the argument
		{
			
			if (strcmp(lineEntered, "cd") == 0)
			{
				chdir(getenv("HOME"));	//getenv gets the value of the "HOME" variable; changing current dir to home dir b/c there is no path after cd
			}
			else
			{
				token1 = strtok(NULL, "\0");	//move the token pointer to the next "token" whatever is after the "cd"
				chdir(token1);				//change the directory to whatever is after the cd
			}

		}
		else if(strcmp(lineEntered, "status") == 0) // User Entered Status into prompt
		{
			statusCmd(exitStatus);
		}
									//If # is entered then nothing happens
		else if((str1[0] == 35))	//ASCII value for # == 35 
		{
			//Intentionally left blank
		}
		else //User enters something else like a bash command
		{
			// system(lineEntered);	//use the system() function

			//From Lecture 3.4
			pid_t spawnPid = -5;
			int childExitStatus = -5;
			spawnPid = fork();
			switch (spawnPid)
			{
				case -1: {perror("Hull Breach!\n"); exit(1); break;}
				case 0:
				{
					// printf("lineEntered: %s\nlineEntered length: %d\n", lineEntered, strlen(lineEntered));	//test print
			
					token1 = strtok(NULL, "\0");		//tokenized str1 and using token to get to the "arugments/parameters" of whatever command the user entered

					if(token1 != NULL)					//Checking if there is more than 1 argument
					{
						char* str2 = NULL;					//Will use to split up lineEntered string to get arguments
						str2 = strdup(token1); 				//copy token1 to str2
						char* token2 = strtok(str2, " ");	//split str2 into tokens delimiter is whitespace

						token2 = strtok(NULL, "\0");		//tokenized str2 and using token to get to the "arugments/parameters" of whatever command the user entered
															//str2 be whatever comes after a whitespace like "<" or ">"; 
						// printf("\nstr2: %s\nstr2 length: %d\n", str2, strlen(str2));	//test print
						//Checking for redirection 
						if (strcmp(str2, ">") == 0)
						{	
						
							printf("I know this is a >\n");		//test print
							
							// /* ***************************************
							char* str3 = NULL;					//Will use to split up lineEntered string to get arguments
							str3 = strdup(token2); 				//copy token2 to str3
							char* token3 = strtok(str3, " ");	//split str3 into tokens delimiter is whitespace
							token3 = strtok(NULL, "\0");		//tokenized str3 and using token to get to the "arugments/parameters" of whatever command the user entered
																//str3 be whatever comes after a "<" or ">"; 
							printf("\nstr2: %s\nstr2 length: %d\n", str2, strlen(str2));	//test print
							printf("\nstr3: %s\nstr3 length: %d\n", str3, strlen(str3));	//test print
							
							//Taken from 3.4 lecture notes
							int sourceFD, targetFD, result;
							
							sourceFD = open(str1, O_RDONLY);
							if (sourceFD == -1) {perror("source open()"); exit(1);}
							printf("sourceFD == %d\n", sourceFD); // Written to terminal
							targetFD = open(str3, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
							if (targetFD == -1) { perror("target open()"); exit(1); } 
							printf("targetFD == %d\n", targetFD); // Written to terminal

							result = dup2(sourceFD, 0); 
							if (result == -1) { perror("source dup2()"); exit(2); } 
							result = dup2(targetFD, 1); 
							if (result == -1) { perror("target dup2()"); exit(2); }
							//  *************************************** */

							
							exit(2);
						}
						else if (strcmp(str2, "<") == 0)
						{
							printf("I know this is a <\n");
							exitStatus = 11;
							exit(11);
							// printf("\nstr2: %s\nstr2 length: %d\n", str2, strlen(str2));	//test print
						}
						else
						{
							//using execlp
							execlp(str1, str1, token1, NULL);	//execlp("ls", "ls", "-a", NULL); example from 3.4 lecture notes for "ls -a"

						}
							perror("CHILD: exec failure!\n");
							exit(2);
							break;
					}
					else
					{
						execlp(str1, str1, token1, NULL);	//uncomment after done so exec command works
						perror("CHILD: exec failure!\n");
						exit(2);
						break;
					}
					
					//Taken from 3.4 lecture notes
				// *******************************************************************
					// int sourceFD, targetFD, result;

					// sourceFD = open(stdin, O_RDONLY);
					// if (sourceFD == -1) {perror("source open()"); exit(1);}

					// printf("sourceFD == %d\n", sourceFD); // Written to terminal

					// targetFD = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); 
					// if (targetFD == -1) { perror("target open()"); exit(1); } 
					// printf("targetFD == %d\n", targetFD); // Written to terminal

					// result = dup2(sourceFD, 0); 
					// if (result == -1) { perror("source dup2()"); exit(2); } 
					// result = dup2(targetFD, 1); 
					// if (result == -1) { perror("target dup2()"); exit(2); }
				// *******************************************************************


				}
				default:
				{
					pid_t actualPid = waitpid(spawnPid, &childExitStatus, 0);
					break;
				}
			}

		}
        
        fflush( stdout ); //added fflush 

		free(lineEntered); // Free the memory allocated by getline() or else memory leak
		lineEntered = NULL;

		free(str1);
		str1 = NULL;
	}
}

/******************************************************************************
	The exit command exits your shell. It takes no arguments. 
	When this command is run, your shell must kill any other processes or jobs 
	that your shell has started before it terminates itself.
 *******************************************************************************/

// 8/7/19 4:30pm - only for 1 process as of now
void exitCmd()
{
	int pid = getpid(); 	//get pid of processess

	kill(pid, SIGTERM);		//use kill and SIGTERM to terminate process
}

/******************************************************************************
	The cd command changes the working directory of your shell. 
	By itself - with no arguments - it changes to the directory specified in the HOME environment variable 
		(not to the location where smallsh was executed from, 
		unless your shell executable is located in the HOME directory, 
		in which case these are the same). This command can also take one argument: 
		the path of a directory to change to. Your cd command should support both 
		absolute and relative paths. When smallsh terminates, 
		the original shell it was launched from will still be in its 
		original working directory, despite your use of chdir() in smallsh. 
		Your shell's working directory begins in whatever directory your shell's 
		executible was launched from.
 *******************************************************************************/
void cdCmd(char * word)
{
	//might not need this function
	//put logic inside if/else statment in prompt function
}

/******************************************************************************
	The status command prints out either the exit status or the terminating signal 
	of the last foreground process (not both, processes killed by signals do not have exit statuses!) 
	ran by your shell. If this command is run before any foreground command is run, then it should simply 
	return the exit status 0. These three built-in shell commands do not count as foreground processes for 
	the purposes of this built-in command - i.e., status should ignore built-in commands.
 *******************************************************************************/
void statusCmd(int status)
{
	//Currently having trouble getting to display correct status
	if(WIFEXITED(status))	//taken from lecture 3.1
	{
		int exitStatus = WEXITSTATUS(status);         
        printf("exit value %d\n", exitStatus); 
	}
	else
	{
		printf("terminated by signal %d\n", status);
	}	
}


 int main()
 {

	//From userinput_adv.c; From lecture notes 3.3
    struct sigaction SIGINT_action = {0};
	SIGINT_action.sa_handler = catchSIGINT;
	sigfillset(&SIGINT_action.sa_mask);
	//SIGINT_action.sa_flags = SA_RESTART;
	sigaction(SIGINT, &SIGINT_action, NULL);

	int numCharsEntered = -5; // How many chars we entered
	int currChar = -5; // Tracks where we are when we print out every char
	size_t bufferSize = 0; // Holds how large the allocated buffer is
	char* lineEntered = NULL; // Points to a buffer allocated by getline() that holds our entered string + \n + \0

	int exitStatus = 0;
	
	while(1)
	{
		// Get input from the user
		while(1)	
		{
			printf(":");
			numCharsEntered = getline(&lineEntered, &bufferSize, stdin); // Get a line from the user
			if (numCharsEntered == -1)
				clearerr(stdin);
			else
				break; // Exit the loop - we've got input
		}

		lineEntered[strcspn(lineEntered, "\n")] = '\0'; // Remove the trailing \n that getline adds

		char* str1 = NULL;				//Will use to split up lineEntered string
		str1 = strdup(lineEntered); 	//copy lineEntered to str1
		char* token1 = strtok(str1, " ");	//split str1 into tokens delimiter is whitespace
		

		if(strcmp(lineEntered, "exit") == 0) //User Entered Exit into prompt
		{
			exitCmd();
		}
		else if(strcmp(str1, "cd") == 0)	//User Entered cd into prompt; Using str1 instead of lineEntered b/c we just want the first part of the argument
		{
			
			if (strcmp(lineEntered, "cd") == 0)
			{
				chdir(getenv("HOME"));	//getenv gets the value of the "HOME" variable; changing current dir to home dir b/c there is no path after cd
			}
			else
			{
				token1 = strtok(NULL, "\0");	//move the token pointer to the next "token" whatever is after the "cd"
				chdir(token1);				//change the directory to whatever is after the cd
			}

		}
		else if(strcmp(lineEntered, "status") == 0) // User Entered Status into prompt
		{
			statusCmd(exitStatus);
		}
									//If # is entered then nothing happens
		else if((str1[0] == 35))	//ASCII value for # == 35 
		{
			//Intentionally left blank
		}
		else //User enters something else like a bash command
		{
			/*
			// //Trying pipe lecture 3.4
			// int r, pipeFDs[2]; 
			// char completeMessage[512], readBuffer[10]; 
			// pid_t spawnpid;

			// if (pipe(pipeFDs) == -1) { perror("Hull Breach!"); exit(1); } // Create the pipe with error check

			// spawnpid = fork(); // Fork the child, which will write into the pipe 
			
			// switch (spawnpid) 
			// { 
			// 	case 0: // Child
			// 		close(pipeFDs[0]); // close the input file descriptor
			// 		write(pipeFDs[1], "CHILD: Hi parent!@@", 19); // Write the entire string into the pipe
			// 		exit(0); break; // Terminate the child 
				
			// 	default: // Parent
			// 		close(pipeFDs[1]); // close output file descriptor
			// 		memset(completeMessage, '\0', sizeof(completeMessage)); // Clear the buffer
			// 		while (strstr(completeMessage, "@@") == NULL) // As long as we haven't found the terminal... 
			// 		{
			// 			memset(readBuffer, '\0', sizeof(readBuffer)); // Clear the buffer
			// 			r = read(pipeFDs[0], readBuffer, sizeof(readBuffer) - 1); // Get the next chunk
			// 			strcat(completeMessage, readBuffer); // Add that chunk to what we have so far
			// 			printf("PARENT: Message received from child: \"%s\", total: \"%s\"\n", readBuffer, completeMessage);
			// 			if (r == -1) { printf("r == -1\n"); break; } // Check for errors
			// 			if (r == 0) { printf("r == 0\n"); break; } 
			// 		} 
			// 		int terminalLocation = strstr(completeMessage, "@@") - completeMessage; // Where is the terminal 
			// 		completeMessage[terminalLocation] = '\0'; // End the string early to wipe out the terminal 
			// 		printf("PARENT: Complete string: \"%s\"\n", completeMessage); 
			// 		break;
			// }
			 */

		// Trying out the dup2 
		
			//From Lecture 3.4
			pid_t spawnPid = -5;
			int childExitStatus = -5;
			spawnPid = fork();
			switch (spawnPid)
			{
				case -1: {perror("Hull Breach!\n"); exit(1); break;}
				case 0:
				{
					// printf("lineEntered: %s\nlineEntered length: %d\n", lineEntered, strlen(lineEntered));	//test print
			
					token1 = strtok(NULL, "\0");		//tokenized str1 and using token to get to the "arugments/parameters" of whatever command the user entered

					if(token1 != NULL)					//Checking if there is more than 1 argument
					{
						char* str2 = NULL;					//Will use to split up lineEntered string to get arguments
						str2 = strdup(token1); 				//copy token1 to str2
						char* token2 = strtok(str2, " ");	//split str2 into tokens delimiter is whitespace

						token2 = strtok(NULL, "\0");		//tokenized str2 and using token to get to the "arugments/parameters" of whatever command the user entered
															//str2 be whatever comes after a whitespace like "<" or ">"; 
						// printf("\nstr2: %s\nstr2 length: %d\n", str2, strlen(str2));	//test print
						//Checking for redirection 
						if (strcmp(str2, ">") == 0)
						{	
							char* str3 = NULL;					//Will use to split up lineEntered string to get arguments
							str3 = strdup(token2); 				//copy token2 to str3
							char* token3 = strtok(str3, " ");	//split str3 into tokens delimiter is whitespace
							token3 = strtok(NULL, "\0");		//tokenized str3 and using token to get to the "arugments/parameters" of whatever command the user entered
																//str3 be whatever comes after a "<" or ">"; 
							// printf("\nstr2: %s\nstr2 length: %d\n", str2, strlen(str2));	//test print
							// printf("\nstr3: %s\nstr3 length: %d\n", str3, strlen(str3));	//test print
							
							//Taken from 3.4 lecture notes
							int sourceFD, targetFD, result;
							char * inputFile = NULL;
							char * outputFile = NULL;
							inputFile = strdup(str1);
							outputFile = strdup(str3);
							fflush( stdout );
							targetFD = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
							if (targetFD == -1) { perror("target open()"); exit(1); } 
							// printf("targetFD == %d\n", targetFD); // Written to terminal

							result = dup2(sourceFD, 0); 
							if (result == -1) { perror("source dup2()"); exit(2); } 
							result = dup2(targetFD, 1); 
							if (result == -1) { perror("target dup2()"); exit(2); }
							execlp(str1, str1, NULL);
							exit(2);
						}
						else if (strcmp(str2, "<") == 0)
						{
							char* str3 = NULL;					//Will use to split up lineEntered string to get arguments
							str3 = strdup(token2); 				//copy token2 to str3
							char* token3 = strtok(str3, " ");	//split str3 into tokens delimiter is whitespace
							token3 = strtok(NULL, "\0");		//tokenized str3 and using token to get to the "arugments/parameters" of whatever command the user entered
																//str3 be whatever comes after a "<" or ">"; 
							int sourceFD, targetFD, result;
							char * inputFile = NULL;
							char * outputFile = NULL;
							inputFile = strdup(str1);
							outputFile = strdup(str3);

							// printf("inputFile: %s\noutputFile: %s\n", inputFile, outputFile);
							fflush( stdout );
							// sourceFD = open(inputFile, O_RDONLY);
							// if (sourceFD == -1) {perror("source open()"); exit(1);}
							// printf("sourceFD == %d\n", sourceFD); // Written to terminal

							targetFD = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
							if (targetFD == -1) { perror("target open()"); exit(1); } 
							printf("targetFD == %d\n", targetFD); // Written to terminal

							fflush( stdout );

							result = dup2(sourceFD, 0); 
							if (result == -1) { perror("source dup2()"); exit(2); } 
							result = dup2(targetFD, 1); 
							if (result == -1) { perror("target dup2()"); exit(2); }

							char * args[3] = {str1, str2, str3};
							execvp(args[0], args);
							// execlp(str1, str1, token1, str3, NULL);

						}
						else
						{
							//using execlp
							execlp(str1, str1, token1, NULL);	//execlp("ls", "ls", "-a", NULL); example from 3.4 lecture notes for "ls -a"
						}
							perror("CHILD: exec failure!\n");
							exit(2);
							break;
					}
					else
					{
						execlp(str1, str1, token1, NULL);	//uncomment after done so exec command works
						perror("CHILD: exec failure!\n");
						exit(2);
						break;
					}
				}
				default:
				{
					pid_t actualPid = waitpid(spawnPid, &childExitStatus, 0);
					break;
				}
			} 
		}
        
        fflush( stdout ); //added fflush 

		free(lineEntered); // Free the memory allocated by getline() or else memory leak
		lineEntered = NULL;

		free(str1);
		str1 = NULL;
	}

     return 0;
 }