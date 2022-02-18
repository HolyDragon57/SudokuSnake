/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> //生成以时间为基础的随机数函数所在库 
#include<conio.h> //获取键盘输入函数所在库 
#include<windows.h> //获取并改变光标位置函数所在库 

#pragma comment(lib, "winmm.lib")//播放背景音乐需要指令 

#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'

//两个小游戏通用的函数
int start();//开始界面
int finish();//结束界面 
void init_score();//初始化得分 
void create_snake();//蛇链表初始化并打印 
void go_to_xy(int x, int y);//光标重定位并隐藏光标 但等待输入时会覆盖原有打印，容易形成黑块 
void go_to_print(int x, int y);//在指定位置打印
void go_to_delete(int x, int y);//在指定位置擦除 
void color_change(int x, int y, int color); //在指定位置使用指定颜色 
void direction_change(int *x, int *y, int click);// 改变位置的细节操作 
int print_info();//向玩家展示两个游戏的相关信息 

//经典贪吃蛇使用函数 
void complete_map_info(); //完善地图信息 
void build_graph();//建图
int read_clicks();//判断玩家是否操作以及操作方向
int opposite(char click, char old_click);//判断玩家操作是否与上一次操作恰好相反 
void changing_body(int x, int y);//改变位置 
void eating();//吃到食物后改变长度 
void growing();//蛇身变长
void speed_change();//蛇速度控制改变
void situation_change(int old_color);//蛇吃到不同果实的情况变化 
void food_appearing();//食物随机出现
int game_over_judgement();//判断游戏是否结束
int in_or_out_judgement(int x, int y); //判断坐标（x,y）是否存在于墙内

//贪吃蛇数独使用函数 
void complete_sudoku_info();//完善数独信息  
void build_sudoku();//在控制台上画数独图 
int read_clicks2();//贪吃蛇数独中读取玩家操作 
void changing_body2(int x, int y);//贪吃蛇数独中改变位置 
int sudoku_completed();//判断数独是否完成
int game_over_judgement2();//贪吃蛇数独中结束判断条件略有不同 
void speed_change2();//贪吃蛇数独中蛇的速度应该更慢 
void eating2();//贪吃蛇数独中积分规则不同 

typedef struct Snake
{
	int x;
	int y;
	struct Snake *next;
}snake;
//蛇的链表结构 

snake *head;  

struct Food
{
	int x;
	int y;
}food;

int map[10][80][40];//第一个坐标为地图序号，后两个坐标为横纵坐标 
int sudoku[10][50][30];//第一个坐标为数独序号，最后两个坐标为横纵坐标 
int value[10][50][30];//数独中数字的状态 ,1代表显现，2代表隐藏 
int map_num;//存储随机生成地图序号 
int sudoku_map_num;//存储随机生成数独序号 
int score[2][100];//不同游戏的得分 
int play_round0 = 0;//记录玩家贪吃蛇局数
int play_round1 = 0;//记录玩家贪吃蛇数独局数
int personal_best0 = 0;//记录贪吃蛇最高分数 
int personal_best1 = 0;//记录贪吃蛇数独最高分数 
int color;//食物颜色 
int speed = 0;//速度改变参照 
char coin; //玩家选择游戏模式 
int finish_num = 0;//玩家已填写数字个数 
int changeflag = 0;//判断蛇增长状态 
int end_or_not; //判断结束标志 
int begin, end; //两个时间变量记录数独完成时间 

/********************************/
//主函数 
int main()
{
	//玩家可能反复玩游戏，因此要放置在循环里 
	//mciSendString("open background.mp3 alias start", NULL, 0, NULL);
	//打开音乐文件 
	//mciSendString("play start repeat", NULL, 0, NULL);
	//进入游戏就开始播放背景音乐 
	init_score();
	while(1)
		{system("cls");
		while(start())
			{
			}
		//限定控制台只对玩家按特定按键有回馈，否则继续执行 
		if(coin == '1') 
			{build_graph();
			food_appearing();
			if(read_clicks() == 0) 
				if(end_or_not == 1)
					break;
			//不断读取玩家操作，直至游戏结束 
			play_round0 ++;
			}
		
		else if(coin == '2')
			{build_sudoku();
			if(read_clicks2() == 0)
				if(end_or_not == 1)
					break;
			play_round1 ++;
			}
		finish_num = 0;//数独完成个数初始化，防止累积 
		}
	//游戏结束背景音乐终止，关闭音乐文件。 
	//mciSendString("stop start", NULL, 0, NULL);
	//mciSendString("close start", NULL, 0, NULL);
	//消除四个mciSendString函数前的注释符号即可循环播放背景音乐，但贪吃蛇会偶现左上角墙砖消失的BUG。 
	return 0;
}

