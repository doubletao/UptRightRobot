
// UptRightRobotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UptRightRobot.h"
#include "UptRightRobotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUptRightRobotDlg 对话框




CUptRightRobotDlg::CUptRightRobotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUptRightRobotDlg::IDD, pParent)
	, m_stuCoinMap(10, 10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_stuCoinMap.AutoFillCoin(3);
	m_stuRobot.LoadingMap(m_stuCoinMap.GetMap());
}

void CUptRightRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, m_ctrTxtInfo);
}

BEGIN_MESSAGE_MAP(CUptRightRobotDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_MOVE, &CUptRightRobotDlg::OnBnClickedBtnMove)
END_MESSAGE_MAP()


// CUptRightRobotDlg 消息处理程序

BOOL CUptRightRobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUptRightRobotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		CalculateScene();
		DrawMap(dc, m_stuCoinMap);
		DrawRobot(dc, m_stuRobot);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUptRightRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUptRightRobotDlg::DrawMap( CPaintDC & dc, const CoinMap & coinMap )
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(200, 180, 180));
	CPen pen;
	pen.CreatePen(PS_SOLID, 0, RGB(100, 100, 100));

	HGDIOBJ oldBrush = dc.SelectObject(brush);
	HGDIOBJ oldPen =  dc.SelectObject(pen);

	for (int x = 0; x < m_stuCoinMap.GetWidth(); x++)
	{
		for (int y = 0; y < m_stuCoinMap.GetHeight(); y++)
		{
			//有金币则绘制金币
			if (m_stuCoinMap.HasCoin(x, y))
			{
				dc.Rectangle(x * m_nBoxSize, y * m_nBoxSize, x * m_nBoxSize + m_nBoxSize, y * m_nBoxSize + m_nBoxSize);

				CBrush brush;
				brush.CreateSolidBrush(RGB(231, 171, 48));
				CPen pen;
				pen.CreatePen(PS_SOLID, 2, RGB(243, 142, 35));

				HGDIOBJ oldBrush = dc.SelectObject(brush);
				HGDIOBJ oldPen =  dc.SelectObject(pen);

				dc.Ellipse(x * m_nBoxSize + m_nBoxSize / 4, y * m_nBoxSize + m_nBoxSize / 4, x * m_nBoxSize + m_nBoxSize - m_nBoxSize / 4, y * m_nBoxSize + m_nBoxSize - m_nBoxSize / 4);

				dc.SelectObject(oldPen);
				dc.SelectObject(oldBrush);
			}
			//没有金币指绘制一个格子
			else
			{
				dc.Rectangle(x * m_nBoxSize, y * m_nBoxSize, x * m_nBoxSize + m_nBoxSize, y * m_nBoxSize + m_nBoxSize);
			}
		}
	}

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
}

