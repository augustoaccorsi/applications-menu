#define _POSIX_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

pid_t call_web_browser(char * URL);
pid_t call_text_editor(char * txt_name);
char * get_process_status(pid_t pid);
void show_all_process();
void init_sigaction(struct sigaction* conf_sinal);
void sinal_chegou(int signum);

struct sigaction conf_sinal;

#define clear() printf("\033[H\033[J")

pid_t terminal_pid = NULL, text_pid = NULL, browser_pid = NULL;
char * status_browser  = "NULL";
char * status_editor   = "NULL";
char * status_terminal = "NULL";

int main(void) {
  setlocale(LC_ALL, "PORTUGUESE");
  char choice = 0;
  char URL[50];
  char txt_name[50];

  init_sigaction(&conf_sinal);
  sigaction(SIGINT, &conf_sinal, NULL);
  sigaction(SIGCHLD, &conf_sinal, NULL);

  while (1) {
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

    switch (choice) {
      case 49:
        printf("Opção 1 selecionada: Web Browser\n\n");
        printf("Digite uma URL: ");
        scanf("%s", URL);
        browser_pid = call_web_browser(URL);
        status_browser = get_process_status(browser_pid);
        break;
      case 50:
    	printf("Opção 2 selecionada: Text Editor\n\n");
    	printf("Digite o nome do arquivo: ");
    	scanf("%s", txt_name);
    	text_pid = call_text_editor(txt_name);
    	status_editor = get_process_status(text_pid);
    	break;
      case 51: // terminal
        break;
      case 52: // finalizar processo
    	show_all_process(&browser_pid, &text_pid, &terminal_pid);
        break;
      case 53: // quit
        exit(EXIT_FAILURE);
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

char * get_process_status(pid_t process_id) {
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

void show_all_process(int *browser_pid, int *text_pid, int *terminal_pid) {
	int count = 0;
	char choice = 0;
	int break_loop = 0;

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
					status_browser = kill(*browser_pid, SIGTERM);
					if(status_browser == -1){
		        	    perror("erro no kill");
		        	}
		        	break_loop = 1;
		          break;
		        case 50: // text editor
		        	status_editor = kill(*text_pid, SIGTERM);
		        	if(status_editor == -1){
		        		perror("erro no kill");
		        	}
		        	break_loop = 1;
		          break;
		        case 51: // terminal
		        	status_terminal = kill(*terminal_pid, SIGTERM);
		        	if(status_terminal == -1){
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
	memset(p_conf_sinal, 0, sizeof(&p_conf_sinal));
	sigemptyset(&p_conf_sinal->sa_mask);
	p_conf_sinal->sa_handler = sinal_chegou;
}

void sinal_chegou(int signum){
	pid_t pid;
	int status;
	int retstatus;

	if(signum == SIGINT || signum == SIGCHLD){
		clear();
		alarm(0);
		pid = waitpid(-1, &status, WNOHANG);
		retstatus = WEXITSTATUS(status);
		if(pid == browser_pid){
			browser_pid = NULL;
			if(status_browser == 0){
				status_browser = "concluído";
			}
			else if(status_browser == -1){
				status_browser = "falhou";
			}
		}
		else if(pid == text_pid){
			text_pid = NULL;
			if(status_editor == 0){
				status_editor = "concluído";
			}
			else if(status_editor == -1){
				status_editor = "falhou";
			}
		}
		else if(pid == terminal_pid){
			terminal_pid = NULL;
			if(status_terminal == 0){
				status_terminal = "concluído";
			}
			else if(status_terminal == -1){
				status_terminal = "falhou";
			}
		}
	}
	else if(signum == SIGABRT){
		clear();
		alarm(0);
		pid = waitpid(-1, &status, WNOHANG);
		retstatus = WEXITSTATUS(status);
		if(pid == terminal_pid){
			terminal_pid = NULL;
			status_browser = "abortado";
		}
		else if(pid == text_pid){
			text_pid = NULL;
			status_editor = "abortado";
		}
		else if(pid == browser_pid){
			browser_pid = NULL;
			status_terminal = "abortado";
		}
	}
}
