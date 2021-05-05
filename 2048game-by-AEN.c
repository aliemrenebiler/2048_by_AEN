#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char start_screen(char);
void create_random(int [4][4]);
void print_board(int*, int*, int [4][4]);
void print_row(int, int [4][4]);
char get_command(char, int, int);

// Kontroller
int stuck_control(int [4][4]); // Oyun takýlýrsa
int win_control(int [4][4]); // Oyun takýlýrsa

void copy_board(int*, int*, int*, int*, int [4][4], int [4][4]);
void r_undo(int*, int*, int*, int*, int [4][4], int [4][4]);
void x_restart(int*, int*, int*, int*, int*, int [4][4], int [4][4]);

// Hareket fonksiyonlarý
void w_up(int*, int*, int [4][4]);
void a_left(int*, int*, int [4][4]);
void s_down(int*, int*, int [4][4]);
void d_right(int*, int*, int [4][4]);



int main(){
	int board[4][4]; //Anlýk tahta
	int round = 1, score = 0;
	int highscore = 0;
	
	int preboard[4][4]; // Önceki tahta
	int preround = 1, prescore = 0;
	
	int tmpboard[4][4]; // Geçici tahta
	int tmpround = 1, tmpscore = 0;
	
	// Durumlar
	int stuck = 0; // Kontrol durumu
	int win = 0; // Kontrol durumu
	int nomove = 0; // Kontrol durumu
	
	char cmd; // Basýlan tuþ
	int i, j;
	
	srand(time(NULL));
	
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			board[i][j] = 0;
			preboard[i][j] = 0;
			tmpboard[i][j] = 0;
		}
	}
	
	// Baþlangýç ekraný
	cmd = start_screen(cmd);
	
	// Oyuna baþla
	if(cmd == 'S' || cmd == 's'){
		// Ýki rastgele deðer oluþtur
		create_random(board);
		create_random(board);
		// Hamleler bitene kadar devam et
		do{
			if(!nomove){
				print_board(&round, &score, board);
			}
			cmd = get_command(cmd, stuck, nomove);
			if(cmd == 'R' || cmd == 'r'
			|| cmd == 'X' || cmd == 'x'
			|| cmd == 'E' || cmd == 'e'){
				if(cmd == 'R' || cmd == 'r'){
					r_undo(&round, &preround, &score, &prescore, board, preboard); // Geri al
					stuck = 0;
					nomove = 0;
				}else if(cmd == 'X' || cmd == 'x'){
					x_restart(&round, &preround, &score, &prescore, &highscore, board, preboard); // Yeniden baþlat
					stuck = 0;
					nomove = 0;
				}
			}else{
				copy_board(&preround, &tmpround, &prescore, &tmpscore, preboard, tmpboard);
				copy_board(&round, &preround, &score, &prescore, board, preboard);
				if(cmd == 'W' || cmd == 'w'){
					w_up(&nomove, &score, board); // Yukarý kaydýr
				}else if(cmd == 'A' || cmd == 'a'){
					a_left(&nomove, &score, board); // Sola kaydýr
				}else if(cmd == 'S' || cmd == 's'){
					s_down(&nomove, &score, board); // Aþaðý kaydýr
				}else if(cmd == 'D' || cmd == 'd'){
					d_right(&nomove, &score, board); // Saða kaydýr
				}
				win = win_control(board);
				if(!win){
					if(nomove){
						copy_board(&tmpround, &preround, &tmpscore, &prescore, tmpboard, preboard);
					}else{
					create_random(board);
					round++;
					}
					stuck = stuck_control(board);
				}
			}
		}while(cmd != 'E' && cmd != 'e' && !win);
	}
	
	// Oyunu bitir
	if(cmd == 'E' || cmd == 'e'){
		printf("_________________________\n");
		printf("| | | | | | | | | | | | |\n");
		printf("| | G A M E | O V E R | |\n");
		printf("|_|_|_|_|_|_|_|_|_|_|_|_|\n\n");
		printf("- YOUR SCORE: %d\n", score);
		if(score > highscore){
			highscore = score;
		}
		printf("- HIGHSCORE:  %d\n", highscore);
	}
	if(win){
		round++;
		print_board(&round, &score, board);
		printf("_________________________\n");
		printf("| | | | | | | | | | | | |\n");
		printf("| | | Y O U | W O N | | |\n");
		printf("|_|_|_|_|_|_|_|_|_|_|_|_|\n");
		printf("\n- YOUR SCORE: %d\n", score);
		if(score > highscore){
			highscore = score;
		}
		printf("- HIGHSCORE:  %d\n", highscore);
	}
	return 0;
}

