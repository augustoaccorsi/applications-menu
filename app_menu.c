#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t call_web_browser(char * URL);
pid_t call_terminal();
char * get_process_status(pid_t pid);
void show_all_process();
void init_sigaction(struct sigaction* conf_sinal);
void sinal_chegou(int signum, siginfo_t* info, void* vp);

#define clear() printf("\033[H\033[J")

int main(void) {
  setlocale(LC_ALL, "PORTUGUESE");
  char choice = 0;
  char URL[50];
  char txt_name[50];
  char * status_browser  = "NULL";
  char * status_editor   = "NULL";
  char * status_terminal = "NULL";
  pid_t terminal_pid = NULL, text_pid = NULL, browser_pid = NULL;
  struct sigaction conf_sinal;

  init_sigaction(&conf_sinal);
  conf_sinal.sa_handler = &sinal_chegou;

  while (1) {
    clear();
    if(browser_pid != NULL){
    	status_browser = get_process_status(browser_pid);
    }
    if(text_pid != NULL){
    	status_editor = get_process_status(text_pid);
    }
    if(terminal_pid != NULL){
    	status_terminal = get_process_status(terminal_pid);
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
		printf("Opção 2 selecionada: Text Editor\n\n");
		printf("Digite o nome do arquivo: ");
		scanf("%s", txt_name);
		text_pid = call_text_editor(txt_name);
		status_editor = get_process_status(text_pid);
		break;
      case 51: // terminal
          printf("Opção 3 selecionada: Terminal\n\n");
          terminal_pid = call_terminal(URL);
          status_terminal = get_process_status(terminal_pid);
          break;
      case 52: // finalizar processo
    	  show_all_process(browser_pid, text_pid, terminal_pid);
        break;
      case 53: // quit
        exit(EXIT_FAILURE);
      case 54: // fazer o ctrl + c funcionar aqui
        status_browser = get_process_status(browser_pid);
        status_editor = get_process_status(text_pid);
        status_terminal = get_process_status(terminal_pid);
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

pid_t call_text_editor(char * txt_name) {
  pid_t child_pid = fork();
  if (child_pid == -1) {
    perror("fork error");
  } else if (child_pid == 0) {
    child_pid = getpid();
    execlp("gedit", "gedit", txt_name, NULL);
    perror("exec error");
  }
  printf("child pid -> %d", child_pid);
  return child_pid;
}

pid_t call_terminal() {
  pid_t child_pid = fork();
  if (child_pid == -1) {
    perror("fork error");
  } else if (child_pid == 0) {
    child_pid = getpid();
    execlp("xterm", "xterm", NULL);
    perror("exec error");
  }
  printf("child pid -> %d", child_pid);
  return child_pid;
}

char * get_process_status(pid_t process_id) {
  int status = 0;
  pid_t return_pid = waitpid(process_id, & status, WNOHANG); /* WNOHANG def'd in wait.h */
  if (return_pid == -1) {
    return "error";
  } else if (return_pid == 0) {
    return "executando";
  } else if (return_pid == process_id) {
    return "finalizado";
  }
}

void show_all_process(int *browser_pid, int *text_pid, int *terminal_pid) {
	int count = 0;
	char choice = 0;
	int break_loop = 0;

	struct sigaction conf_sinal;

	init_sigaction(&conf_sinal);
	conf_sinal.sa_handler = &sinal_chegou;

	clear();

	while(break_loop == 0){

		printf("Processos em execução:\n\n");

		if (strcmp(get_process_status(*browser_pid),"executando") == 0) {
			printf("1. Web Browser, pid: %d\n",*browser_pid);
			count++;
		}
		if (strcmp(get_process_status(*text_pid),"executando") == 0) {
			printf("2. Text Editor, pid: %d\n",*text_pid);
			count++;
		}
		if (strcmp(get_process_status(*terminal_pid),"executando") == 0) {
			printf("3. Terminal, pid: %d\n",*terminal_pid);
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
					if(kill(*browser_pid, SIGTERM)==-1){
		        	    perror("erro no kill");
		        	}
		        	break_loop = 1;
		          break;
		        case 50: // text editor
		        	if(kill(*text_pid, SIGTERM)==-1){
		        		perror("erro no kill");
		        	}
		        	break_loop = 1;
		          break;
		        case 51: // terminal
		        	if(kill(*terminal_pid, SIGTERM)==-1){
		        		perror("erro no kill");
		        	}
		        	break_loop = 1;
		          break;
			}

			clear();
		}
	}
}

  void init_sigaction(struct sigaction* p_conf_sinal){
  	memset(p_conf_sinal, 0, sizeof(struct sigaction));
  	p_conf_sinal->sa_flags = 0;
  	p_conf_sinal->sa_restorer = NULL;
  	sigemptyset(&p_conf_sinal->sa_mask);
  	p_conf_sinal->sa_handler = NULL;
  }

  void sinal_chegou(int signum, siginfo_t* info, void* vp){
  	int status = 0;
  	int retstatus = 0;

  	if(signum == SIGCHLD){
  		alarm(0);
  		waitpid(info->si_pid, &status, WNOHANG);
  		retstatus = WEXITSTATUS(status);
  		printf("%d", retstatus);
  		sleep(5);
  	}
  }
