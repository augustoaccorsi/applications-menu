#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


pid_t call_web_browser(char * URL);
char * get_process_status(pid_t pid);
void show_all_process();

#define clear() printf("\033[H\033[J")

int main(void) {
  setlocale(LC_ALL, "PORTUGUESE");
  char choice = 0;
  char URL[50];
  char * status_browser = "NULL", status_editor[30] = "NULL", status_terminal[30] = "NULL";
  pid_t terminal_pid = NULL, text_pid = NULL, browser_pid = NULL;
  while (1) {
    clear();
    if(browser_pid != NULL){
    	status_browser = get_process_status(browser_pid);
    }
    printf("<<<< Applications Menu >>>\n");
    printf("1) Web Browser                        (%s, pid=%d)\n", status_browser, browser_pid);
    printf("2) Text Editor                        (%s, pid=%d)\n", status_editor, text_pid);
    printf("3) Terminal                           (%s, pid=%d)\n", status_terminal, terminal_pid);
    printf("4) Finalizar processo\n");
    printf("5) Quit\n");


    printf("Opção: ");

    scanf("%c", &choice);

    printf("escolhido: %c\n", choice);

    switch (choice) {
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
    	  show_all_process(browser_pid, terminal_pid, text_pid);
        break;
      case 53: // quit
        exit(EXIT_FAILURE);
      case 54: // fazer o ctrl + c funcionar aqui
        status_browser = get_process_status(browser_pid);
        break;
    }
  }
}

pid_t call_web_browser(char * URL) {
  pid_t child_pid = fork();
  if (child_pid == -1) {
    perror("fork error");
  } else if (child_pid == 0) {
    child_pid = getpid();
    execlp("firefox", "firefox", URL, NULL);
    perror("exec error");
  }
  printf("child pid -> %d", child_pid);
  return child_pid;
}

char * get_process_status(pid_t process_id) {
  int status;
  pid_t return_pid = waitpid(process_id, & status, WNOHANG); /* WNOHANG def'd in wait.h */
  if (return_pid == -1) {
    return "error";
  } else if (return_pid == 0) {
    return "executando";
  } else if (return_pid == process_id) {
    return "finalizado";
  }
}

void show_all_process(pid_t browser_pid, pid_t terminal_pid, pid_t text_pid) {
  clear();
  int count = 0;

  char choice = 0;

  int break_loop = 0;

  while(break_loop == 0){

  printf("Processos em execução:\n\n");

  if (strcmp(get_process_status(browser_pid),"executando") == 0) {
    printf("1. Web Browser, pid: %d\n",browser_pid);
    count++;
  }
  if (strcmp(get_process_status(terminal_pid),"executando") == 0) {
    printf("2. Terminal, pid: %d\n",terminal_pid);
    count++;
  }
  if (strcmp(get_process_status(text_pid),"executando") == 0) {
    printf("2. Text Editor, pid: %d\n",text_pid);
    count++;
  }

  if(count == 0){
	  printf("Não há processos em execução, aguarde...\n");
  	break_loop = 1;
	  sleep(3);
  }
  else{

		  printf("Digite o processo que você quer finalizar:");
		  	  scanf("%c", &choice);

		  switch (choice) {
		        case 49: //web browser
		        	if(kill(browser_pid, SIGTERM)==-1){
		        	    perror("erro no kill");
		        	}
		        	break_loop = 1;
		          break;
		        case 50: // text editor
		        	if(kill(terminal_pid, SIGTERM)==-1){
		        			        	    perror("erro no kill");
		        			        	}
		        	break_loop = 1;
		          break;
		        case 51: // terminal
		        	if(kill(text_pid, SIGTERM)==-1){
		        			        	    perror("erro no kill");
		        			        	}
		        	break_loop = 1;
		          break;
		      }

		  clear();
	  }
  }


}

