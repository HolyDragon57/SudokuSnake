/*������ҵ�ɱ��˶���ʵ����ɣ����г�Ϯ��Ϊ��Ը�е��ɴ˴�����һ�в������*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> //������ʱ��Ϊ������������������ڿ� 
#include<conio.h> //��ȡ�������뺯�����ڿ� 
#include<windows.h> //��ȡ���ı���λ�ú������ڿ� 

#pragma comment(lib, "winmm.lib")//���ű���������Ҫָ�� 

#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'

//����С��Ϸͨ�õĺ���
int start();//��ʼ����
int finish();//�������� 
void init_score();//��ʼ���÷� 
void create_snake();//�������ʼ������ӡ 
void go_to_xy(int x, int y);//����ض�λ�����ع�� ���ȴ�����ʱ�Ḳ��ԭ�д�ӡ�������γɺڿ� 
void go_to_print(int x, int y);//��ָ��λ�ô�ӡ
void go_to_delete(int x, int y);//��ָ��λ�ò��� 
void color_change(int x, int y, int color); //��ָ��λ��ʹ��ָ����ɫ 
void direction_change(int *x, int *y, int click);// �ı�λ�õ�ϸ�ڲ��� 
int print_info();//�����չʾ������Ϸ�������Ϣ 

//����̰����ʹ�ú��� 
void complete_map_info(); //���Ƶ�ͼ��Ϣ 
void build_graph();//��ͼ
int read_clicks();//�ж�����Ƿ�����Լ���������
int opposite(char click, char old_click);//�ж���Ҳ����Ƿ�����һ�β���ǡ���෴ 
void changing_body(int x, int y);//�ı�λ�� 
void eating();//�Ե�ʳ���ı䳤�� 
void growing();//����䳤
void speed_change();//���ٶȿ��Ƹı�
void situation_change(int old_color);//�߳Ե���ͬ��ʵ������仯 
void food_appearing();//ʳ���������
int game_over_judgement();//�ж���Ϸ�Ƿ����
int in_or_out_judgement(int x, int y); //�ж����꣨x,y���Ƿ������ǽ��

//̰��������ʹ�ú��� 
void complete_sudoku_info();//����������Ϣ  
void build_sudoku();//�ڿ���̨�ϻ�����ͼ 
int read_clicks2();//̰���������ж�ȡ��Ҳ��� 
void changing_body2(int x, int y);//̰���������иı�λ�� 
int sudoku_completed();//�ж������Ƿ����
int game_over_judgement2();//̰���������н����ж��������в�ͬ 
void speed_change2();//̰�����������ߵ��ٶ�Ӧ�ø��� 
void eating2();//̰���������л��ֹ���ͬ 

typedef struct Snake
{
	int x;
	int y;
	struct Snake *next;
}snake;
//�ߵ�����ṹ 

snake *head;  

struct Food
{
	int x;
	int y;
}food;

int map[10][80][40];//��һ������Ϊ��ͼ��ţ�����������Ϊ�������� 
int sudoku[10][50][30];//��һ������Ϊ������ţ������������Ϊ�������� 
int value[10][50][30];//���������ֵ�״̬ ,1�������֣�2�������� 
int map_num;//�洢������ɵ�ͼ��� 
int sudoku_map_num;//�洢�������������� 
int score[2][100];//��ͬ��Ϸ�ĵ÷� 
int play_round0 = 0;//��¼���̰���߾���
int play_round1 = 0;//��¼���̰������������
int personal_best0 = 0;//��¼̰������߷��� 
int personal_best1 = 0;//��¼̰����������߷��� 
int color;//ʳ����ɫ 
int speed = 0;//�ٶȸı���� 
char coin; //���ѡ����Ϸģʽ 
int finish_num = 0;//�������д���ָ��� 
int changeflag = 0;//�ж�������״̬ 
int end_or_not; //�жϽ�����־ 
int begin, end; //����ʱ�������¼�������ʱ�� 

/********************************/
//������ 
int main()
{
	//��ҿ��ܷ�������Ϸ�����Ҫ������ѭ���� 
	//mciSendString("open background.mp3 alias start", NULL, 0, NULL);
	//�������ļ� 
	//mciSendString("play start repeat", NULL, 0, NULL);
	//������Ϸ�Ϳ�ʼ���ű������� 
	init_score();
	while(1)
		{system("cls");
		while(start())
			{
			}
		//�޶�����ֻ̨����Ұ��ض������л������������ִ�� 
		if(coin == '1') 
			{build_graph();
			food_appearing();
			if(read_clicks() == 0) 
				if(end_or_not == 1)
					break;
			//���϶�ȡ��Ҳ�����ֱ����Ϸ���� 
			play_round0 ++;
			}
		
		else if(coin == '2')
			{build_sudoku();
			if(read_clicks2() == 0)
				if(end_or_not == 1)
					break;
			play_round1 ++;
			}
		finish_num = 0;//������ɸ�����ʼ������ֹ�ۻ� 
		}
	//��Ϸ��������������ֹ���ر������ļ��� 
	//mciSendString("stop start", NULL, 0, NULL);
	//mciSendString("close start", NULL, 0, NULL);
	//�����ĸ�mciSendString����ǰ��ע�ͷ��ż���ѭ�����ű������֣���̰���߻�ż�����Ͻ�ǽש��ʧ��BUG�� 
	return 0;
}