/********************************/
//初始化分数
void init_score()
{
	int i;
	for(i = 0; i < 100; i ++);
		score[0][i] = score[1][i] = 0;
} 

/********************************/
//造蛇
void create_snake()
{
	snake *q;
	head = (snake*)malloc(sizeof(snake));
	q = (snake*)malloc(sizeof(snake));
	head -> x = 24; head -> y = 12;
	q -> x = 24; q -> y = 13;
	head -> next = q;
	q -> next = NULL;
	//初始蛇的位置 
	color_change(head -> x, head -> y, 7);
	go_to_print(head -> x, head -> y);
	go_to_print(q -> x, q -> y);
	go_to_xy(16, 27);//光标移开，防止形成黑块 
} 

/********************************/
//移动到指定位置 
void go_to_xy(int x, int y)
{
	COORD new_pos;
	HANDLE old_pos = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前光标位置 
	new_pos.X = x;
	new_pos.Y = y;
	SetConsoleCursorPosition(old_pos, new_pos);//更新光标位置
	
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE; //光标不可见 
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(old_pos, &cursor);//更新光标信息，隐藏光标 
}

/********************************/
//指定位置打印 
void go_to_print(int x, int y)
{
	go_to_xy(x, y);
	printf("■");
}

/********************************/
//指定位置删除 
void go_to_delete(int x, int y)
{
	go_to_xy(x, y);
	printf("  ");
}

/********************************/
//将光标移动至指定位置并改变前景字体颜色 
void color_change(int x, int y, int color)
{
	go_to_xy(x, y);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color); //改变当前句柄的前景字体颜色 
} 

/********************************/
//有关方向改变的具体细节 
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
	//注意此处x的变化间隔为2 
} 

/********************************/
//开始界面 
int start()
{
	int x;
	srand((int)time(NULL));
	go_to_xy(11, 3);
	printf("  ■■■    ■      ■      ■      ■    ■■  ■■■■■");
	go_to_xy(11, 4);
	printf("■          ■■    ■    ■  ■    ■  ■      ■");
	go_to_xy(11, 5);
	printf("  ■■■    ■  ■  ■  ■■■■■  ■■        ■■■■");
	go_to_xy(11, 6);
	printf("        ■  ■    ■■  ■      ■  ■  ■      ■");
	go_to_xy(11, 7);
	printf("  ■■■    ■      ■  ■      ■  ■    ■■  ■■■■■");
	go_to_xy(35, 9);
	                        printf("■      ■");
	go_to_xy(35, 10);
	                        printf("■■  ■■");
	go_to_xy(35, 11);
	                        printf("■      ■");
	go_to_xy(37, 12);
	                          printf("■■■");
	go_to_xy(39, 13);
	                            printf("■");
	go_to_xy(37, 14);
	                          printf("■  ■");
	go_to_xy(13, 15);
	printf("请在英文输入法中操作，wasd控制方向，其余任意键暂停");
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
		system("cls"); //清屏操作 
		return 0;
		}
	else if(coin == '2')
		{complete_sudoku_info();
		sudoku_map_num = rand() % 5; 
		system("cls"); //清屏操作
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
		//将coin值初始化，防止干扰后续操作 
		return 1;
		}
	else return 1;
}

/********************************/
//结束界面 
int finish()
{
	char click;
	system("cls"); //清屏 
	color_change(35, 8, 7); 
	int points, time;
	//根据游戏不同给出不同的结算界面 
	if(coin == '1') points = score[0][play_round0];
	else if(coin == '2') points = score[1][play_round1];
	printf("YOUR SCORE: %d", points);
	time = end - begin;
	//记录数独用时 
	go_to_xy(35, 9);
	//数独要单独展示结算时间 
	if(coin == '2')
		printf("YOUR TIME: %d", time);
	go_to_xy(30, 12);
	if(points <= 300)
	    printf("Play more carefully next time!");
	else if(points > 300 && points <= 1000)
		printf("You can do better next time!");
	else
		printf("Good job! play for the highest record next time!");
	//对不同分段给予评价 
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
	//玩家下一步操作提示 
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
	//判断玩家是想要继续游戏还是结束游戏 
	if(end_or_not == 1)
		{snake *p, *q;
		for(p = head; p != NULL; p = q)
			{q = p -> next;
			free(p);
			}
		//释放链表内存 
		return 0;
		}
	else
		return 0;
}

