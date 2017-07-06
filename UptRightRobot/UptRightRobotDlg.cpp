
// UptRightRobotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UptRightRobot.h"
#include "UptRightRobotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUptRightRobotDlg �Ի���




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


// CUptRightRobotDlg ��Ϣ�������

BOOL CUptRightRobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUptRightRobotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		CalculateScene();
		DrawMap(dc, m_stuCoinMap);
		DrawRobot(dc, m_stuRobot);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
			//�н������ƽ��
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
			//û�н��ָ����һ������
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
	//���ƻ����˱���
	{
		//ȡ�û���������
		int x = robot.GetPosition().x;
		int y = robot.GetPosition().y;
		//ȡ�û����˻�����
		CRect rtBound;
		rtBound.SetRect(x * m_nBoxSize, y * m_nBoxSize, x * m_nBoxSize + m_nBoxSize, y * m_nBoxSize + m_nBoxSize);
		//���������������ӣ�һ���Դ���һ�����壬һ���ֱ����
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

	//���ƻ�����·��
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
			//�н������ƽ��
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
			//û�н��ֻ����һ������
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
	//��������˻�û�г������������޸Ľ��������޸ĺ���������¼���
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
