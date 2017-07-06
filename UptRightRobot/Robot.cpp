#include "StdAfx.h"
#include "Robot.h"
#include <algorithm>

Robot::Robot(void)
{
	m_emStrategy = UpFist;
	m_nWidth = 0;
	m_nHeight = 0;
	m_mapCoinCost.clear();
	m_vvecCoinMap.clear();
	m_ptCurent.SetPoint(0, 0);
	m_vecPath.clear();
	m_nCoinCount = 0;
	m_strDebug.Empty();
}


Robot::~Robot(void)
{
}

vector<CPoint> Robot::SearchCoin( bool bIsUp, const CPoint & pos )
{
	vector<CPoint> vecRet;
	if (CheckPos(pos))
	{
		if (bIsUp)
		{
			int x = pos.x;
			for (int y = pos.y - 1; y >= 0; y--)
			{
				if (CheckPos(CPoint(x, y)) && m_vvecCoinMap[x][y])
				{
					vecRet.push_back(CPoint(x, y));
				}
			}
		}
		else
		{
			int y = pos.y;
			for (int x = pos.x + 1; x < m_nWidth; x++)
			{
				if (CheckPos(CPoint(x, y)) && m_vvecCoinMap[x][y])
				{
					vecRet.push_back(CPoint(x, y));
				}
			}
		}
	}
	return vecRet;
}

bool Robot::CheckPos( const CPoint & pos )
{
	bool bRet = false;
	if (pos.x < m_nWidth && pos.y < m_nHeight && pos.x >= 0 && pos.y >= 0)
	{
		bRet = true;
	}
	return bRet;
}

vector<CPoint> Robot::SearchCost( const CPoint & pos )
{
	vector<CPoint> vecRet;
	if (CheckPos(pos))
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			for (int y = 0; y < m_nHeight; y++)
			{
				if ((x < pos.x && y < pos.y) || (x > pos.x && y > pos.y))
				{
					if (m_vvecCoinMap[x][y])
					{
						vecRet.push_back(CPoint(x, y));
					}
				}
			}
		}
	}
	return vecRet;
}

vector<CPoint> Robot::UnionCost( const vector<vector<CPoint> > & vvecCost )
{
	vector<CPoint> vecRet;
	//先合并再去重
	for (int i = 0; i < vvecCost.size(); i++)
	{
		vecRet.insert(vecRet.end(), vvecCost[i].begin(), vvecCost[i].end());
	}
	sort(vecRet.begin(), vecRet.end());
	vector<CPoint>::iterator itUnique = unique(vecRet.begin(), vecRet.end());
	vecRet.erase(itUnique, vecRet.end());
	return vecRet;
}

void Robot::Reset(int x/* = 0*/, int y/* = 0*/)
{
	m_emStrategy = UpFist;
	m_nWidth = 0;
	m_nHeight = 0;
	m_mapCoinCost.clear();
	m_vvecCoinMap.clear();
	m_ptCurent.SetPoint(x, y);
	m_vecPath.clear();
	m_nCoinCount = 0;
	m_strDebug.Empty();
}

void Robot::SetStrategy( MoveStrategy emStrategy )
{
	m_emStrategy = emStrategy;
}

CPoint Robot::GetPosition() const
{
	return m_ptCurent;
}

void Robot::LoadingMap( const vector<vector<bool> > & vvecCoinMap )
{
	//重置数据
	Reset();
	//初始化地图数据
	m_nWidth = vvecCoinMap.size();
	if (m_nWidth > 0)
	{
		m_nHeight = vvecCoinMap[0].size();
	}
	else
	{
		return;
	}
	m_vvecCoinMap = vvecCoinMap;
	//放置机器人到左下角
	m_ptCurent.SetPoint(0, m_nHeight - 1);
	//初始化成本列表
	for (int x = 0; x < m_nWidth; x++)
	{
		for (int y = 0; y < m_nHeight; y++)
		{
			if (m_vvecCoinMap[x][y])
			{
				m_mapCoinCost[CPoint(x, y)] = SearchCost(CPoint(x, y));
			}
		}
	}
}