/********************************/
//完善地图建模   或许之后可以用读取文件方式完成（数独同理） 
void complete_map_info()
{
	int i, j;
	for(i = 0; i < 80; i ++)
		for(j = 0; j < 40; j ++)
			map[0][i][j] = map[1][i][j] = map[2][i][j] = map[3][i][j] = map[4][i][j] = map[5][i][j] = 0;
	//初始化  赋值为1处将要装填墙砖 
	for(i = 0; i < 60; i += 2)                  map[0][i][0] = map[0][i][24] = 1;
	for(j = 1; j < 24; j ++)                    map[0][0][j] = map[0][58][j] = 1;
	//地图0建模完成   30*25矩形 
	for(i = 0; i < 17; i +=2)                   map[1][i][8 - (i/2)] = 1;
	for(j = 9; j < 17; j ++)                    map[1][0][j] = 1;
	for(i = 2, j = 17; i < 17; i += 2, j ++)    map[1][i][j] = 1;
	for(i = 18; i < 33; i += 2)                 map[1][i][24] = 1;
	for(i = 34, j = 23; i < 49; i += 2, j --)   map[1][i][j] = 1;
	for(j = 15; j > 8; j --)                    map[1][48][j] = 1;
	for(i = 48, j = 8; j > 0; i -=2, j --)      map[1][i][j] = 1;
	for(i = 18; i < 33; i ++)                   map[1][i][0] = 1;
	//地图1建模完成   25*25八边形。
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
	//地图2建模完成   25*25十字
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
	//地图3建模完成   26*26沙漏 
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
	//地图4建模完成   24*26回廊 
} 

/********************************/
//建图 
void build_graph()
{
	int i, j;
	//根据三维数组建图 
	for(j = 0; j < 40; j ++)
		for(i = 0; i < 80; i += 2)
			if(map[map_num][i][j] == 1)
				go_to_print(i, j);
	create_snake(); 
	printf("Your Score: %d     =￣ω￣=", score[0][play_round0]);
}

/********************************/
//读取玩家操作 
int read_clicks()
{
	int x = head -> x, y = head -> y;
	char click = 1, old_click = 1;
	while(1)
		{if(game_over_judgement() == 0) return 0; //循环终止条件为游戏结束 
		if(_kbhit()) //非阻塞函数：有按键按下非零，无按键按下为零。 
			{click = _getch();//会在缓冲区存储之前按下的按键，阻塞函数，不按下不会继续 
			if(opposite(click, old_click) == 1) //如果按键与之前相反，则等待下一次按键，解决了向相反反向移动导致死亡的问题 
				click = old_click;
			}
		old_click = click;
		direction_change(&x, &y, click); 
		if(x != head -> x || y != head -> y)
			changing_body(x, y);
		//一旦开始操作，蛇就开始运动 
		eating();
		//实时监测蛇是否吃到食物 
		}
} 

/********************************/
//判断是否与上一次操作正好相反 
int opposite(char click, char old_click)
{
	if(click == 'w' && old_click == 's') return 1;
	if(click == 'a' && old_click == 'd') return 1;
	if(click == 's' && old_click == 'w') return 1;
	if(click == 'd' && old_click == 'a') return 1;
	else return 0;
}


/********************************/
//蛇的身体的移动 
void changing_body(int x, int y)
{
	//通过删除尾巴和打印头部实现动态化效果 
	snake *p, *new_head;
	for(p = head; p -> next -> next != NULL; p = p -> next) ;
	go_to_delete(p -> next -> x, p -> next -> y); // 删除尾巴 
	go_to_xy(39, 28); //改变光标位置，防止形成黑块 
	free(p -> next); //释放尾结点内存空间
	p -> next = NULL;
	new_head = (snake*)malloc(sizeof(snake));
	new_head -> x = x;
	new_head -> y = y;
	new_head -> next = head;
	head = new_head;
	//蛇的链表结构重构完成
	go_to_print(head -> x, head -> y); //打印头部 
	go_to_xy(39, 28); //改变光标位置，防止形成黑块 
} 

/********************************/
//贪吃蛇吃到食物后的变化 
void eating()
{
	speed_change(); //调整速度 
	if(food.x == head -> x && food.y == head -> y)
		{color_change(head -> x, head -> y, color); 
		int old_color = color; 
		food_appearing(); 
		//出现新食物 
		situation_change(old_color); 
		//根据吃到的食物调整贪吃蛇的状态 
		color_change(28, 27, 7); 
		//移动光标至打印得分处 
		printf("%d", score[0][play_round0]);
		color_change(39, 28, old_color); 
		//移开光标，防止形成黑块 
		}
}

