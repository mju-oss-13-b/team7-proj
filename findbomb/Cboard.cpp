#include <iostream>
using namespace std;
#include <time.h>     
#include <windows.h>  
#include "CBoard.h"     
#define LEFT  0x4b		
#define RIGHT 0x4d		
#define UP    0x48		
#define DOWN  0x50		

CBoard::CBoard(void){} 


void CBoard::initialize(void)
{
	cout << "STAGE : 1.EASY  2.MIDDLE  3.HARD :";
	int a;
	cin >> a;
	switch (a)
	{
	case 1:
		number = 10;
		bomb = 10;
		break;
	case 2:
		number = 16;
		bomb = 40;
		break;
	case 3:
		number = 22;
		bomb = 99;
		break;
	}

	
	m_Blocks = new CBlock*[number];
	for (a = 0; a<number; a++) m_Blocks[a] = new CBlock[number];
	cursor_x = 0;       
	cursor_y = 0;       
	count_flag = 0;      
	count_bomb = bomb;    

}

void CBoard::SetBlocks(void) {
	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			m_Blocks[i][j].SetPos(i, j);
		}
	}
	
	srand(time(NULL));                     

	int randX, randY;
	int count = 0;
	while (count < bomb)
	{
		randX = rand() % number;             
		randY = rand() % number;
		if (m_Blocks[randX][randY].IsBomb()) 
		{
			continue;
		}
		else
		{
			m_Blocks[randX][randY].SetBomb(true);
			count++;
		}
	}
	
	int mineCount, i;
	for (i = 0; i < number; i++)             
	{
		for (int j = 0; j < number; j++)      
		{
			mineCount = 0;
			for (int k = -1; k <= 1; k++)     
			{
				for (int l = -1; l <= 1; l++)
				{
					if (i + k == -1 || i + k == number || j + l == -1 || j + l == number) continue;

					if (m_Blocks[i + k][j + l].IsBomb() == true)    
					{
						mineCount++;
					}
				}
			}
			m_Blocks[i][j].SetNumber(mineCount);   
		}
	}
}

CBoard::~CBoard(void)    
{


	for (int i = 0; i<number; i++)   
	{
		delete[] m_Blocks[i];
	}
	delete[] m_Blocks;
}

void CBoard::Draw()            
{

	system("cls");           
	cout << " ┏━━━━━━━━━━━━━┓\n"
		<< " ┃  ☆☆   FUNNY GAME   ☆☆┃\n"
		<< " ┗━━━━━━━━━━━━━┛\n" << endl << endl << endl;

	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			m_Blocks[i][j].Draw();      
		}
		cout << endl;
	}
	SetCursor(0, -2);         
	cout << "♣FLAGS : " << count_flag << "    ♣REST BOMBS : " << count_bomb;
	SetCursor(0, number + 2);  
	cout << "MOVEMENT : " << "←↑↓→  " << "FLAG : " << "f  " << "CLICK : " << "space bar  " << "FINISH : " << "ESC";
	SetCursor(cursor_x, cursor_y); 
}

void CBoard::ClickBlock(void)
{
	m_Blocks[cursor_y][cursor_x].Click();    
	if (m_Blocks[cursor_y][cursor_x].IsFlag()) ClickFlag(); 

	if (m_Blocks[cursor_y][cursor_x].IsBomb()) MineReplay(); 
	else{

		for (int i = -1; i <= 1; i++)      
		{
			for (int j = -1; j <= 1; j++)  
			{
				if (((i == 0) && (j == 0)) || (cursor_y + i < 0) || (cursor_y + i >= number)
					|| (cursor_x + j < 0) || (cursor_x + j >= number)
					)
					continue;
				if ((!m_Blocks[cursor_y + i][cursor_x + j].IsClicked()) &&  
					(!m_Blocks[cursor_y + i][cursor_x + j].IsBomb()) &&     
					m_Blocks[cursor_y][cursor_x].GetNumber() == 0      
					)
					this->ClickBlock(cursor_y + i, cursor_x + j);
			}
		}

	}
}