void Robot::Move()
{
	//已经走出地图外，不要再走
	if (!CheckPos(m_ptCurent))
	{
		return;
	}

	m_strDebug.Empty();

	//移动前将当前位置加入路径集合
	m_vecPath.push_back(m_ptCurent);

	m_strDebug.AppendFormat(_T("当前位置:%s\r\n"), GetDebugString(m_ptCurent));

	//判定能否向上或向右走
	//如果既能向上又能向右，则需要进行策略比较再决定该向哪走
	if (CheckPos(CPoint(m_ptCurent.x + 1, m_ptCurent.y)) && CheckPos(CPoint(m_ptCurent.x, m_ptCurent.y - 1)))
	{
		//计算向上走的成本
		//查找向上的金币
		vector<CPoint> vecCoinUp = SearchCoin(true, m_ptCurent);
		//获取向上金币的成本集合
		vector<vector<CPoint> > vvecCostUp;
		for (int i = 0; i < vecCoinUp.size(); i++)
		{
			if (m_mapCoinCost.count(vecCoinUp[i]) > 0)
			{
				vvecCostUp.push_back(m_mapCoinCost[vecCoinUp[i]]);
			}
			else
			{
				assert(false);
			}
		}

		//计算向右走的成本
		//查找向右的金币
		vector<CPoint> vecCoinRight = SearchCoin(false, m_ptCurent);
		//获取向右金币的成本集合
		vector<vector<CPoint> > vvecCostRight;
		for (int i = 0; i < vecCoinRight.size(); i++)
		{
			if (m_mapCoinCost.count(vecCoinRight[i]) > 0)
			{
				vvecCostRight.push_back(m_mapCoinCost[vecCoinRight[i]]);
			}
			else
			{
				assert(false);
			}
		}

		//向上走则将右边的金币加入成本
		vvecCostUp.push_back(vecCoinRight);
		//向右走则将上边的金币加入成本
		vvecCostRight.push_back(vecCoinUp);

		//合并向上金币的成本集合
		vector<CPoint> vecCostUp = UnionCost(vvecCostUp);
		//合并向右金币的成本集合
		vector<CPoint> vecCostRight = UnionCost(vvecCostRight);

		m_strDebug.AppendFormat(_T("向上走的成本:\r\n"));
		for (int i = 0; i < vecCostUp.size(); i++)
		{
			m_strDebug.AppendFormat(_T("%s"), GetDebugString(vecCostUp[i]));
		}
		m_strDebug.Append(_T("\r\n"));
		m_strDebug.AppendFormat(_T("向右走的成本:\r\n"));
		for (int i = 0; i < vecCostRight.size(); i++)
		{
			m_strDebug.AppendFormat(_T("%s"), GetDebugString(vecCostRight[i]));
		}
		m_strDebug.Append(_T("\r\n"));

		//将成本减去收益进行决策并移动
		bool bIsUp = false;
		if ((int)vecCostUp.size() - (int)vecCoinUp.size() > (int)vecCostRight.size() - (int)vecCoinRight.size())
		{
			m_ptCurent.x++;
			bIsUp = false;
		}
		else if ((int)vecCostUp.size() - (int)vecCoinUp.size() < (int)vecCostRight.size() - (int)vecCoinRight.size())
		{
			m_ptCurent.y--;
			bIsUp = true;
		}
		else
		{
			switch(m_emStrategy)
			{
			case UpFist:
				m_ptCurent.y--;
				bIsUp = true;
				break;
			case RightFist:
				m_ptCurent.x++;
				bIsUp = false;
				break;
			case Random:
				if(rand() % 2 == 0)
				{
					m_ptCurent.y--;
					bIsUp = true;
				}
				else
				{
					m_ptCurent.x++;
					bIsUp = false;
				}
				break;
			default:
				m_ptCurent.y--;
				bIsUp = true;
				break;
			}
		}
		//根据做出的选择，更新成本列表，去除已确定被舍弃的金币
		if (bIsUp)
		{
			AbandonCoin(vecCoinRight);
		}
		else
		{
			AbandonCoin(vecCoinUp);
		}
	}
	//如果只能向右，则不需要考虑，直接向右走
	else if (CheckPos(CPoint(m_ptCurent.x + 1, m_ptCurent.y)))
	{
		m_ptCurent.x++;
	}
	//如果只能向上，直接向上走
	else if (CheckPos(CPoint(m_ptCurent.x, m_ptCurent.y - 1)))
	{
		m_ptCurent.y--;
	}
	//两个方向都不行，游戏已经结束，机器人向上一步，走出地图
	else
	{
		m_ptCurent.y--;
	}

	//更新当前金币数
	m_nCoinCount = 0;
	for (int i = 0; i < m_vecPath.size(); i++)
	{
		if (CheckPos(m_vecPath[i]) && m_vvecCoinMap[m_vecPath[i].x][m_vecPath[i].y])
		{
			m_nCoinCount++;
		}
	}
}

void Robot::AbandonCoin( const vector<CPoint> & vecCoin )
{
	for (int i = 0; i < vecCoin.size(); i++)
	{
		if (m_mapCoinCost.count(vecCoin[i]))
		{
			m_mapCoinCost.erase(vecCoin[i]);
		}
		for (map<CPoint, vector<CPoint> >::iterator itCosts = m_mapCoinCost.begin(); itCosts != m_mapCoinCost.end(); itCosts++)
		{
			vector<CPoint> & vecCosts = itCosts->second;
			vector<CPoint>::iterator itDel = find(vecCosts.begin(), vecCosts.end(), vecCoin[i]);
			if (itDel != vecCosts.end())
			{
				vecCosts.erase(itDel);
			}
		}
	}
}

vector<CPoint> Robot::GetCurPath() const
{
	return m_vecPath;
}

int Robot::GetCurCoinCount() const
{
	return m_nCoinCount;
}

CString Robot::GetDebugInfo()
{
	return m_strDebug;
}

bool operator < (const CPoint & pt1, const CPoint & pt2)
{
	CString str1, str2;
	str1.Format(_T("%d,%d"), pt1.x, pt1.y);
	str2.Format(_T("%d,%d"), pt2.x, pt2.y);
	return str1 < str2;
}

CString GetDebugString( const CPoint & pt )
{
	CString strRet;
	strRet.Format(_T("(%d, %d)"), pt.x, pt.y);
	return strRet;
}
