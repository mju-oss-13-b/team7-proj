#include<iostream>
#include<conio.h>       
using namespace std;
#include "CBlock.h"
#include "CBoard.h"
#define LEFT  0x4b		
#define RIGHT 0x4d		
#define UP    0x48		
#define DOWN  0x50		
#define ESC 27          
int main()
{
	CBoard mine;        
	char in;
	mine.initialize();  
	mine.SetBlocks();   
	mine.Draw();        

	
	while (1)
	{
		in = _getch();
		if (in == LEFT || in == RIGHT || in == UP || in == DOWN)
		{
			mine.MoveCursor(in);
		}

		else if (in == ESC)
		{
			mine.Finish();
			exit(0);
		}
		else if (in == ' ')
		{
			mine.ClickBlock();
			mine.Draw();
		}
		else if (in == 'f')
		{
			mine.ClickFlag();
			mine.Draw();


		}

	}


	return 0;

}