// Baþlama ekraný
char start_screen(char cmd){
	printf("\n   -2048- by Ali Emre    \n");
	printf("_________________________\n");
	printf("|     |     |     |     |\n");
	printf("|     |     |     |     |\n");
	printf("|_____|_____|_____|_____|\n");
	printf("|     |     |     |     |\n");
	printf("|     |     |     |     |\n");
	printf("|_____|_____|_____|_____|\n");
	printf("|     |     |     |     |\n");
	printf("|     |     |     |     |\n");
	printf("|_____|_____|_____|_____|\n");
	printf("|     |     |     |     |\n");
	printf("|     |     |     |     |\n");
	printf("|_____|_____|_____|_____|\n\n");
	printf("Press S to START, E to EXIT.\n");
	do{
		cmd = getch();
		if(cmd != 'E' && cmd != 'e' && cmd != 'S' && cmd != 's'){
			printf("________________________\n");
			printf("|                      |\n");
			printf("|  (!) Wrong Command!  |\n");
			printf("|______________________|\n");
			printf("\nPress S or E.\n");
		}
	}while(cmd != 'E' && cmd != 'e' && cmd != 'S' && cmd != 's');
	return cmd;
}

// Rastgele deðer oluþturma
void create_random(int b[4][4]){
	int rand_value; // Tabloya eklenecek deðer
	int rand_x; // Eklenecek deðer X konumu
	int rand_y; // Eklenecek deðer Y konumu
	do{
		rand_value = rand() % 10;
		if(rand_value == 0){
			rand_value = 4;
		}else{
			rand_value = 2;
		}
		rand_x = rand() % 4;
		rand_y = rand() % 4;
	}while(b[rand_x][rand_y] != 0);
	b[rand_x][rand_y] = rand_value;
}

// Tahtayý yazdýrma
void print_board(int *r, int *s, int b[4][4]){
	printf("\n- ROUND %d - SCORE: %d\n", *r, *s);
	printf("_________________________\n");
	printf("|     |     |     |     |\n");
	print_row(0, b);
	printf("|_____|_____|_____|_____|\n");
	printf("|     |     |     |     |\n");
	print_row(1, b);
	printf("|_____|_____|_____|_____|\n");
	printf("|     |     |     |     |\n");
	print_row(2, b);
	printf("|_____|_____|_____|_____|\n");
	printf("|     |     |     |     |\n");
	print_row(3, b);
	printf("|_____|_____|_____|_____|\n");
}
void print_row(int row, int b[4][4]){
	int i;
	printf("|");
	for(i=0 ; i<4 ; i++){
		if(b[row][i] == 0){
			printf("     |");
		}else{
			printf("%5d|", b[row][i]);
		}
	}
	printf("\n");
}

// Komut alma
char get_command(char cmd, int stuck, int nomove){
	if(stuck){
		printf("________________________\n");
		printf("|                      |\n");
		printf("|  (!) You are stuck.  |\n");
		printf("|______________________|\n");
		printf("\nPress R to UNDO.\n");
		printf("Press X to RESTART, E to EXIT.\n");
		do{
			cmd = getch();
			if(cmd != 'R' && cmd != 'r' && cmd != 'X' && cmd != 'x'
			&& cmd != 'E' && cmd != 'e'){
				printf("________________________\n");
				printf("|                      |\n");
				printf("|  (!) Wrong Command!  |\n");
				printf("|______________________|\n");
			}
		}while(cmd != 'R' && cmd != 'r' && cmd != 'X' && cmd != 'x'
		&& cmd != 'E' && cmd != 'e');
	}else{
		if(!nomove){
			printf("\nUse W, A, S, D to slide.\n");
			printf("Press R to UNDO.\n");
			printf("Press X to RESTART, E to EXIT.\n");
		}else{
			printf("___________________________________\n");
			printf("|                                 |\n");
			printf("|  (!) You cannot move that way.  |\n");
			printf("|_________________________________|\n");
		}
		do{
			cmd = getch();
			if(cmd != 'W' && cmd != 'w' && cmd != 'A' && cmd != 'a'
			&& cmd != 'S' && cmd != 's' && cmd != 'D' && cmd != 'd'
			&& cmd != 'R' && cmd != 'r' && cmd != 'X' && cmd != 'x'
			&& cmd != 'E' && cmd != 'e'){
				printf("________________________\n");
				printf("|                      |\n");
				printf("|  (!) Wrong Command!  |\n");
				printf("|______________________|\n");
			}
		}while(cmd != 'W' && cmd != 'w' && cmd != 'A' && cmd != 'a'
		&& cmd != 'S' && cmd != 's' && cmd != 'D' && cmd != 'd'
		&& cmd != 'R' && cmd != 'r' && cmd != 'X' && cmd != 'x'
		&& cmd != 'E' && cmd != 'e');
	}
	return cmd;
}

