#pragma once
#include <vector>
using namespace std;

class CoinMap
{
private:
	int m_nWidth;//��ͼ��
	int m_nHeight;//��ͼ��
	vector<vector<bool> > m_vvecCoinMap;//��¼ÿ��λ���Ƿ��н�ҵĵ�ͼ
public:
	CoinMap(int nWidth, int nHeight);
	~CoinMap(void);
	void Reset(int nWidth, int nHeight);//���õ�ͼ
	void AutoFillCoin(int nRatio);//�Զ�����ң�����ָʾ���ʱȣ�����Ϊ2��ÿ��λ�ò�����ҵĸ���Ϊ1/2
	void SetCoin(int x, int y);//����ĳһλ���Ƿ��н�ң���һ������Ϊ�У��ٴ�����Ϊ��
	bool HasCoin(int x, int y);//����ĳһλ���Ƿ��н��
	vector<vector<bool> > GetMap();//��ȡ��ҵ�ͼ
	bool CheckPos(const CPoint & pos);//�ж������Ƿ���Ч����Ч��ʾ����
	int GetWidth();
	int GetHeight();
};
