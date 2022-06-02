#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

char board[9][9] = {0,};
int direction[8][2] = {{-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1}};
int player1_place[2] = {0,0};
int player2_place[2] = {8,8};
int arrow_place[2] = {10,10};
int mine_place[8][2] = {};
int mine_count = 0;
int player1_weapon[2] = {5,3};
int player2_weapon[2] = {5,3};

void getanykey(){
	printf("\n\nPress Enter to continue >> ");
	char getanykey;
	fflush(stdin);
	scanf("%c",&getanykey);
}

void print_board(){
	printf("\n      ");
	for(int i = 0; i < 9; i++){
		printf("%d ",i+1);
	}
	printf("\n\n");
	for(int i = 0; i < 9; i++){
		printf("  %c   ",i+65);
		for(int j = 0; j < 9; j++){
			printf("%c ",board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int* get_player_place(char player_num){
	static int place[2] = {};
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(board[i][j] == player_num){
				place[0] = i;
				place[1] = j;
			}
		}
	}
	return place;
}

float get_yintercept(int i, int j, int a, int b){
	return j-(i*(float)(b-j)/(a-i));
}

float get_xintercept(int i, int j, int a, int b){
	return (-1)*get_yintercept(i,j,a,b)*(float)(a-i)/(b-j);
}

float get_y(int i, int j, int a, int b, int x){
	return (float)(b-j)/(a-i)*x + get_yintercept(i,j,a,b);
}

float get_x(int i, int j, int a, int b, int y){
	return (y - get_yintercept(i,j,a,b))*(float)(a-i)/(b-j);
}

void get_sight(int i, int j, char player_num){
	int *player_place;
	player_place = get_player_place(player_num);
	int sight_x;
	int sight_y;
	if(player_place[0] - i == 0){
		for(int k = 1; k <= abs(player_place[1] - j); k++){
			if((player_place[1] - j) < 0){
				sight_y = player_place[1] + k;
			}
			else{
				sight_y = player_place[1] - k;
			}
			sight_x = i;
			if(board[sight_x][sight_y] != 'X'){
				board[sight_x][sight_y] = 'O';
			}
			else{
				break;
			}
		}
		return;
	}

	float steepness = (float)(player_place[1] - j)/(player_place[0] - i);
	
	if(-1 <= steepness && steepness <= 1){
		for(int k = 1; k <= abs(player_place[0] - i); k++){
			if((player_place[0] - i) < 0){
				sight_x = player_place[0] + k;
			}
			else{
				sight_x = player_place[0] - k;
			}
			sight_y = get_y(i,j,player_place[0],player_place[1],sight_x) + 0.5;
			if(board[sight_x][sight_y] != 'X'){
				board[sight_x][sight_y] = 'O';
			}
			else{
				break;
			}
		}
	}
	
	else if(1 < steepness || steepness < -1){
		for(int k = 1; k <= abs(player_place[1] - j); k++){
			if((player_place[1] - j) < 0){
				sight_y = player_place[1] + k;
			}
			else{
				sight_y = player_place[1] - k;
			}
			sight_x = get_x(i,j,player_place[0],player_place[1],sight_y) + 0.5;
			if(board[sight_x][sight_y] != 'X'){
				board[sight_x][sight_y] = 'O';
			}
			else{
				break;
			}
		}
	}
}

void get_allsight(char player_num){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(i == 0 || i == 8 || j == 0 || j == 8){
				get_sight(i,j,player_num);
			}
		}
	}
}

int check_valid_terrain(){
	int count;
	for(int i = 2; i < 7; i++){
		for(int j = 2; j < 7; j++){
			count = 0;
			if(board[i][j] != 'X'){
				for(int k = 0; k < 8; k++){
					if(board[i + direction[k][0]][j + direction[k][1]] != 'X'){
						count++;
					}
				}
				if(count == 8){
					return 1;
				}
			}
		}
	}
	return 0;
}