// Hareketler
void w_up(int *no_move, int *s, int b[4][4]){
	int i;
	*no_move = 1;
	for(i=0 ; i<4 ; i++){
		// Sýfýrlarý tara ve kaydýr
		while(b[0][i] == 0 && (b[1][i] != 0 || b[2][i] != 0 || b[3][i] != 0)){
			b[0][i] = b[1][i];
			b[1][i] = b[2][i];
			b[2][i] = b[3][i];
			b[3][i] = 0;
			*no_move = 0;
		}
		while(b[1][i] == 0 && (b[2][i] != 0 || b[3][i] != 0)){
			b[1][i] = b[2][i];
			b[2][i] = b[3][i];
			b[3][i] = 0;
			*no_move = 0;
		}
		if(b[2][i] == 0 && b[3][i] != 0){
			b[2][i] = b[3][i];
			b[3][i] = 0;
			*no_move = 0;
		}
		
		// Ayný olan çiftleri topla ve kaydýr
		if(b[0][i] == b[1][i] && b[0][i] != 0 && b[1][i] != 0){
			b[0][i] = b[0][i] * 2;
			b[1][i] = b[2][i];
			b[2][i] = b[3][i];
			b[3][i] = 0;
			*s = *s + b[0][i];
			*no_move = 0;
		}
		if(b[1][i] == b[2][i] && b[1][i] != 0 && b[2][i] != 0){
			b[1][i] = b[1][i] * 2;
			b[2][i] = b[3][i];
			b[3][i] = 0;
			*s = *s + b[1][i];
			*no_move = 0;
		}
		if(b[2][i] == b[3][i] && b[1][i] != 0 && b[2][i] != 0){
			b[2][i] = b[2][i] * 2;
			b[3][i] = 0;
			*s = *s + b[2][i];
			*no_move = 0;
		}
	}
}
void a_left(int *no_move, int *s, int b[4][4]){
	int i;
	*no_move = 1;
	for(i=0 ; i<4 ; i++){
		// Sýfýrlarý tara ve kaydýr
		while(b[i][0] == 0 && (b[i][1] != 0 || b[i][2] != 0 || b[i][3] != 0)){
			b[i][0] = b[i][1];
			b[i][1] = b[i][2];
			b[i][2] = b[i][3];
			b[i][3] = 0;
			*no_move = 0;
		}
		while(b[i][1] == 0 && (b[i][2] != 0 || b[i][3] != 0)){
			b[i][1] = b[i][2];
			b[i][2] = b[i][3];
			b[i][3] = 0;
			*no_move = 0;
		}
		if(b[i][2] == 0 && b[i][3] != 0){
			b[i][2] = b[i][3];
			b[i][3] = 0;
			*no_move = 0;
		}
		
		// Ayný olan çiftleri topla ve kaydýr
		if(b[i][0] == b[i][1] && b[i][0] != 0 && b[i][1] != 0){
			b[i][0] = b[i][0] * 2;
			b[i][1] = b[i][2];
			b[i][2] = b[i][3];
			b[i][3] = 0;
			*s = *s + b[i][0];
			*no_move = 0;
		}
		if(b[i][1] == b[i][2] && b[i][1] != 0 && b[i][2] != 0){
			b[i][1] = b[i][1] * 2;
			b[i][2] = b[i][3];
			b[i][3] = 0;
			*s = *s + b[i][1];
			*no_move = 0;
		}
		if(b[i][2] == b[i][3] && b[i][2] != 0 && b[i][3] != 0){
			b[i][2] = b[i][2] * 2;
			b[i][3] = 0;
			*s = *s + b[i][2];
			*no_move = 0;
		}
	}
}
void s_down(int *no_move, int *s, int b[4][4]){
	
	int i;
	*no_move = 1;
	for(i=0 ; i<4 ; i++){
		// Sýfýrlarý tara ve kaydýr
		while(b[3][i] == 0 && (b[0][i] != 0 || b[1][i] != 0 || b[2][i] != 0)){
			b[3][i] = b[2][i];
			b[2][i] = b[1][i];
			b[1][i] = b[0][i];
			b[0][i] = 0;
			*no_move = 0;
		}
		while(b[2][i] == 0 && (b[0][i] != 0 || b[1][i] != 0)){
			b[2][i] = b[1][i];
			b[1][i] = b[0][i];
			b[0][i] = 0;
			*no_move = 0;
		}
		if(b[1][i] == 0 && b[0][i] != 0){
			b[1][i] = b[0][i];
			b[0][i] = 0;
			*no_move = 0;
		}
		
		// Ayný olan çiftleri topla ve kaydýr
		if(b[3][i] == b[2][i] && b[3][i] != 0 && b[2][i] != 0){
			b[3][i] = b[3][i] * 2;
			b[2][i] = b[1][i];
			b[1][i] = b[0][i];
			b[0][i] = 0;
			*s = *s + b[3][i];
			*no_move = 0;
		}
		if(b[2][i] == b[1][i] && b[2][i] != 0 && b[1][i] != 0){
			b[2][i] = b[2][i] * 2;
			b[1][i] = b[0][i];
			b[0][i] = 0;
			*s = *s + b[2][i];
			*no_move = 0;
		}
		if(b[1][i] == b[0][i] && b[1][i] != 0 && b[0][i] != 0){
			b[1][i] = b[1][i] * 2;
			b[0][i] = 0;
			*s = *s + b[1][i];
			*no_move = 0;
		}
	}
}
void d_right(int *no_move, int *s, int b[4][4]){
	int i;
	*no_move = 1;
	for(i=0 ; i<4 ; i++){
		// Sýfýrlarý tara ve kaydýr
		while(b[i][3] == 0 && (b[i][0] != 0 || b[i][1] != 0 || b[i][2] != 0)){
			b[i][3] = b[i][2];
			b[i][2] = b[i][1];
			b[i][1] = b[i][0];
			b[i][0] = 0;
			*no_move = 0;
		}
		while(b[i][2] == 0 && (b[i][0] != 0 || b[i][1] != 0)){
			b[i][2] = b[i][1];
			b[i][1] = b[i][0];
			b[i][0] = 0;
			*no_move = 0;
		}
		if(b[i][1] == 0 && b[i][0] != 0){
			b[i][1] = b[i][0];
			b[i][0] = 0;
			*no_move = 0;
		}
		
		// Ayný olan çiftleri topla ve kaydýr
		if(b[i][3] == b[i][2] && b[i][3] != 0 && b[i][2] != 0){
			b[i][3] = b[i][3] * 2;
			b[i][2] = b[i][1];
			b[i][1] = b[i][0];
			b[i][0] = 0;
			*s = *s + b[i][3];
			*no_move = 0;
		}
		if(b[i][2] == b[i][1] && b[i][2] != 0 && b[i][1] != 0){
			b[i][2] = b[i][2] * 2;
			b[i][1] = b[i][0];
			b[i][0] = 0;
			*s = *s + b[i][2];
			*no_move = 0;
		}
		if(b[i][1] == b[i][0] && b[i][1] != 0 && b[i][0] != 0){
			b[i][1] = b[i][1] * 2;
			b[i][0] = 0;
			*s = *s + b[i][1];
			*no_move = 0;
		}
	}
}