/********************************/
//��ʼ������
void init_score()
{
	int i;
	for(i = 0; i < 100; i ++);
		score[0][i] = score[1][i] = 0;
} 

/********************************/
//����
void create_snake()
{
	snake *q;
	head = (snake*)malloc(sizeof(snake));
	q = (snake*)malloc(sizeof(snake));
	head -> x = 24; head -> y = 12;
	q -> x = 24; q -> y = 13;
	head -> next = q;
	q -> next = NULL;
	//��ʼ�ߵ�λ�� 
	color_change(head -> x, head -> y, 7);
	go_to_print(head -> x, head -> y);
	go_to_print(q -> x, q -> y);
	go_to_xy(16, 27);//����ƿ�����ֹ�γɺڿ� 
} 

/********************************/
//�ƶ���ָ��λ�� 
void go_to_xy(int x, int y)
{
	COORD new_pos;
	HANDLE old_pos = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ��ǰ���λ�� 
	new_pos.X = x;
	new_pos.Y = y;
	SetConsoleCursorPosition(old_pos, new_pos);//���¹��λ��
	
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE; //��겻�ɼ� 
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(old_pos, &cursor);//���¹����Ϣ�����ع�� 
}

/********************************/
//ָ��λ�ô�ӡ 
void go_to_print(int x, int y)
{
	go_to_xy(x, y);
	printf("��");
}

/********************************/
//ָ��λ��ɾ�� 
void go_to_delete(int x, int y)
{
	go_to_xy(x, y);
	printf("  ");
}

/********************************/
//������ƶ���ָ��λ�ò��ı�ǰ��������ɫ 
void color_change(int x, int y, int color)
{
	go_to_xy(x, y);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ��ǰ���ھ��
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color); //�ı䵱ǰ�����ǰ��������ɫ 
} 

/********************************/
//�йط���ı�ľ���ϸ�� 
void direction_change(int *x, int *y, int click) 
{
	switch(click)
		{case up:
			*y -= 1;
			break;
		case down:
			*y += 1;
			break;
		case left:
			*x -= 2;
			break;
		case right:
			*x += 2;
			break;
		default:
			break;
		}
	//ע��˴�x�ı仯���Ϊ2 
} 

/********************************/
//��ʼ���� 
int start()
{
	int x;
	srand((int)time(NULL));
	go_to_xy(11, 3);
	printf("  ������    ��      ��      ��      ��    ����  ����������");
	go_to_xy(11, 4);
	printf("��          ����    ��    ��  ��    ��  ��      ��");
	go_to_xy(11, 5);
	printf("  ������    ��  ��  ��  ����������  ����        ��������");
	go_to_xy(11, 6);
	printf("        ��  ��    ����  ��      ��  ��  ��      ��");
	go_to_xy(11, 7);
	printf("  ������    ��      ��  ��      ��  ��    ����  ����������");
	go_to_xy(35, 9);
	                        printf("��      ��");
	go_to_xy(35, 10);
	                        printf("����  ����");
	go_to_xy(35, 11);
	                        printf("��      ��");
	go_to_xy(37, 12);
	                          printf("������");
	go_to_xy(39, 13);
	                            printf("��");
	go_to_xy(37, 14);
	                          printf("��  ��");
	go_to_xy(13, 15);
	printf("����Ӣ�����뷨�в�����wasd���Ʒ��������������ͣ");
	go_to_xy(13, 17); 
	printf("Press 1 to have a quick start of game SNAKE!");
	go_to_xy(13, 18);
	printf("Press 2 to try the game Snake Sudoku!");
	go_to_xy(13, 19);
	printf("Press 3 to get relevant information about the two games!");
	if(_kbhit())
		coin = _getch();
	if(coin == '1')
		{complete_map_info();
		map_num = rand() % 5; 
		system("cls"); //�������� 
		return 0;
		}
	else if(coin == '2')
		{complete_sudoku_info();
		sudoku_map_num = rand() % 5; 
		system("cls"); //��������
		return 0;
		}
	else if(coin == '3')
		{system("cls");
		while(1)
			{x = print_info();
			if(x == 0)
				break;
			}
		coin = '0';
		//��coinֵ��ʼ������ֹ���ź������� 
		return 1;
		}
	else return 1;
}