void CBoard::ClickBlock(int x, int y)                           
{                                      

	m_Blocks[x][y].Click();

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (((i == 0) && (j == 0)) || (x + i < 0) || (x + i >= number)
				|| (y + j < 0) || (y + j >= number))
				continue;
			if ((!m_Blocks[x + i][y + j].IsClicked()) &&
				(!m_Blocks[x + i][y + j].IsBomb())
				&& m_Blocks[x][y].GetNumber() == 0)
				this->ClickBlock(x + i, y + j);
		}
	}

}

void CBoard::SetCursor(int x, int y)    
{                                      
	COORD Cur;
	Cur.X = 2 * x;          
	Cur.Y = y + 6;         
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void CBoard::MoveCursor(int direction)
{
	switch (direction)
	{
	case LEFT:
		if (cursor_x != 0) --cursor_x;  
		break;
	case RIGHT:
		if (cursor_x != number - 1) ++cursor_x;
		break;
	case UP:
		if (cursor_y != 0) --cursor_y; 
		break;
	case DOWN:
		if (cursor_y != number - 1) ++cursor_y;  
		break;
	}
	SetCursor(cursor_x, cursor_y);  
}

void CBoard::Finish(void){
	system("cls");
	SetCursor(0, 4);
	cout << "                      The End  ... ";
	Sleep(1000); 
	system("cls");
	SetCursor(0, 4); 
	Sleep(2000);  
	system("cls");
	SetCursor(0, 7);
	Sleep(4000);  


	exit(0);     

}

void CBoard::ClickFlag(void)
{
	if (!m_Blocks[cursor_y][cursor_x].IsClicked() || m_Blocks[cursor_y][cursor_x].IsFlag())
	{
		if (m_Blocks[cursor_y][cursor_x].IsFlag())     
		{
			count_flag--;                            
			if (m_Blocks[cursor_y][cursor_x].IsBomb())
			{
				count_bomb++;                        
			}
		}
		else if (m_Blocks[cursor_y][cursor_x].IsBomb())
		{
			count_bomb--;	                          
			count_flag++;                             
		}
		else count_flag++;	                          



		m_Blocks[cursor_y][cursor_x].Flag();             

		if (count_flag == bomb && count_bomb == 0) Replay(); 
	}	 	                                         
}

void CBoard::Replay(){

	system("cls");
	SetCursor(0, number + 2);
	cout << "CLEAR!!! 축하합니다 ~!\n";
	int a;
	cout << "겜을 다시 하시겠습니까? : 1.yes 2.no:";
	cin >> a;
	if (a == 1)
	{
		for (int i = 0; i<number; i++)         
			delete[] m_Blocks[i];          

		delete[] m_Blocks;
		initialize();                    
		SetBlocks();
	}

	else Finish();
}

void CBoard::MineReplay(void)
{
	for (int i = 0; i < number; i++)
	{
		for (int j = 0; j < number; j++)
		{
			m_Blocks[i][j].Click();   
		}
	}
	Draw();
	SetCursor(0, number + 3);

	int a;
	cout << endl
		<< "           @@@          @@@ " << endl
		<< "          @@@          @@@  " << endl
		<< "         @@@          @@@   " << endl
		<< "        @@@          @@@    " << endl
		<< "       @@@          @@@     " << endl
		<< "      @@@          @@@      " << endl
		<< "     @@@          @@@       " << endl
		<< "    @@@          @@@        " << endl
		<< "                            " << endl
		<< "                            " << endl
		<< "   @@@          @@@         " << endl
		<< "   @@           @@          " << endl << endl << endl;
		

	cout << "Play Again? : 1.yes 2.no:";
	cin >> a;
	if (a == 1)
	{
		for (int i = 0; i<number; i++)          
			delete[] m_Blocks[i];            

		delete[] m_Blocks;
		initialize();                       
		SetBlocks();
	}
	else Finish();
}