// Oyunun týkanmasý
int stuck_control(int b[4][4]){
	int i, j;
	int stuck = 1;
	// Sýfýr olan eleman var mý kontrolü
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			if(b[i][j] == 0){
				stuck = 0;
			}
		}
	}
	// Yan yana olan deðerler ayný mý kontrolü
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<3 ; j++){
			if(b[i][j] == b[i][j+1] || b[j][i] == b[j+1][i]){
				stuck = 0;
			}
		}
	}
	return stuck;
}

// Oyunun kazanýlmasý
int win_control(int b[4][4]){
	int i, j;
	int win = 0;
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			if(b[i][j] == 2048){
				win = 1;
			}
		}
	}
	return win;
}

// Kopyalama
void copy_board(int *r, int *pr, int *s, int *ps, int b[4][4], int pb[4][4]){
	int i, j;
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			pb[i][j] = b[i][j];
		}
	}
	*pr = *r;
	*ps = *s;
}

// Geri alma
void r_undo(int *r, int *pr, int *s, int *ps, int b[4][4], int pb[4][4]){
	int i, j;
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			b[i][j] = pb[i][j];
		}
	}
	*r = *pr;
	*s = *ps;
	printf("____________________________\n");
	printf("|                          |\n");
	printf("|  (!) You undo the game.  |\n");
	printf("|__________________________|\n");
}

// Yeniden baþlatma
void x_restart(int *r, int *pr, int *s, int *ps, int *hs, int b[4][4], int pb[4][4]){
	int i, j;
	printf("____________________________\n");
	printf("|                          |\n");
	printf("|  (!) Game is restarted.  |\n");
	printf("|__________________________|\n");
	printf("\n- YOUR SCORE: %d\n", *s);
	if(*s > *hs){
		*hs = *s;
	}
	printf("- HIGHSCORE:  %d\n", *hs);
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<4 ; j++){
			b[i][j] = pb[i][j] = 0;
		}
	}
	*r = 1, *pr = 1;
	*s = 0, *ps = 0;
	create_random(b);
	create_random(b);
}

/*
Call by Reference için kaynak:
https://qnaplus.com/change-a-variable-from-inside-a-c-function/
*/