/********************************/
//�������� 
int finish()
{
	char click;
	system("cls"); //���� 
	color_change(35, 8, 7); 
	int points, time;
	//������Ϸ��ͬ������ͬ�Ľ������ 
	if(coin == '1') points = score[0][play_round0];
	else if(coin == '2') points = score[1][play_round1];
	printf("YOUR SCORE: %d", points);
	time = end - begin;
	//��¼������ʱ 
	go_to_xy(35, 9);
	//����Ҫ����չʾ����ʱ�� 
	if(coin == '2')
		printf("YOUR TIME: %d", time);
	go_to_xy(30, 12);
	if(points <= 300)
	    printf("Play more carefully next time!");
	else if(points > 300 && points <= 1000)
		printf("You can do better next time!");
	else
		printf("Good job! play for the highest record next time!");
	//�Բ�ͬ�ֶθ������� 
	go_to_xy(35, 10);
	if(coin == '1')
		{if(score[0][play_round0] > personal_best0)
			personal_best0 = score[0][play_round0];
		printf("Personal Best: %d", personal_best0);
		}
	else if(coin == '2') 
		{if(score[1][play_round1] > personal_best1)
			personal_best1 = score[1][play_round1];
		printf("Personal Best: %d", personal_best1);
		}
	//�����һ��������ʾ 
	go_to_xy(20, 18);  
	printf("Press 4 to return to the start interface.");
	go_to_xy(20, 19);  
	printf("Press 5 to finish the program.");
	while(1)
		{if(kbhit())
			{click = _getch();
			if(click == '4')
				{coin =  '0';
				end_or_not = 0;
				break;
				}
			else if(click == '5')
				{end_or_not = 1;
				break;
				}
			}
		}
	//�ж��������Ҫ������Ϸ���ǽ�����Ϸ 
	if(end_or_not == 1)
		{snake *p, *q;
		for(p = head; p != NULL; p = q)
			{q = p -> next;
			free(p);
			}
		//�ͷ������ڴ� 
		return 0;
		}
	else
		return 0;
}

/********************************/
//���Ƶ�ͼ��ģ   ����֮������ö�ȡ�ļ���ʽ��ɣ�����ͬ�� 
void complete_map_info()
{
	int i, j;
	for(i = 0; i < 80; i ++)
		for(j = 0; j < 40; j ++)
			map[0][i][j] = map[1][i][j] = map[2][i][j] = map[3][i][j] = map[4][i][j] = map[5][i][j] = 0;
	//��ʼ��  ��ֵΪ1����Ҫװ��ǽש 
	for(i = 0; i < 60; i += 2)                  map[0][i][0] = map[0][i][24] = 1;
	for(j = 1; j < 24; j ++)                    map[0][0][j] = map[0][58][j] = 1;
	//��ͼ0��ģ���   30*25���� 
	for(i = 0; i < 17; i +=2)                   map[1][i][8 - (i/2)] = 1;
	for(j = 9; j < 17; j ++)                    map[1][0][j] = 1;
	for(i = 2, j = 17; i < 17; i += 2, j ++)    map[1][i][j] = 1;
	for(i = 18; i < 33; i += 2)                 map[1][i][24] = 1;
	for(i = 34, j = 23; i < 49; i += 2, j --)   map[1][i][j] = 1;
	for(j = 15; j > 8; j --)                    map[1][48][j] = 1;
	for(i = 48, j = 8; j > 0; i -=2, j --)      map[1][i][j] = 1;
	for(i = 18; i < 33; i ++)                   map[1][i][0] = 1;
	//��ͼ1��ģ���   25*25�˱��Ρ�
	for(i = 16; i < 33; i += 2)                 map[2][i][0] = 1;
	for(j = 1; j < 9; j ++)                     map[2][16][j] = 1;
	for(i = 0; i < 15; i += 2)                  map[2][i][8] = 1;
	for(j = 9; j < 17; j ++)                    map[2][0][j] = 1;
	for(i = 2; i < 17; i += 2)                  map[2][i][16] = 1;
	for(j = 17; j < 25; j ++)					map[2][16][j] = 1;
	for(i = 18; i < 33; i +=2)					map[2][i][24] = 1;
	for(j = 23; j > 15; j --)					map[2][32][j] = 1;
	for(i = 34; i < 49; i += 2)					map[2][i][16] = 1;
	for(j = 15; j > 7; j --)					map[2][48][j] = 1;
	for(i = 46; i > 31; i -= 2)					map[2][i][8] = 1;
	for(j = 7; j > 0; j --)						map[2][32][j] = 1;
	//��ͼ2��ģ���   25*25ʮ��
	for(i = 0; i < 51; i += 2)					map[3][i][0] = 1;
	for(j = 1; j < 6; j ++)						map[3][0][j] = 1;
	for(i = 2, j = 6; j < 11; i += 2, j ++)		map[3][i][j] = 1;
	for(i = 12; i < 21; i += 2)					map[3][i][10] = 1;
	for(j = 11; j < 16; j ++)					map[3][20][j] = 1;
	for(i = 18; i > 9; i -= 2)					map[3][i][15] = 1;
	for(i = 8, j = 16; j < 21; i -= 2, j ++)	map[3][i][j] = 1;
	for(j = 21; j < 26; j ++)					map[3][0][j] = 1;
	for(i = 2; i < 51; i += 2)					map[3][i][25] = 1;
	for(j = 24; j > 19; j --)					map[3][50][j] = 1;
	for(i = 48, j = 19; j > 14; i -= 2, j --)	map[3][i][j] = 1;
	for(i = 38; i > 29; i -= 2)					map[3][i][15] = 1;
	for(j = 14; j > 9; j --)					map[3][30][j] = 1;
	for(i = 32; i < 41; i += 2)					map[3][i][10] = 1;
	for(i = 42, j = 9; j > 4; i += 2, j --)		map[3][i][j] = 1;
	for(j = 4; j > 0; j --)						map[3][50][j] = 1;
	//��ͼ3��ģ���   26*26ɳ© 
	for(i = 0; i < 47; i += 2)					map[4][i][0] = 1;
	for(j = 1; j < 16; j ++)					map[4][46][j] = 1; 
	for(i = 44; i > 9; i -= 2)					map[4][i][15] = 1;
	for(j = 16; j < 21; j ++)					map[4][10][j] = 1;
	for(i = 12; i < 47; i += 2)					map[4][i][20] = 1;
	for(j = 21; j < 26; j ++)					map[4][46][j] = 1;
	for(i = 46; i >= 0; i -= 2)					map[4][i][25] = 1;
	for(j = 24; j > 9; j --)					map[4][0][j] = 1;
	for(i = 2; i < 37; i += 2)					map[4][i][10] = 1;
	for(j = 9; j > 4; j --)						map[4][36][j] = 1;
	for(i = 34; i >= 0; i -= 2)					map[4][i][5] = 1;
	for(j = 4; j > 0; j --)						map[4][0][j] = 1;
	//��ͼ4��ģ���   24*26���� 
} 