void CUptRightRobotDlg::DrawRobot( CPaintDC & dc, const Robot & robot )
{
	//绘制机器人本身
	{
		//取得机器人坐标
		int x = robot.GetPosition().x;
		int y = robot.GetPosition().y;
		//取得机器人绘制区
		CRect rtBound;
		rtBound.SetRect(x * m_nBoxSize, y * m_nBoxSize, x * m_nBoxSize + m_nBoxSize, y * m_nBoxSize + m_nBoxSize);
		//机器人由两个轮子，一个脑袋，一个身体，一个手臂组成
		CRect rtWheel1;
		CRect rtWheel2;
		CRect rtHead;
		CRect rtBody;
		CRect rtArm;
		rtHead.SetRect(rtBound.left + m_nBoxSize / 4 + m_nBoxSize / 8, rtBound.top, rtBound.left + m_nBoxSize / 4 + m_nBoxSize / 4 + m_nBoxSize / 8, rtBound.top + m_nBoxSize / 4);
		rtBody.SetRect(rtBound.left + m_nBoxSize / 4, rtBound.top + m_nBoxSize / 4, rtBound.left + m_nBoxSize - m_nBoxSize / 4, rtBound.top + m_nBoxSize - m_nBoxSize / 4);
		rtArm.SetRect(rtBound.left + m_nBoxSize / 2, rtBound.top + m_nBoxSize / 2 - m_nBoxSize / 8, rtBound.left + m_nBoxSize - m_nBoxSize / 8, rtBound.top + m_nBoxSize / 2 + m_nBoxSize / 8);
		rtWheel1.SetRect(rtBound.left + m_nBoxSize / 4, rtBound.top + m_nBoxSize - m_nBoxSize / 4, rtBound.left + m_nBoxSize / 2, rtBound.bottom);
		rtWheel2.SetRect(rtBound.left + m_nBoxSize / 2, rtBound.top + m_nBoxSize - m_nBoxSize / 4, rtBound.left + m_nBoxSize - m_nBoxSize / 4, rtBound.bottom);

		RGB(129,84,87);
		dc.Rectangle(rtBody);
		dc.Rectangle(rtArm);
		dc.Ellipse(rtHead);
		dc.Ellipse(rtWheel1);
		dc.Ellipse(rtWheel2);
	}

	//绘制机器人路径
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(200, 200, 200));
		CPen pen;
		pen.CreatePen(PS_SOLID, 0, RGB(100, 100, 100));

		HGDIOBJ oldBrush = dc.SelectObject(brush);
		HGDIOBJ oldPen =  dc.SelectObject(pen);

		for (int i = 0; i < m_stuRobot.GetCurPath().size(); i++)
		{
			int x = m_stuRobot.GetCurPath().at(i).x;
			int y = m_stuRobot.GetCurPath().at(i).y;
			//有金币则绘制金币
			if (m_stuCoinMap.HasCoin(x, y))
			{
				dc.Rectangle(x * m_nBoxSize, y * m_nBoxSize, x * m_nBoxSize + m_nBoxSize, y * m_nBoxSize + m_nBoxSize);

				CBrush brush;
				brush.CreateSolidBrush(RGB(231, 171, 48));
				CPen pen;
				pen.CreatePen(PS_SOLID, 2, RGB(243, 142, 35));

				HGDIOBJ oldBrush = dc.SelectObject(brush);
				HGDIOBJ oldPen =  dc.SelectObject(pen);

				dc.Ellipse(x * m_nBoxSize + m_nBoxSize / 4, y * m_nBoxSize + m_nBoxSize / 4, x * m_nBoxSize + m_nBoxSize - m_nBoxSize / 4, y * m_nBoxSize + m_nBoxSize - m_nBoxSize / 4);

				dc.SelectObject(oldPen);
				dc.SelectObject(oldBrush);
			}
			//没有金币只绘制一个格子
			else
			{
				dc.Rectangle(x * m_nBoxSize, y * m_nBoxSize, x * m_nBoxSize + m_nBoxSize, y * m_nBoxSize + m_nBoxSize);
			}
		}

		dc.SelectObject(oldPen);
		dc.SelectObject(oldBrush);
	}
}

void CUptRightRobotDlg::CalculateScene()
{
	CRect rtBound;
	GetClientRect(rtBound);
	int nMiniWndSize = rtBound.Width() > rtBound.Height() ? rtBound.Height() : rtBound.Width();
	int nMaxMapSize = m_stuCoinMap.GetWidth() < m_stuCoinMap.GetHeight() ? m_stuCoinMap.GetHeight() : m_stuCoinMap.GetWidth();
	m_nBoxSize = nMiniWndSize / nMaxMapSize;
}

void CUptRightRobotDlg::OnLButtonDown( UINT nFlags, CPoint point )
{
	//如果机器人还没有出发，则允许修改金币情况，修改后机器人重新计算
	if (m_stuRobot.GetCurPath().empty())
	{
		m_stuCoinMap.SetCoin(point.x / m_nBoxSize, point.y / m_nBoxSize);
		m_stuRobot.LoadingMap(m_stuCoinMap.GetMap());
		Invalidate(FALSE);
	}
}


void CUptRightRobotDlg::OnBnClickedBtnMove()
{
	m_stuRobot.Move();
	CString strDebug;
	m_ctrTxtInfo.GetWindowText(strDebug);
	strDebug.Append(m_stuRobot.GetDebugInfo());
	m_ctrTxtInfo.SetWindowText(strDebug);
	Invalidate(FALSE);
}
