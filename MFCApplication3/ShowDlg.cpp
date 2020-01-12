// ShowDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication3.h"
#include "ShowDlg.h"
#include "afxdialogex.h"


// ShowDlg 对话框

IMPLEMENT_DYNAMIC(ShowDlg, CDialogEx)

ShowDlg::ShowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOW_DIALOG, pParent)
{
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);

	CDC* pDC=GetDesktopWindow()->GetDC();

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, m_nWidth, m_nHeight);
	m_memDC.CreateCompatibleDC(pDC);
	m_memDC.SelectObject(&bmp);
	m_memDC.BitBlt(0, 0, m_nWidth, m_nHeight, pDC, 0, 0, SRCCOPY);
	pDC->DeleteDC();
	bmp.DeleteObject();

}

ShowDlg::~ShowDlg()
{
}

void ShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ShowDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// ShowDlg 消息处理程序


BOOL ShowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_nWidth=GetSystemMetrics(SM_CXSCREEN);
	m_nHeight= GetSystemMetrics(SM_CYSCREEN);
	MoveWindow(0, 0, m_nWidth, m_nHeight);


	m_tracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeOutside;
	m_tracker.m_nHandleSize = 5;
	m_tracker.m_rect.SetRect(0, 0, 0, 0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void ShowDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	dc.BitBlt(0, 0, m_nWidth, m_nHeight, &m_memDC, 0, 0, SRCCOPY);
	m_tracker.Draw(&dc);
}


void ShowDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_tracker.HitTest(point) < 0) {
		
		m_tracker.TrackRubberBand(this, point, TRUE);
		m_tracker.m_rect.NormalizeRect();

	}
	else {
		m_tracker.Track(this, point, TRUE);
		m_tracker.m_rect.NormalizeRect();
	}
	Invalidate(FALSE);
	

	CDialogEx::OnLButtonDown(nFlags, point);
}


void ShowDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nWidth = m_tracker.m_rect.Width();
	int nHeight = m_tracker.m_rect.Height();


	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&m_memDC, nWidth, nHeight);
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(&bmp);
	memDC.BitBlt(0, 0, nWidth, nHeight, &m_memDC, m_tracker.m_rect.left, m_tracker.m_rect.top, SRCCOPY);
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bmp);
	CloseClipboard();

	CTime t = CTime::GetCurrentTime();
	srand(time(NULL));

	CString strPicName;
	strPicName.Format(L"C:\\Users\\user\\Desktop\\S.K截图%s%d.png", t.Format(L"%Y.%m.%d.%H.%M%S"),rand()%900+100);

	CImage img;
	img.Attach(bmp);
	img.Save(strPicName);
	img.Detach();


	memDC.DeleteDC();
	bmp.DeleteObject();
	m_memDC.DeleteDC();

	EndDialog(IDCANCEL);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
