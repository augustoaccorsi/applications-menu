#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


pid_t call_web_browser(char* URL);
char *get_process_status(pid_t pid);
void update_status(pid_t browser_pid);

#define clear() printf("\033[H\033[J")

int main(void){
	setlocale(LC_ALL, "PORTUGUESE");
	char choice = 0;
	char URL[50];
	char* status_browser = "NULL", status_editor[30] = "NULL", status_terminal[30] = "NULL";
	pid_t terminal_pid=NULL, text_pid=NULL, browser_pid=NULL;
	while(1){
	clear();
	printf("<<<< Applications Menu >>>\n");
	printf("1) Web Browser                        (%s, pid=%d)\n", status_browser, browser_pid);
	printf("2) Text Editor                        (%s, pid=%d)\n", status_editor, text_pid);
	printf("3) Terminal                           (%s, pid=%d)\n", status_terminal, terminal_pid);
	printf("4) Finalizar processo\n");
	printf("5) Quit\n");


	printf("Opção: ");

	scanf("%c", &choice);

	printf("escolhido: %c\n", choice);

	switch(choice){
		case 49:
			printf("Opção 1 selecionada: Web Browser\n\n");
			printf("Digite uma URL: ");
			scanf("%s", URL);
			browser_pid = call_web_browser(URL);
			status_browser = get_process_status(browser_pid);
			break;
		case 50: // text editor
			break;
		case 51: // terminal
			break;
		case 52: // finalizar processo
			break;
		case 53: // quit
			 exit(EXIT_FAILURE);
		case 54: // fazer o ctrl + c funcionar aqui
			status_browser = get_process_status(browser_pid);
			break;
	}
	}
}

pid_t call_web_browser(char* URL){
		pid_t child_pid = fork();
		if (child_pid == -1){
		    perror("fork error");
		}
		else if (child_pid == 0)
		{
			child_pid = getpid();
		    execlp("firefox", "firefox", URL, NULL);
		    perror("exec error");
		}
		printf("child pid -> %d", child_pid);
		return child_pid;
}

char *get_process_status(pid_t process_id){
	printf("status -> %d",process_id);
	int status;
	pid_t return_pid = waitpid(process_id, &status, WNOHANG); /* WNOHANG def'd in wait.h */
	if (return_pid == -1) {
		return "error";
	} else if (return_pid == 0) {
	    return "executando";
	} else if (return_pid == process_id) {
	    return "finalizado";
	}
}