/********************************/
//��ͼ 
void build_graph()
{
	int i, j;
	//������ά���齨ͼ 
	for(j = 0; j < 40; j ++)
		for(i = 0; i < 80; i += 2)
			if(map[map_num][i][j] == 1)
				go_to_print(i, j);
	create_snake(); 
	printf("Your Score: %d     =���أ�=", score[0][play_round0]);
}

/********************************/
//��ȡ��Ҳ��� 
int read_clicks()
{
	int x = head -> x, y = head -> y;
	char click = 1, old_click = 1;
	while(1)
		{if(game_over_judgement() == 0) return 0; //ѭ����ֹ����Ϊ��Ϸ���� 
		if(_kbhit()) //�������������а������·��㣬�ް�������Ϊ�㡣 
			{click = _getch();//���ڻ������洢֮ǰ���µİ��������������������²������ 
			if(opposite(click, old_click) == 1) //���������֮ǰ�෴����ȴ���һ�ΰ�������������෴�����ƶ��������������� 
				click = old_click;
			}
		old_click = click;
		direction_change(&x, &y, click); 
		if(x != head -> x || y != head -> y)
			changing_body(x, y);
		//һ����ʼ�������߾Ϳ�ʼ�˶� 
		eating();
		//ʵʱ������Ƿ�Ե�ʳ�� 
		}
} 

/********************************/
//�ж��Ƿ�����һ�β��������෴ 
int opposite(char click, char old_click)
{
	if(click == 'w' && old_click == 's') return 1;
	if(click == 'a' && old_click == 'd') return 1;
	if(click == 's' && old_click == 'w') return 1;
	if(click == 'd' && old_click == 'a') return 1;
	else return 0;
}


/********************************/
//�ߵ�������ƶ� 
void changing_body(int x, int y)
{
	//ͨ��ɾ��β�ͺʹ�ӡͷ��ʵ�ֶ�̬��Ч�� 
	snake *p, *new_head;
	for(p = head; p -> next -> next != NULL; p = p -> next) ;
	go_to_delete(p -> next -> x, p -> next -> y); // ɾ��β�� 
	go_to_xy(39, 28); //�ı���λ�ã���ֹ�γɺڿ� 
	free(p -> next); //�ͷ�β����ڴ�ռ�
	p -> next = NULL;
	new_head = (snake*)malloc(sizeof(snake));
	new_head -> x = x;
	new_head -> y = y;
	new_head -> next = head;
	head = new_head;
	//�ߵ�����ṹ�ع����
	go_to_print(head -> x, head -> y); //��ӡͷ�� 
	go_to_xy(39, 28); //�ı���λ�ã���ֹ�γɺڿ� 
} 

/********************************/
//̰���߳Ե�ʳ���ı仯 
void eating()
{
	speed_change(); //�����ٶ� 
	if(food.x == head -> x && food.y == head -> y)
		{color_change(head -> x, head -> y, color); 
		int old_color = color; 
		food_appearing(); 
		//������ʳ�� 
		situation_change(old_color); 
		//���ݳԵ���ʳ�����̰���ߵ�״̬ 
		color_change(28, 27, 7); 
		//�ƶ��������ӡ�÷ִ� 
		printf("%d", score[0][play_round0]);
		color_change(39, 28, old_color); 
		//�ƿ���꣬��ֹ�γɺڿ� 
		}
}