/********************************/
//蛇变长 
void growing()
{
	//链表长度变长 
	snake *q, *new_tail;
	new_tail = (snake*)malloc(sizeof(snake));
	for(q = head; q -> next != NULL; q = q -> next) ;  
	q -> next = new_tail;
	new_tail -> next = NULL;
}

/********************************/
//速度控制与改变 
void speed_change() 
{
	int time; 
	//停止操作时间，毫秒为单位 
	//根据当前玩家获得分数改变屏幕挂起时间，修改游戏难度 
	if(score[0][play_round0] <= 500 && speed == 0) time = 150;
	else if(score[0][play_round0] <= 500 && speed == 1) time = 120;
	else if(score[0][play_round0] <= 500 && speed == -1) time = 180;
	//一阶难度 
	else if(score[0][play_round0] > 500 && score[0][play_round0] <= 1000 && speed == 0) time = 100;
	else if(score[0][play_round0] > 500 && score[0][play_round0] <= 1000 && speed == 1) time = 80;
	else if(score[0][play_round0] > 500 && score[0][play_round0] <= 1000 && speed == -1) time = 120;
	//二阶难度 
	else if(score[0][play_round0] > 1000 && score[0][play_round0] <= 1500) time = 50;
	//三阶难度 
	else time = 10;
	//地狱难度 
	Sleep(time); //执行挂起 
} 

/********************************/
//吃到不同果实的情况分析 
void situation_change(int old_color)
{
	if(old_color == 7)
		{growing();
		score[0][play_round0] += 10;
		speed = 0; 
		}//尾巴长度变长一节 白色果实 
	else if(old_color == 6)
		{growing();
		growing(); 
		score[0][play_round0] += 20;
		speed = 0; 
		}//尾巴长度变长两节  黄色果实
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
		} //尾巴长度缩短一节 两节时不缩短  青色果实
	else if(old_color == 2)
		{speed = 0;
		growing(); 
		score[0][play_round0] += 50; //加分50  绿色果实 
		}
	else if(old_color == 4)
		{speed = 1;
		growing();
		score[0][play_round0] += 10;  
		}//速度加快（1000分以下） 红色果实
	else if(old_color == 1)
		{speed = -1;
		growing();
		score[0][play_round0] += 5; 
		}//速度减慢且只加5分（1000分以下）  蓝色果实
	else
		{speed = 0;
		score[0][play_round0] += 10;
		}//紫色果实 长度不变化
} 

/********************************/
//食物随机出现 
void food_appearing()
{
	//产生多组随机位置，判断每一个位置是否合理，直到出现一个合适的位置 
	int flag = 1;
	while(flag)
		{flag = 0;
		//首先使之出现在墙内 
		if(map_num == 0)
			{food.x = ((rand() % (30 - 2)) + 1) * 2; //只能出现在2-length*2 -4中的偶数位置 
			food.y = rand() % (25 - 2) + 1; //只能出现在1-width-1行 
			}
		else
			{food.x = (rand() % 24 + 1) * 2;
			food.y = rand() % 24 + 1; 
			//通过缩小范围从而缩短时间 
			}
		if(in_or_out_judgement(food.x, food.y) == 0) //判断是否位于墙内或墙上 
			flag = 1;
		//然后监测是否与蛇身重合 
		snake *p; 
		for(p = head; p != NULL; p = p -> next)
			if(p -> x == food.x && p -> y == food.y)
				flag = 1;
		} 
	color = rand() % 7 + 1;
	color_change(food.x, food.y, color);
	printf("⊙"); 
	//随机产生不同颜色果实 
	color_change(39, 28, 7); 
	//改变回白色 并移动光标至它处防止形成黑块 
}

/********************************/
//判定游戏结束条件 
int game_over_judgement()
{
	snake *p;
	if(in_or_out_judgement(head -> x, head -> y) == 0)
		{finish();
		return 0; 
		}
	//撞墙，游戏结束 
	else 
		{for(p = head -> next; p != NULL; p = p -> next)
			if(head -> x == p -> x && head -> y == p -> y)
				{finish();
				return 0;
				}
		//撞到蛇身，游戏结束 
		return 1;
		//否则，游戏继续 
		}
}

