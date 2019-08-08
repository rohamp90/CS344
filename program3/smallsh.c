#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define INPUTLENGTH 2048

//Function Prototypes
void catchSIGINT(int signo);
void prompt();
void exitCmd();		
void cdCmd();
void userInputAdv();

void catchSIGINT(int signo)
{
	char* message = "SIGINT. Use CTRL-Z to Stop.\n";
	write(STDOUT_FILENO, message, 28);
}


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

	//Lines commented out below are from userinput_adv.c
		// printf("Allocated %zu bytes for the %d chars you entered.\n", bufferSize, numCharsEntered);
		// printf("Here is the raw entered line: \"%s\"\n", lineEntered);
		lineEntered[strcspn(lineEntered, "\n")] = '\0'; // Remove the trailing \n that getline adds
		// printf("Here is the cleaned line: \"%s\"\n", lineEntered);
	//******************************************************************************


		if(strcmp(lineEntered, "exit") == 0) //User Entered Exit into prompt
		{
			exitCmd();
		}
		else if(strcmp(lineEntered, "cd") == 0)	//User Entered cd into prompt
		{
			cdCmd();
		}
        
        fflush( stdout ); //added fflush 

		free(lineEntered); // Free the memory allocated by getline() or else memory leak
		lineEntered = NULL;
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
void cdCmd()
{
	printf("Entered into cdCmd Function\n");
}

void userInputAdv()
{
//******************************************************************************
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
	
	while(1)
	{
		// Get input from the user
		while(1)	
		{
			printf("Enter in a line of text (CTRL-C to exit):");
			numCharsEntered = getline(&lineEntered, &bufferSize, stdin); // Get a line from the user
			if (numCharsEntered == -1)
				clearerr(stdin);
			else
				break; // Exit the loop - we've got input
		}

		printf("Allocated %zu bytes for the %d chars you entered.\n", bufferSize, numCharsEntered);
		printf("Here is the raw entered line: \"%s\"\n", lineEntered);
		lineEntered[strcspn(lineEntered, "\n")] = '\0'; // Remove the trailing \n that getline adds
		printf("Here is the cleaned line: \"%s\"\n", lineEntered);
        
        fflush( stdout ); //added fflush 

		free(lineEntered); // Free the memory allocated by getline() or else memory leak
		lineEntered = NULL;
	}
//****************************************************
//************* From userinput_adv.c; From lecture notes 3.3 ********************
}

 int main()
 {

    prompt();


     return 0;
 }