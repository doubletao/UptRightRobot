
// UptRightRobotDlg.h : ͷ�ļ�
//

#pragma once
#include "CoinMap.h"
#include "Robot.h"
#include "afxwin.h"

// CUptRightRobotDlg �Ի���
class CUptRightRobotDlg : public CDialogEx
{
// ����
public:
	CUptRightRobotDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPTRIGHTROBOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	CoinMap m_stuCoinMap;
	Robot m_stuRobot;
	int m_nBoxSize;
	CEdit m_ctrTxtInfo;
private:
	void DrawMap(CPaintDC & dc, const CoinMap & coinMap);
	void DrawRobot(CPaintDC & dc, const Robot & robot);
	void CalculateScene();
public:
	afx_msg void OnBnClickedBtnMove();
};