/********************************/
//判断坐标是否位于闭合地图内部 
int in_or_out_judgement(int x, int y)
{
	int flag = 0, i;
	//与墙重合则显然不行。 
	if(map[map_num][x][y] == 1) return 0;
	//通过判断四个方位是否均存在墙来判断是否位于墙内，对某些极端特殊地图不适用 
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
//从文件中读取完整数独信息和最开始数字的显隐情况（1表示显现，0表示隐藏 
void complete_sudoku_info()
{
	FILE *in;
	int i, j, k;
	in = fopen("sudoku.txt", "r");
	if(in == NULL)
		printf("CANNOT OPEN FILE!");
	//问题文件读取报错 
	for(k = 0; k < 5; k ++)
		for(i = 0; i < 50; i ++)
			for(j = 0; j < 30; j ++)
				sudoku[k][i][j] = 0;
	//初始化三维数组 
	for(k = 0; k < 5; k ++)
		{for(j = 1; j < 18 ; j += 2)
			for(i = 2; i < 35; i += 4)
				fscanf(in, "%d", &sudoku[k][i][j]);
		//读取数字信息 
		for(j = 0; j < 19; j += 2)
			for(i = 0; i < 37; i += 2)
				sudoku[k][i][j] = 10;
		for(i = 0; i < 37; i += 4)
			for(j = 0; j < 19; j ++)
				if(sudoku[k][i][j] != 10)
					sudoku[k][i][j] = 10;
		//设定边界 
		for(j = 1; j < 18 ; j += 2)
			for(i = 2; i < 35; i += 4)
				fscanf(in, "%d", &value[k][i][j]);
		//读取数字状态 
		}
	fclose(in); 
	//关闭文件 
}

/********************************/
//在控制台上画出数独图 
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
			//墙内数字分隔线为灰色 九宫格分割线为绿色 
			else if(value[sudoku_map_num][i][j] == 1)
				{color_change(i, j, 2);
				printf("%d", sudoku[sudoku_map_num][i][j]);
				} 
			//显现数字颜色为绿色 
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
	//打印边界蓝色墙，并赋值11，方便后续游戏结束判断 
	color_change(6, 21, 7);
	printf("Your Score: %d      =￣ω￣=", score[1][play_round1]); 
	finish_num = 0; 
	//玩家已填写数字初始化为零 
} 

/********************************/
//贪吃蛇数独中读取玩家操作 
int read_clicks2()
{
	begin = time(NULL);
	//记录开始时时间 
	create_snake();
	int x = head -> x, y = head -> y;
	char click = 1, old_click = 1;
	while(1)
		{if(game_over_judgement2() == 0) return 0; //循环终止条件为游戏结束 
		if(_kbhit()) //非阻塞函数：有按键按下非零，无按键按下为零。 
			{click = _getch();//会在缓冲区存储之前按下的按键，阻塞函数，不按下不会继续 
			if(isalpha(click))
				{if(opposite(click, old_click) == 1) //如果按键与之前相反，则等待下一次按键，解决了向相反反向移动导致死亡的问题 
					click = old_click;
				else old_click = click; 
				//否则正常操作，并存储上一次方向 便于填数字时使用 
				}
			}
		//是字母则改变方向 
		if(isalpha(click)) 
			direction_change(&x, &y, click);
		//此处注意x的变化间隔为2 
		else if(isdigit(click))
			{if(sudoku[sudoku_map_num][x][y]!=0&&sudoku[sudoku_map_num][x][y]!=10)
				if(sudoku[sudoku_map_num][x][y]!=11&&value[sudoku_map_num][x][y]==0&&(click-'0')==sudoku[sudoku_map_num][x][y])
				{eating2();
				value[sudoku_map_num][x][y] = 1;
				}
			//判断答案是否正确，正确则改变蛇身长度，得分，真值 
			click = '0';
			//一定要初始化，不然蛇会一直存储这个数字直到移动到正确位置直接打印 
			if(_kbhit())
				{click = _getch();
				if(isalpha(click))
					{if(opposite(click, old_click) == 1) 
						click = old_click;
					else old_click = click;
					}
				} 
			//数字在角落时，操作频率小于屏幕挂起时间，易导致撞墙 
			direction_change(&x, &y, old_click);
			} 
		if(x != head -> x || y != head -> y)
			changing_body2(x, y);
		//移动蛇身 
		speed_change2();
		//刷新屏幕 
		}
}

