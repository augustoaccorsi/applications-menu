#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <unistd.h>
 
void call_web_browser(char* URL);

int main(void){
	setlocale(LC_ALL, "PORTUGUESE");
	int choice;
	char URL[50];
	while(1){
	
	printf("Digite a opÃ§Ã£o desejada\n\n");
	printf("1) Web Browser\n");
    printf("2) Text Editor\n");
    printf("3) Terminal\n");
    printf("4) Finalizar processo\n");
    printf("5) Quit\n");
	
	choice = getch();
	
	printf("escolhido : %d\n\n", choice);
	
	switch(choice){
		case 49: 
			printf("Opção 1 selecionada: Web Browser\n\n");
			printf("Digite uma URL: ");
			scanf("%s", URL);
			call_web_browser(URL);
			break;
		case 50: // text editor
			break;
		case 51: // terminal
			break;
		case 52: // finalizar processo
			break;
		case 53: // quit
			return(0);
	}
	
	system("cls");
	}
}

void call_web_browser(char* URL){
	execlp("C:/Program Files/Mozilla Firefox/firefox.exe", "www.google.com", NULL);
	perror("Exec com erro");
	getch();
}