/********************************/
//�߱䳤 
void growing()
{
	//�����ȱ䳤 
	snake *q, *new_tail;
	new_tail = (snake*)malloc(sizeof(snake));
	for(q = head; q -> next != NULL; q = q -> next) ;  
	q -> next = new_tail;
	new_tail -> next = NULL;
}

/********************************/
//�ٶȿ�����ı� 
void speed_change() 
{
	int time; 
	//ֹͣ����ʱ�䣬����Ϊ��λ 
	//���ݵ�ǰ��һ�÷����ı���Ļ����ʱ�䣬�޸���Ϸ�Ѷ� 
	if(score[0][play_round0] <= 500 && speed == 0) time = 150;
	else if(score[0][play_round0] <= 500 && speed == 1) time = 120;
	else if(score[0][play_round0] <= 500 && speed == -1) time = 180;
	//һ���Ѷ� 
	else if(score[0][play_round0] > 500 && score[0][play_round0] <= 1000 && speed == 0) time = 100;
	else if(score[0][play_round0] > 500 && score[0][play_round0] <= 1000 && speed == 1) time = 80;
	else if(score[0][play_round0] > 500 && score[0][play_round0] <= 1000 && speed == -1) time = 120;
	//�����Ѷ� 
	else if(score[0][play_round0] > 1000 && score[0][play_round0] <= 1500) time = 50;
	//�����Ѷ� 
	else time = 10;
	//�����Ѷ� 
	Sleep(time); //ִ�й��� 
} 

/********************************/
//�Ե���ͬ��ʵ��������� 
void situation_change(int old_color)
{
	if(old_color == 7)
		{growing();
		score[0][play_round0] += 10;
		speed = 0; 
		}//β�ͳ��ȱ䳤һ�� ��ɫ��ʵ 
	else if(old_color == 6)
		{growing();
		growing(); 
		score[0][play_round0] += 20;
		speed = 0; 
		}//β�ͳ��ȱ䳤����  ��ɫ��ʵ
	else if(old_color == 3) 
		{snake *p;
		speed = 0;
		for(p = head; p -> next -> next!= NULL; p = p -> next) ;
		if(p != head)
			{go_to_delete(p -> next -> x, p -> next-> y);
			go_to_xy(62, 28); 
			free(p -> next);
			p -> next = NULL;
			}  
		score[0][play_round0] += 10;
		} //β�ͳ�������һ�� ����ʱ������  ��ɫ��ʵ
	else if(old_color == 2)
		{speed = 0;
		growing(); 
		score[0][play_round0] += 50; //�ӷ�50  ��ɫ��ʵ 
		}
	else if(old_color == 4)
		{speed = 1;
		growing();
		score[0][play_round0] += 10;  
		}//�ٶȼӿ죨1000�����£� ��ɫ��ʵ
	else if(old_color == 1)
		{speed = -1;
		growing();
		score[0][play_round0] += 5; 
		}//�ٶȼ�����ֻ��5�֣�1000�����£�  ��ɫ��ʵ
	else
		{speed = 0;
		score[0][play_round0] += 10;
		}//��ɫ��ʵ ���Ȳ��仯
} 

/********************************/
//ʳ��������� 
void food_appearing()
{
	//�����������λ�ã��ж�ÿһ��λ���Ƿ����ֱ������һ�����ʵ�λ�� 
	int flag = 1;
	while(flag)
		{flag = 0;
		//����ʹ֮������ǽ�� 
		if(map_num == 0)
			{food.x = ((rand() % (30 - 2)) + 1) * 2; //ֻ�ܳ�����2-length*2 -4�е�ż��λ�� 
			food.y = rand() % (25 - 2) + 1; //ֻ�ܳ�����1-width-1�� 
			}
		else
			{food.x = (rand() % 24 + 1) * 2;
			food.y = rand() % 24 + 1; 
			//ͨ����С��Χ�Ӷ�����ʱ�� 
			}
		if(in_or_out_judgement(food.x, food.y) == 0) //�ж��Ƿ�λ��ǽ�ڻ�ǽ�� 
			flag = 1;
		//Ȼ�����Ƿ��������غ� 
		snake *p; 
		for(p = head; p != NULL; p = p -> next)
			if(p -> x == food.x && p -> y == food.y)
				flag = 1;
		} 
	color = rand() % 7 + 1;
	color_change(food.x, food.y, color);
	printf("��"); 
	//���������ͬ��ɫ��ʵ 
	color_change(39, 28, 7); 
	//�ı�ذ�ɫ ���ƶ������������ֹ�γɺڿ� 
}

