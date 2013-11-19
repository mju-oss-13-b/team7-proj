#include <iostream>
using namespace std;
#include "CBlock.h"

CBlock::CBlock(void)       
{
	m_bClicked = false;     
	m_bFlag = false;         
	m_bBomb = false;         
	m_nNumber = 0;           
}

CBlock::~CBlock(void)
{
}
    
void CBlock::Flag() {
	if (m_bFlag == true) m_bFlag = false;
	else m_bFlag = true;
}
void CBlock::Draw()       
{

	if (m_bFlag)                 
	{
		cout << "¢¹";
	}
	else if (!m_bClicked)         
	{
		cout << "¡á";
	}
	else
	{
		if (m_bBomb)                      
		{
			cout << "£ª";
		}
		else
		{
			if (m_nNumber == 0)           
			{
				cout << "¡à";
			}
			else
			{
				cout << " " << GetNumber();      
			}
		}
	}
}

