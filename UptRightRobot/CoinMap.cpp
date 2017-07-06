#include "StdAfx.h"
#include "CoinMap.h"


CoinMap::CoinMap(int nWidth, int nHeight)
{
	Reset(nWidth, nHeight);
}


CoinMap::~CoinMap(void)
{
}

void CoinMap::Reset( int nWidth, int nHeight )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_vvecCoinMap.clear();
	m_vvecCoinMap.resize(nWidth);
	vector<bool> vecColumn;
	vecColumn.resize(nHeight);
	for(int i = 0; i  < nWidth; i++)
	{
		m_vvecCoinMap[i] = vecColumn;
	}
}

void CoinMap::AutoFillCoin( int nRatio )
{
	if (nRatio < 1)
	{
		nRatio = 1;
	}
	srand(time(0) + clock() / nRatio);
	for (int x = 0; x < m_nWidth; x++)
	{
		for (int y = 0; y < m_nHeight; y++)
		{
			if (rand() % nRatio == 0)
			{
				m_vvecCoinMap[x][y] = true;
			}
		}
	}
}

void CoinMap::SetCoin( int x, int y )
{
	if (CheckPos(CPoint(x, y)))
	{
		m_vvecCoinMap[x][y] = !m_vvecCoinMap[x][y];
	}
}

bool CoinMap::HasCoin( int x, int y )
{
	bool bRet = false;
	if (CheckPos(CPoint(x, y)) && m_vvecCoinMap[x][y])
	{
		bRet = true;
	}
	return bRet;
}

vector<vector<bool> > CoinMap::GetMap()
{
	return m_vvecCoinMap;
}

bool CoinMap::CheckPos( const CPoint & pos )
{
	bool bRet = false;
	if (pos.x < m_nWidth && pos.y < m_nHeight && pos.x >= 0 && pos.y >= 0)
	{
		bRet = true;
	}
	return bRet;
}

int CoinMap::GetWidth()
{
	return m_nWidth;
}

int CoinMap::GetHeight()
{
	return m_nHeight;
}
