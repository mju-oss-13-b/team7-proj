#include "CBlock.h"
#define LEFT  0x4b		
#define RIGHT 0x4d		
#define UP    0x48		
#define DOWN  0x50		
class CBoard
{

private:
	CBlock	**m_Blocks;   
	int bomb;           
	int number;         
	int count_bomb;       
	int count_flag;      
	int cursor_x;         
	int cursor_y;        

public:

	CBoard(void);                      
	void initialize(void);              

	~CBoard(void);						
	void Draw();						
	void SetBlocks(void);               
	void ClickBlock(int x, int y);       
	void ClickBlock(void);              
	void MoveCursor(int direction);    
	void SetCursor(int x, int y);      
	void Finish(void);                 
	void ClickFlag(void);               
	void Replay(void);                  
	void MineReplay(void);              
};
