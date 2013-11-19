#pragma once 
				
				
				


class CBlock
{

	
private:
	int m_iPosX, m_iPosY;      
	int m_nNumber;             
	bool m_bClicked;          
	bool m_bFlag;             
	bool m_bBomb;              

	
public:
	void Draw();                                  
	void Click() { m_bClicked = true; }          
	void Flag();                                 
	void SetPos(int x, int y) { m_iPosX = x; m_iPosY = y; } 
	bool IsFlag() const { return m_bFlag; }          
	bool IsBomb() const { return m_bBomb; }       
	bool IsClicked() const { return m_bClicked; } 
	int GetNumber() const { return m_nNumber; }   
	void SetBomb(bool bBomb) { m_bBomb = bBomb; } 
	void SetNumber(int number) { m_nNumber = number; }


	CBlock(void);     

	~CBlock(void);     
};