void make_terrain_object(){
	int object_x, object_y, object_type_1, object_type_2;
	int valid_place;
	valid_place = 0;
	if(check_valid_terrain() == 0){
		return;
	}
	do{
		object_x = rand()%5 + 2;
		object_y = rand()%5 + 2;
		if( 
			board[object_x][object_y-1] == 'X' || 
			board[object_x][object_y+1] == 'X' || 
			board[object_x-1][object_y] == 'X' || 
			board[object_x+1][object_y] == 'X' ||
			board[object_x+1][object_y+1] == 'X' ||
			board[object_x-1][object_y+1] == 'X' ||
			board[object_x+1][object_y-1] == 'X' ||
			board[object_x-1][object_y-1] == 'X'
			){
			valid_place = 0;
		}
		else{
			valid_place = 1;
		}
	}while(valid_place != 1);

	object_type_1 = rand()%2;
	object_type_2 = rand()%2;
	board[object_x][object_y] = 'X';
	if(object_type_1 == 0){
		if( board[object_x][object_y-2] != 'X' &&
			board[object_x-1][object_y-2] != 'X' &&
			board[object_x+1][object_y-2] != 'X'
		){
			board[object_x][object_y-1] = 'X';
		}
	}
	else{
		if( board[object_x-2][object_y] != 'X' &&
			board[object_x-2][object_y-1] != 'X' &&
			board[object_x-2][object_y+1] != 'X'
		){
			board[object_x-1][object_y] = 'X';
		}
		
	}
	if(object_type_2 == 0){
		if( board[object_x][object_y+2] != 'X' &&
			board[object_x-1][object_y+2] != 'X' &&
			board[object_x+1][object_y+2] != 'X'
		){
			board[object_x][object_y+1] = 'X';
		}
	}
	else{
		if( board[object_x+2][object_y] != 'X' &&
			board[object_x+2][object_y-1] != 'X' &&
			board[object_x+2][object_y+1] != 'X'
		){
			board[object_x+1][object_y] = 'X';
		}
	}
}

void make_terrain(int max_object){
	for(int i = 0; i < max_object; i++){
		make_terrain_object();
	}
	board[1][1] = 'X';
	board[7][7] = 'X';
}

void print_help(){
	printf("\n%s\n \n%s \n%s \n%s \n%s\n \n%s \n%s \n%s \n%s \n%s\n \n%s \n%s \n%s \n%s\n \n%s \n%s \n%s \n",
		"--Sight of Gun Manual--",
		"1.Introduction",
		" First, This Game is turn-by-turn game.",
		" Second, this game is 2-player game.",
		" And finally, make sure not to see other player's screen.",

		"2.Game rule",
		" Player1 moves first.(move character with (w,a,s,d) )",
		" On each turn, you will firstly move your character one block at a time.",
		" Then you can do something introducted below.",
		" (Sight computation is correct, so do not claim your innocence.)",

		"  Use",
		"  -Gun (use if enemy is in your sight)",
		"  -Bow & Arrow (use where enemy will come)",
		"  -Land mine (use to block enemy's move, you can use land mine on your(left,right,up,down) , warning : do not step on your land mine)",

		"3.Developer Information",
		" Made by GeonHo Son",
		" Contact : sohn1029@gmail.com"
		);
}

void cover(int player){
	printf("\n\n   Player%d's turn\n\n\n\n",player);
	getanykey();
	system("cls");
}