/********************************/
//�ж���Ϸ�������� 
int game_over_judgement()
{
	snake *p;
	if(in_or_out_judgement(head -> x, head -> y) == 0)
		{finish();
		return 0; 
		}
	//ײǽ����Ϸ���� 
	else 
		{for(p = head -> next; p != NULL; p = p -> next)
			if(head -> x == p -> x && head -> y == p -> y)
				{finish();
				return 0;
				}
		//ײ��������Ϸ���� 
		return 1;
		//������Ϸ���� 
		}
}

/********************************/
//�ж������Ƿ�λ�ڱպϵ�ͼ�ڲ� 
int in_or_out_judgement(int x, int y)
{
	int flag = 0, i;
	//��ǽ�غ�����Ȼ���С� 
	if(map[map_num][x][y] == 1) return 0;
	//ͨ���ж��ĸ���λ�Ƿ������ǽ���ж��Ƿ�λ��ǽ�ڣ���ĳЩ���������ͼ������ 
	for(i = x - 2; i >= 0; i -= 2)
		if(map[map_num][i][y] == 1)
			{flag ++;
			break;
			}
	for(i = x + 2; i < 80; i += 2)
		if(map[map_num][i][y] == 1)
			{flag ++;
			break;
			}
	for(i = y - 1; i >= 0; i --)
		if(map[map_num][x][i] == 1)
			{flag ++;
			break;
			}
	for(i = y + 1; i < 40; i --)
		if(map[map_num][x][i] == 1)
			{flag ++;
			break;
			}
	if(flag == 4) return 1;
	else return 0;
} 

/********************************/
//���ļ��ж�ȡ����������Ϣ���ʼ���ֵ����������1��ʾ���֣�0��ʾ���� 
void complete_sudoku_info()
{
	FILE *in;
	int i, j, k;
	in = fopen("sudoku.txt", "r");
	if(in == NULL)
		printf("CANNOT OPEN FILE!");
	//�����ļ���ȡ���� 
	for(k = 0; k < 5; k ++)
		for(i = 0; i < 50; i ++)
			for(j = 0; j < 30; j ++)
				sudoku[k][i][j] = 0;
	//��ʼ����ά���� 
	for(k = 0; k < 5; k ++)
		{for(j = 1; j < 18 ; j += 2)
			for(i = 2; i < 35; i += 4)
				fscanf(in, "%d", &sudoku[k][i][j]);
		//��ȡ������Ϣ 
		for(j = 0; j < 19; j += 2)
			for(i = 0; i < 37; i += 2)
				sudoku[k][i][j] = 10;
		for(i = 0; i < 37; i += 4)
			for(j = 0; j < 19; j ++)
				if(sudoku[k][i][j] != 10)
					sudoku[k][i][j] = 10;
		//�趨�߽� 
		for(j = 1; j < 18 ; j += 2)
			for(i = 2; i < 35; i += 4)
				fscanf(in, "%d", &value[k][i][j]);
		//��ȡ����״̬ 
		}
	fclose(in); 
	//�ر��ļ� 
}

/********************************/
//�ڿ���̨�ϻ�������ͼ 
void build_sudoku()
{
	int i, j; 
	for(i = 2; i < 35; i += 2)
		for(j = 1; j < 18; j ++)
			{if(sudoku[sudoku_map_num][i][j] == 10)
				{if(i == 12 || i == 24 || j == 6 || j == 12)
					color_change(i, j, 10);
				else
					color_change(i, j, 8);
				go_to_print(i, j);
				}
			//ǽ�����ַָ���Ϊ��ɫ �Ź���ָ���Ϊ��ɫ 
			else if(value[sudoku_map_num][i][j] == 1)
				{color_change(i, j, 2);
				printf("%d", sudoku[sudoku_map_num][i][j]);
				} 
			//����������ɫΪ��ɫ 
			}
	color_change(0, 0, 1);
	for(i = 0; i < 37; i += 2)
		{go_to_print(i, 0);
		go_to_print(i, 18);
		sudoku[sudoku_map_num][i][0] = sudoku[sudoku_map_num][i][18] = 11;
		}
	for(j = 1; j < 18; j ++)
		{go_to_print(0, j);
		go_to_print(36, j); 
		sudoku[sudoku_map_num][0][j] = sudoku[sudoku_map_num][36][j] = 11;
		}
	//��ӡ�߽���ɫǽ������ֵ11�����������Ϸ�����ж� 
	color_change(6, 21, 7);
	printf("Your Score: %d      =���أ�=", score[1][play_round1]); 
	finish_num = 0; 
	//�������д���ֳ�ʼ��Ϊ�� 
} 

