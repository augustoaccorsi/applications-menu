#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>

int main(void){
	setlocale(LC_ALL, "PORTUGUESE");
	int choice;
	while(1){
	
	printf("Digite a opção desejada\n\n");
	printf("1) Web Browser\n");
    printf("2) Text Editor\n");
    printf("3) Terminal\n");
    printf("4) Finalizar processo\n");
    printf("5) Quit\n");
	
	choice = getch();
	
	printf("escolhido : %d\n\n", choice);
	
	switch(choice){
		case 49: // web browser
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