int move(int player_num){
	char movement;
	printf("\n\n  Move your character one block first.(w,a,s,d) >>  ");
	fflush(stdin);
	scanf("%c",&movement);
	if(movement == 'w' && player_num == 1){
		player1_place[0] -= 1;
		if(player1_place[0] < 0 || board[player1_place[0]][player1_place[1]] == 'X'){
			player1_place[0] += 1;
			return 0;
		}
		board[player1_place[0]+1][player1_place[1]] == '+';
		return 1;
	}
	if(movement == 'w' && player_num == 2){
		player2_place[0] -= 1;
		if(player2_place[0] < 0 || board[player2_place[0]][player2_place[1]] == 'X'){
			player2_place[0] += 1;
			return 0;
		}
		board[player2_place[0]+1][player2_place[1]] == '+';
		return 1;

	}
	if(movement == 'a' && player_num == 1){
		player1_place[1] -= 1;
		if(player1_place[1] < 0 || board[player1_place[0]][player1_place[1]] == 'X'){
			player1_place[1] += 1;
			return 0;
		}
		board[player1_place[0]][player1_place[1]+1] == '+';
		return 1;

	}
	if(movement == 'a' && player_num == 2){
		player2_place[1] -= 1;
		if(player2_place[1] < 0 || board[player2_place[0]][player2_place[1]] == 'X'){
			player2_place[1] += 1;
			return 0;
		}
		board[player2_place[0]][player2_place[1]+1] == '+';
		return 1;

	}
	if(movement == 's' && player_num == 1){
		player1_place[0] += 1;
		if(player1_place[0] > 8 || board[player1_place[0]][player1_place[1]] == 'X'){
			player1_place[0] -= 1;
			return 0;
		}
		board[player1_place[0]-1][player1_place[1]] == '+';
		return 1;

	}
	if(movement == 's' && player_num == 2){
		player2_place[0] += 1;
		if(player2_place[0] > 8 || board[player2_place[0]][player2_place[1]] == 'X'){
			player2_place[0] -= 1;
			return 0;
		}
		board[player2_place[0]-1][player2_place[1]] == '+';
		return 1;

	}
	if(movement == 'd' && player_num == 1){
		player1_place[1] += 1;
		if(player1_place[1] > 8 || board[player1_place[0]][player1_place[1]] == 'X'){
			player1_place[1] -= 1;
			return 0;
		}
		board[player1_place[0]][player1_place[1]-1] == '+';
		return 1;

	}
	if(movement == 'd' && player_num == 2){
		player2_place[1] += 1;
		if(player2_place[1] > 8 || board[player2_place[0]][player2_place[1]] == 'X'){
			player2_place[1] -= 1;
			return 0;
		}
		board[player2_place[0]][player2_place[1]-1] == '+';
		return 1;
	}
	if(movement == 'w' || movement == 'a' || movement == 's' || movement == 'd'){
		return 1;
	}
	else{
		return 0;
	}
}

void make_player_if_sight(int player_num){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(player_num == 1 && board[i][j] == 'O' && i == player2_place[0] && j == player2_place[1]){
				board[i][j] = '2';
			}
			if(player_num == 2 && board[i][j] == 'O' && i == player1_place[0] && j == player1_place[1]){
				board[i][j] = '1';
			}
		}
	}
}

void make_blood(){
	int count = 0;
	while(count < 5){
		system("color 40");
		system("color 70");
		Sleep(10);
		count++;
	}
}
	

int death_arrow(int player_num){
	if(arrow_place[0] == 10 && arrow_place[1] == 10){
		return 0;
	}
	else{
		if(player_num == 1){
			if(player1_place[0] == arrow_place[0] && player1_place[1] == arrow_place[1]){
				make_blood();
				printf("\n Player1 killed by an arrow.\n");
				return 1;
			}
		}
		else if(player_num == 2){
			if(player2_place[0] == arrow_place[0] && player2_place[1] == arrow_place[1]){
				make_blood();
				printf("\n Player2 killed by an arrow.\n");
				return 2;
			}
		}
	}
	arrow_place[0] = 10;
	arrow_place[1] = 10;
	return 0;
}

int death_mine(int player_num){
	if(mine_count == 0){
		return 0;
	}
	else{
		for(int i = 0; i < mine_count; i++){
			if(player_num == 1){
				if(player1_place[0] == mine_place[i][0] && player1_place[1] == mine_place[i][1]){
					make_blood();
					printf("\n Player1 killed by a mine.\n");
					return 1;
				}
			}
			else if(player_num == 2){
				if(player2_place[0] == mine_place[i][0] && player2_place[1] == mine_place[i][1]){
					make_blood();
					printf(" \n Player2 killed by a mine.\n");
					return 2;
				}
			}
		}
		return 0;
	}
}