/********************************/
//数独内蛇的移动，与贪吃蛇略不同 
void changing_body2(int x, int y)
{
	//通过删除尾巴和打印头部实现动态化效果 
	snake *p, *new_head;
	for(p = head; p -> next -> next != NULL; p = p -> next) ;
	//找出蛇尾 
	if(changeflag == 0) 
		{go_to_delete(p -> next -> x, p -> next -> y); // 删除尾巴 
		go_to_xy(p -> next -> x, p -> next -> y);
		int i = p -> next -> x, j = p -> next -> y;
		//删除尾巴后要重新打印原数独信息 
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
		go_to_xy(39, 28); //改变光标位置，防止形成黑块 
		free(p -> next); 
		//释放尾结点内存空间
		p -> next = NULL;
		} 
	else if(changeflag == 1)
		{changeflag = 0;
		}
	//如果蛇身变长，changeflag改变则不删除尾巴，实现蛇身变长效果，另一种蛇身变长途径 
	new_head = (snake*)malloc(sizeof(snake));
	new_head -> x = x;
	new_head -> y = y;
	new_head -> next = head;
	head = new_head;
	//蛇的链表结构重构完成
	color_change(head -> x, head -> y, 7); 
	go_to_print(head -> x, head -> y); //打印头部 
	go_to_xy(39, 28); //改变光标位置，防止形成黑块 
} 

/********************************/
//判断数独是否完成，完成则返回1 
int sudoku_completed()
{
	int i, j;
	for(i = 2; i < 35; i += 4)
		for(j = 1; j < 18; j += 2)
			if(value[sudoku_map_num][i][j] == 0)
				return 0;
	//有任何一空没有完成，都返回0 
	return 1;
	//全部完成，才返回1 
}

/********************************/
//数独游戏判断结束 
int game_over_judgement2()
{
	snake *p;
	if(head -> x == 0 || head -> x == 36 || head -> y == 0 || head -> y == 18)
		{end = time(NULL);
		finish();
		return 0;
		}
	//撞墙，游戏结束 
	else if(sudoku_completed()) 
		{end = time(NULL); 
		finish();
		return 0;
		} 
	//数独完成，游戏结束 
	else 
		{for(p = head -> next; p != NULL; p = p -> next)
			if(head -> x == p -> x && head -> y == p -> y)
				{end = time(NULL);
				finish();
				return 0;
				}
		//撞到蛇身，游戏结束 
		return 1;
		//否则，游戏继续 
		}
}

/********************************/
//数独贪吃蛇中速度改变的标准 
void speed_change2()
{
	int time; 
	//停止操作时间，毫秒为单位，视当前得分而定 
	if(score[1][play_round1] <= 200) time = 300;
	else if(score[1][play_round1] > 200 && score[1][play_round1] <= 600) time =250;
	else if(score[1][play_round1] > 600 && score[1][play_round1] <= 1200) time = 200;
	else time = 150;
	Sleep(time); //执行挂起 
}

/********************************/
//描述填数字后的变化
void eating2()
{
	//完成不同数字后加分逐渐减少  前面的更难 
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
//向玩家展示有关两个游戏的相关信息
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
	color_change(4, 7, 1);  printf("⊙"); 
	color_change(6, 7, 7);	printf(": speed down and only worth 5 points.");
	go_to_xy(4, 8); printf("(lonicera edulis, grown in cold areas with a frozing heart inside.)");
	color_change(4, 9, 2);  printf("⊙"); 
	color_change(6, 9, 7);	printf(": worth 50 points.");
	go_to_xy(4, 10); printf("(green jujube, highly nutritious and conductive to prolong life.)");
	color_change(4, 11, 3);  printf("⊙"); 
	color_change(6, 11, 7);	printf(": cut one node.(when you nodes surpass 2)");
	go_to_xy(4, 12); printf("(java banana, vanilla icecream taste! back to the childhood.)");
	color_change(4, 13, 4);  printf("⊙"); 
	color_change(6, 13, 7);	printf(": speed up.");
	go_to_xy(4, 14); printf("(cactus, furious power from the desert.)");
	color_change(4, 15, 5);  printf("⊙"); 
	color_change(6, 15, 7);	printf(": no change.");
	go_to_xy(4, 16); printf("(purple sweet potato, you eat and you excrete.)");
	color_change(4, 17, 6);  printf("⊙"); 
	color_change(6, 17, 7);	printf(": grow two nodes and get 20 points");
	go_to_xy(4, 18); printf("(yellow peach, exactly suit this snake's taste)");
	color_change(4, 19, 7);  printf("⊙"); 
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
	//不断执行输入，直到玩家选择返回页面 
	return 0;
}  