/********************************/
//̰���������ж�ȡ��Ҳ��� 
int read_clicks2()
{
	begin = time(NULL);
	//��¼��ʼʱʱ�� 
	create_snake();
	int x = head -> x, y = head -> y;
	char click = 1, old_click = 1;
	while(1)
		{if(game_over_judgement2() == 0) return 0; //ѭ����ֹ����Ϊ��Ϸ���� 
		if(_kbhit()) //�������������а������·��㣬�ް�������Ϊ�㡣 
			{click = _getch();//���ڻ������洢֮ǰ���µİ��������������������²������ 
			if(isalpha(click))
				{if(opposite(click, old_click) == 1) //���������֮ǰ�෴����ȴ���һ�ΰ�������������෴�����ƶ��������������� 
					click = old_click;
				else old_click = click; 
				//�����������������洢��һ�η��� ����������ʱʹ�� 
				}
			}
		//����ĸ��ı䷽�� 
		if(isalpha(click)) 
			direction_change(&x, &y, click);
		//�˴�ע��x�ı仯���Ϊ2 
		else if(isdigit(click))
			{if(sudoku[sudoku_map_num][x][y]!=0&&sudoku[sudoku_map_num][x][y]!=10)
				if(sudoku[sudoku_map_num][x][y]!=11&&value[sudoku_map_num][x][y]==0&&(click-'0')==sudoku[sudoku_map_num][x][y])
				{eating2();
				value[sudoku_map_num][x][y] = 1;
				}
			//�жϴ��Ƿ���ȷ����ȷ��ı������ȣ��÷֣���ֵ 
			click = '0';
			//һ��Ҫ��ʼ������Ȼ�߻�һֱ�洢�������ֱ���ƶ�����ȷλ��ֱ�Ӵ�ӡ 
			if(_kbhit())
				{click = _getch();
				if(isalpha(click))
					{if(opposite(click, old_click) == 1) 
						click = old_click;
					else old_click = click;
					}
				} 
			//�����ڽ���ʱ������Ƶ��С����Ļ����ʱ�䣬�׵���ײǽ 
			direction_change(&x, &y, old_click);
			} 
		if(x != head -> x || y != head -> y)
			changing_body2(x, y);
		//�ƶ����� 
		speed_change2();
		//ˢ����Ļ 
		}
}

/********************************/
//�������ߵ��ƶ�����̰�����Բ�ͬ 
void changing_body2(int x, int y)
{
	//ͨ��ɾ��β�ͺʹ�ӡͷ��ʵ�ֶ�̬��Ч�� 
	snake *p, *new_head;
	for(p = head; p -> next -> next != NULL; p = p -> next) ;
	//�ҳ���β 
	if(changeflag == 0) 
		{go_to_delete(p -> next -> x, p -> next -> y); // ɾ��β�� 
		go_to_xy(p -> next -> x, p -> next -> y);
		int i = p -> next -> x, j = p -> next -> y;
		//ɾ��β�ͺ�Ҫ���´�ӡԭ������Ϣ 
		if(sudoku[0][i][j] == 10) 
			{if(i == 12 || i == 24 || j == 6 || j == 12)
				color_change(i, j, 10);
			else
				color_change(i, j, 8);
			go_to_print(i, j);
			}
		else if(sudoku[sudoku_map_num][i][j] != 0 && value[sudoku_map_num][i][j] == 1) 
			{color_change(i, j, 2);
			printf("%d", sudoku[sudoku_map_num][i][j]);
			}
		go_to_xy(39, 28); //�ı���λ�ã���ֹ�γɺڿ� 
		free(p -> next); 
		//�ͷ�β����ڴ�ռ�
		p -> next = NULL;
		} 
	else if(changeflag == 1)
		{changeflag = 0;
		}
	//�������䳤��changeflag�ı���ɾ��β�ͣ�ʵ������䳤Ч������һ������䳤;�� 
	new_head = (snake*)malloc(sizeof(snake));
	new_head -> x = x;
	new_head -> y = y;
	new_head -> next = head;
	head = new_head;
	//�ߵ�����ṹ�ع����
	color_change(head -> x, head -> y, 7); 
	go_to_print(head -> x, head -> y); //��ӡͷ�� 
	go_to_xy(39, 28); //�ı���λ�ã���ֹ�γɺڿ� 
} 

/********************************/
//�ж������Ƿ���ɣ�����򷵻�1 
int sudoku_completed()
{
	int i, j;
	for(i = 2; i < 35; i += 4)
		for(j = 1; j < 18; j += 2)
			if(value[sudoku_map_num][i][j] == 0)
				return 0;
	//���κ�һ��û����ɣ�������0 
	return 1;
	//ȫ����ɣ��ŷ���1 
}

/********************************/
//������Ϸ�жϽ��� 
int game_over_judgement2()
{
	snake *p;
	if(head -> x == 0 || head -> x == 36 || head -> y == 0 || head -> y == 18)
		{end = time(NULL);
		finish();
		return 0;
		}
	//ײǽ����Ϸ���� 
	else if(sudoku_completed()) 
		{end = time(NULL); 
		finish();
		return 0;
		} 
	//������ɣ���Ϸ���� 
	else 
		{for(p = head -> next; p != NULL; p = p -> next)
			if(head -> x == p -> x && head -> y == p -> y)
				{end = time(NULL);
				finish();
				return 0;
				}
		//ײ��������Ϸ���� 
		return 1;
		//������Ϸ���� 
		}
}