void print_weapon(int player_num){
	printf("\n Remaining items \n");
	if(player_num == 1){
		printf("\n Arrow : %d",player1_weapon[0]);
		printf("\n Land mine : %d\n",player1_weapon[1]);
	}
	else if(player_num == 2){
		printf("\n Arrow : %d",player2_weapon[0]);
		printf("\n Land mine : %d\n",player2_weapon[1]);
	}
}

int player(int player_num){
	char board_c[9][9] = {};
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			board_c[i][j] = board[i][j];
		}
	}

	cover(player_num);
	if(player_num == 1){
		board[player1_place[0]][player1_place[1]] = '1';
	}
	else if(player_num == 2){
		board[player2_place[0]][player2_place[1]] = '2';
	}
	get_allsight((player_num + '0'));
	make_player_if_sight(player_num);

	int valid_move = 0;//move character
	while(valid_move == 0){
		print_board();
		valid_move = move(player_num);
		if(valid_move == 0){
			printf("\n  Invalid movement...\n");
			getanykey();
		}
		system("cls");
	}
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			board[i][j] = board_c[i][j];
		}
	}
	if(player_num == 1){
		board[player1_place[0]][player1_place[1]] = '1';
	}
	else if(player_num == 2){
		board[player2_place[0]][player2_place[1]] = '2';
	}
	get_allsight((player_num + '0'));
	make_player_if_sight(player_num);
	int death_code = 0;
	death_code = death_mine(player_num);
	if(death_code == 1){
		return 2;
	}
	else if(death_code == 2){
		return 1;
	}
	death_code = death_arrow(player_num);
	if(death_code == 1){
		return 2;
	}
	else if(death_code == 2){
		return 1;
	}
	//code for death because of move

	int valid_hope = 0;
	do{
		print_board();
		print_weapon(player_num);
		printf("\n\n What do you want to use?(1/Nothing, 2/Gun, 3/Bow & arrow, 4/Land mine) >> ");
		fflush(stdin);
		scanf("%d",&valid_hope);
		if(valid_hope != 1 && valid_hope != 2 && valid_hope != 3 && valid_hope != 4){
			printf("\n  Invalid Usage...\n");
			getanykey();
		}
		if(player_num == 1 && valid_hope == 3 && player1_weapon[0] < 1){
			printf("\n No more arrows...\n");
			getanykey();
			valid_hope = 0;
		}
		if(player_num == 1 && valid_hope == 4 && player1_weapon[1] < 1){
			printf("\n No more land mines...\n");
			getanykey();
			valid_hope = 0;
		}
		if(player_num == 2 && valid_hope == 3 && player2_weapon[0] < 1){
			printf("\n No more arrows...\n");
			getanykey();
			valid_hope = 0;
		}
		if(player_num == 2 && valid_hope == 4 && player2_weapon[1] < 1){
			printf("\n No more land mines...\n");
			getanykey();
			valid_hope = 0;
		}
		system("cls");
	}while(valid_hope != 1 && valid_hope != 2 && valid_hope != 3 && valid_hope != 4);

	if(valid_hope == 1){
		print_board();
		getanykey();
	}
	else if(valid_hope == 2){//gun
		print_board();
		if(player_num == 1){
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){
					if(board[i][j] == '2'){
						make_blood();
						printf("\n Player2 killed by a gun.\n");
						getanykey();
						return 1;
					}
				}
			}
		}
		else if(player_num == 2){
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){
					if(board[i][j] == '1'){
						make_blood();
						printf("\n Player1 killed by a gun.\n");
						getanykey();
						return 2;
					}
				}
			}
		}
		printf("\n There's nothing!");
		getanykey();
	}
	else if(valid_hope == 3){//bow & arrow
		if(player_num == 1){
			player1_weapon[0]--;
		}
		else if(player_num == 2){
			player2_weapon[0]--;
		}
		int valid_arrow = 0;
		do{
			char ans[3] = {};
			system("cls");
			print_board();
			printf("\n Where do you want to shoot?(ex:C6) >> ");
			fflush(stdin);
			scanf("%s",&ans);
			if((65 <= ans[0] && ans[0] <= 73) && (49 <= ans[1] && ans[1] <= 57)){
				int place_x = ans[0] - 65;
				int place_y = ans[1] - 49;
				if(board[place_x][place_y] == 'O'){
					arrow_place[0] = place_x;
					arrow_place[1] = place_y;
					valid_arrow = 1;
				}
				else{
					printf("\n Unprofitable select...");
					getanykey();
				}
			}
			else{
				printf("\n Invalid expression... ");
				getanykey();
			}

		}while(valid_arrow == 0);
	}
	else if(valid_hope == 4){//land mine
		if(player_num == 1){
			player1_weapon[1]--;
		}
		else if(player_num == 2){
			player2_weapon[1]--;
		}
		int valid_mine = 0;
		do{
			char direction_m;
			system("cls");
			print_board();
			printf("\n Where do you want to place?(w,a,s,d) >> ");
			fflush(stdin);
			scanf("%c",&direction_m);
			int x,y;
			if(direction_m == 'w' && player_num == 1){
				x = player1_place[0] - 1;
				y = player1_place[1];
				if(x < 0 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 'w' && player_num == 2){
				x = player2_place[0] - 1;
				y = player2_place[1];
				if(x < 0 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 'a' && player_num == 1){
				x = player1_place[0];
				y = player1_place[1] - 1;
				if(y < 0 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 'a' && player_num == 2){
				x = player2_place[0];
				y = player2_place[1] - 1;
				if(y < 0 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 's' && player_num == 1){
				x = player1_place[0] + 1;
				y = player1_place[1];
				if(x > 8 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 's' && player_num == 2){
				x = player2_place[0] + 1;
				y = player2_place[1];
				if(x > 8 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 'd' && player_num == 1){
				x = player1_place[0];
				y = player1_place[1] + 1;
				if(y > 8 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m == 'd' && player_num == 2){
				x = player2_place[0];
				y = player2_place[1] + 1;
				if(y > 8 || board[x][y] == 'X'){
					printf("\n Invalid placement... ");
					getanykey();
					continue;
				}
				mine_place[mine_count][0] = x;
				mine_place[mine_count][1] = y;
				mine_count++;
				printf("\n Mine placed.");
				getanykey();
				valid_mine = 1;
			}
			if(direction_m != 'w' && direction_m != 'a' && direction_m != 's' && direction_m != 'd'){
				printf("\n Invalid expression... ");
				getanykey();
				continue;
			}

		}while(valid_mine == 0);
	}
	

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			board[i][j] = board_c[i][j];
		}
	}
	return 0;
}


int main(){
	srand(time(NULL));
	system("title Sight of Gun");
	system("color 70");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			board[i][j] = '+';
		}
	}

	int menu;
	do{
		system("cls");
		printf("\n>> Welcome to Sight of Gun <<\n\n");
		printf(">> Choose what you want.\n(1/Start Game, 2/Game Manual) >> ");
		scanf("%d",&menu);
		if(menu == 2){
			print_help();
			getanykey();
		}
		if(menu != 2 && menu != 1){
			printf("Invalid menu picked!\n");
		}
	}while(menu != 1);
	system("cls");

	make_terrain(7);

	int win_flag = 0;
	while(1){
		win_flag = player(1);
		if(win_flag == 1 || win_flag == 2){
			break;
		}
		system("cls");
		win_flag = player(2);
		if(win_flag == 2 || win_flag == 1){
			break;
		}
		system("cls");
	}

	printf("\n Player%d wins\n\n",win_flag);
	print_board();
	system("color 07");
	return 0;
}