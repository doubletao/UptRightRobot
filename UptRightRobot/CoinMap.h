#pragma once
#include <vector>
using namespace std;

class CoinMap
{
private:
	int m_nWidth;//地图宽
	int m_nHeight;//地图高
	vector<vector<bool> > m_vvecCoinMap;//记录每个位置是否有金币的地图
public:
	CoinMap(int nWidth, int nHeight);
	~CoinMap(void);
	void Reset(int nWidth, int nHeight);//重置地图
	void AutoFillCoin(int nRatio);//自动填充金币，参数指示概率比，例如为2则每个位置产生金币的概率为1/2
	void SetCoin(int x, int y);//设置某一位置是否有金币，第一次设置为有，再次设置为无
	bool HasCoin(int x, int y);//返回某一位置是否有金币
	vector<vector<bool> > GetMap();//获取金币地图
	bool CheckPos(const CPoint & pos);//判定坐标是否有效，无效表示出界
	int GetWidth();
	int GetHeight();
};
