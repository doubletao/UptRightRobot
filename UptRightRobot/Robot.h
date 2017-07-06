#pragma once
#include <map>
#include <vector>
using namespace std;

bool operator < (const CPoint & pt1, const CPoint & pt2);
CString GetDebugString(const CPoint & pt);

//遇到成本相同时应当如何选择
enum MoveStrategy
{
	UpFist,			//优先向上
	RightFist,		//优先向右
	Random			//随机
};

class Robot
{
private:
	int m_nWidth;//地图宽
	int m_nHeight;//地图高
	CPoint m_ptCurent;//机器人当前位置
	MoveStrategy m_emStrategy;
	map<CPoint, vector<CPoint> > m_mapCoinCost;//每个金币的成本金币
	vector<vector<bool> > m_vvecCoinMap;//记录每个位置是否有金币的地图
	vector<CPoint> m_vecPath;//机器人走过的路径
	int m_nCoinCount;//机器人当前吃到的金币数
	CString m_strDebug;//调试信息
private:
	vector<CPoint> SearchCost(const CPoint & posCoin);//查找指定位置的金币成本，posCoin指示金币位置
	vector<CPoint> SearchCoin(bool bIsUp, const CPoint & pos);//从指定pos向指定方向搜索，返回搜索到的金币坐标，bIsUp为真则向上搜索，否则向右搜索
	vector<CPoint> UnionCost(const vector<vector<CPoint> > & vvecCost);//求多个成本的并集(合并多个集合并去重)
	void AbandonCoin(const vector<CPoint> & vecCoin);//舍弃指定金币，更新成本列表
	bool CheckPos(const CPoint & pos);//判定坐标是否有效，无效表示出界
public:
	Robot(void);
	~Robot(void);
	void LoadingMap(const vector<vector<bool> > & vvecCoinMap);//装载地图并解析
	void Move();//根据决策情况移动一步
	CPoint GetPosition() const;//返回机器人当前位置
	void Reset(int x = 0, int y = 0);//重置机器人，并指定机器人初始坐标
	void SetStrategy(MoveStrategy emStrategy);//设置成本相同时的优先策略
	vector<CPoint> GetCurPath() const;//获取当前走过的路径
	int GetCurCoinCount() const;//获取当前获得的金币数
	CString GetDebugInfo();//获取当前调试信息
};

