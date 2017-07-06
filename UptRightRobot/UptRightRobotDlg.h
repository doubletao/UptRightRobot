
// UptRightRobotDlg.h : 头文件
//

#pragma once
#include "CoinMap.h"
#include "Robot.h"
#include "afxwin.h"

// CUptRightRobotDlg 对话框
class CUptRightRobotDlg : public CDialogEx
{
// 构造
public:
	CUptRightRobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPTRIGHTROBOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