/********************************/
//����̰�������ٶȸı�ı�׼ 
void speed_change2()
{
	int time; 
	//ֹͣ����ʱ�䣬����Ϊ��λ���ӵ�ǰ�÷ֶ��� 
	if(score[1][play_round1] <= 200) time = 300;
	else if(score[1][play_round1] > 200 && score[1][play_round1] <= 600) time =250;
	else if(score[1][play_round1] > 600 && score[1][play_round1] <= 1200) time = 200;
	else time = 150;
	Sleep(time); //ִ�й��� 
}

/********************************/
//���������ֺ�ı仯
void eating2()
{
	//��ɲ�ͬ���ֺ�ӷ��𽥼���  ǰ��ĸ��� 
	finish_num ++;
	changeflag = 1;
	if(finish_num <= 20) score[1][play_round1] += 30;
	else if(finish_num > 20 && finish_num <= 40) score[1][play_round1] += 20;
	else if(finish_num > 40 && finish_num <= 60) score[1][play_round1] += 10;
	else score[1][play_round1] += 5;
	color_change(18, 21, 7);
	printf("%d", score[1][play_round1]);
}

/********************************/
//�����չʾ�й�������Ϸ�������Ϣ
int print_info()
{
	color_change(50, 0, 7);
	printf("SNAKE");
	go_to_xy(4, 1);
	printf("Once upon a time, a gluttonous snake found a mysterious place with colorful and magical food...");
	go_to_xy(0, 3);
	printf("Rule:");
	go_to_xy(4, 4);
	printf("When your head bumps into your body or the wall, you die.");
	go_to_xy(0, 6);
	
	printf("Different Foods:");
	color_change(4, 7, 1);  printf("��"); 
	color_change(6, 7, 7);	printf(": speed down and only worth 5 points.");
	go_to_xy(4, 8); printf("(lonicera edulis, grown in cold areas with a frozing heart inside.)");
	color_change(4, 9, 2);  printf("��"); 
	color_change(6, 9, 7);	printf(": worth 50 points.");
	go_to_xy(4, 10); printf("(green jujube, highly nutritious and conductive to prolong life.)");
	color_change(4, 11, 3);  printf("��"); 
	color_change(6, 11, 7);	printf(": cut one node.(when you nodes surpass 2)");
	go_to_xy(4, 12); printf("(java banana, vanilla icecream taste! back to the childhood.)");
	color_change(4, 13, 4);  printf("��"); 
	color_change(6, 13, 7);	printf(": speed up.");
	go_to_xy(4, 14); printf("(cactus, furious power from the desert.)");
	color_change(4, 15, 5);  printf("��"); 
	color_change(6, 15, 7);	printf(": no change.");
	go_to_xy(4, 16); printf("(purple sweet potato, you eat and you excrete.)");
	color_change(4, 17, 6);  printf("��"); 
	color_change(6, 17, 7);	printf(": grow two nodes and get 20 points");
	go_to_xy(4, 18); printf("(yellow peach, exactly suit this snake's taste)");
	color_change(4, 19, 7);  printf("��"); 
	color_change(6, 19, 7);	printf(": grow one node and get 10 points.");
	go_to_xy(4, 20); printf("(casaba, common fruit with great taste.)");
	
	go_to_xy(0, 22); printf("Notes:");
	go_to_xy(4, 23); printf("1.In the normal situation, you will grow one node and get 10 points when you eat one food.");
	go_to_xy(4, 24); printf("2.As you eat more food, you move faster.");
	go_to_xy(4, 25); printf("3.Food appears completely randomly.");
	go_to_xy(4, 26); printf("4.Speed change effects of food will disapear when you got points more than 1000.");
	
	go_to_xy(47, 29);
	printf("SNAKE SUDOKU");
	go_to_xy(4, 30);
	printf("Once upon a time, a confused snake happend to get lost in a maze of numbers...");
	go_to_xy(0, 32);
	printf("Rule:");
	go_to_xy(4, 33);
	printf("Finish the sudoku. Only when the snake head moves to the right blank space can you fill the right number in.");
	go_to_xy(0, 35);
	
	printf("Notes:");
	go_to_xy(4, 36);
	printf("1.When your head bumps into your body or the blue wall, game is over.");
	go_to_xy(4, 37);
	printf("2.You can't fill the wrong number in the blank space.");
	go_to_xy(4, 38);
	printf("3.You will grow up, move faster and get more points according to your execution.");
	go_to_xy(4, 39);
	printf("3.Press any other buttons, and you can stop.");
	
	color_change(45, 40, 4);
	printf("This computer game is created by ZhangJian. All rights reserved."); 
	
	color_change(30, 42, 3);
	printf("Press 4 to return to the start interface.");
	color_change(30, 43, 7);
	char click;  
	while(1)
		{if(_kbhit())
			{click = _getch();
			if(click == '4')
				{system("cls");
				break;
				}	
			}
		}
	//����ִ�����룬ֱ�����ѡ�񷵻�ҳ�� 
	return 0;
}  
