#pragma once
#include <map>
#include <vector>
using namespace std;

bool operator < (const CPoint & pt1, const CPoint & pt2);
CString GetDebugString(const CPoint & pt);

//�����ɱ���ͬʱӦ�����ѡ��
enum MoveStrategy
{
	UpFist,			//��������
	RightFist,		//��������
	Random			//���
};

class Robot
{
private:
	int m_nWidth;//��ͼ��
	int m_nHeight;//��ͼ��
	CPoint m_ptCurent;//�����˵�ǰλ��
	MoveStrategy m_emStrategy;
	map<CPoint, vector<CPoint> > m_mapCoinCost;//ÿ����ҵĳɱ����
	vector<vector<bool> > m_vvecCoinMap;//��¼ÿ��λ���Ƿ��н�ҵĵ�ͼ
	vector<CPoint> m_vecPath;//�������߹���·��
	int m_nCoinCount;//�����˵�ǰ�Ե��Ľ����
	CString m_strDebug;//������Ϣ
private:
	vector<CPoint> SearchCost(const CPoint & posCoin);//����ָ��λ�õĽ�ҳɱ���posCoinָʾ���λ��
	vector<CPoint> SearchCoin(bool bIsUp, const CPoint & pos);//��ָ��pos��ָ�����������������������Ľ�����꣬bIsUpΪ��������������������������
	vector<CPoint> UnionCost(const vector<vector<CPoint> > & vvecCost);//�����ɱ��Ĳ���(�ϲ�������ϲ�ȥ��)
	void AbandonCoin(const vector<CPoint> & vecCoin);//����ָ����ң����³ɱ��б�
	bool CheckPos(const CPoint & pos);//�ж������Ƿ���Ч����Ч��ʾ����
public:
	Robot(void);
	~Robot(void);
	void LoadingMap(const vector<vector<bool> > & vvecCoinMap);//װ�ص�ͼ������
	void Move();//���ݾ�������ƶ�һ��
	CPoint GetPosition() const;//���ػ����˵�ǰλ��
	void Reset(int x = 0, int y = 0);//���û����ˣ���ָ�������˳�ʼ����
	void SetStrategy(MoveStrategy emStrategy);//���óɱ���ͬʱ�����Ȳ���
	vector<CPoint> GetCurPath() const;//��ȡ��ǰ�߹���·��
	int GetCurCoinCount() const;//��ȡ��ǰ��õĽ����
	CString GetDebugInfo();//��ȡ��ǰ������Ϣ
};

