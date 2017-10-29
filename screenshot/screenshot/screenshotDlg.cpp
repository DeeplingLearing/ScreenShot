
// screenshotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "screenshot.h"
#include "screenshotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CscreenshotDlg 对话框



CscreenshotDlg::CscreenshotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CscreenshotDlg::IDD, pParent)
	, m_cxScreen(0)
	, m_cyScreen(0)
	, m_xScreen(0)
	, m_yScreen(0)
	, m_bMasked(false)
	, m_ptCurStart(0)
	, m_ptCurEnd(0)
	, m_nAlph(255)
	, m_bPaintOther(true)
	, m_bLbnt(false)
	, m_bFinsh(false)
	, m_ptMoveStart(0)
	, m_ptMoveEnd(0)
{
	m_pBmp = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CscreenshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CscreenshotDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
//	ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CscreenshotDlg::~CscreenshotDlg()
{
	if (m_pBmp)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}
}

// CscreenshotDlg 消息处理程序

BOOL CscreenshotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//屏幕宽高及左上角坐标
	m_cxScreen = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_cyScreen = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
	m_xScreen = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	m_yScreen = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	//抓取屏幕位图
	CaputerImage();
	//初始化窗口为pop
	SetWindowLong(m_hWnd, GWL_STYLE, (LONG)WS_POPUP);
	//鼠标隐藏
	ShowCursor(FALSE);
	SetWindowPos(NULL, m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, SWP_SHOWWINDOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CscreenshotDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		//双缓冲
		CDC *pDc = GetDC();
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDc);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDc, m_cxScreen, m_cyScreen);
		CBitmap *pOldbmp = dcMem.SelectObject(&bmp);
		OnPaintBackgroundImage(&dcMem);
		if (m_bPaintOther) //是否需要绘制辅助信息
		{
			DoPaintLine(&dcMem);
		//	DoPaintTip(&dcMem);
		}
		else
		{
			//
			DoCaptureRgn(&dcMem);

			DoPaintRectange(&dcMem);
		}

		pDc->BitBlt(m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, &dcMem, 0, 0, SRCCOPY);

		DeleteDC(dcMem);
		ReleaseDC(pDc);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CscreenshotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 抓取图片
BOOL CscreenshotDlg::CaputerImage()
{
	//屏幕设备环境
	HDC hScreenDc = ::GetDC(::GetDesktopWindow());
	if (NULL == hScreenDc)
	{
		MessageBox(_T("获取桌面设备环境失败!"));
	}
	CDC dc;
	dc.Attach(hScreenDc);
	//窗口设备环境
	CDC *pDc = GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDc);
	m_pBmp = new CBitmap;
	m_pBmp->CreateCompatibleBitmap(pDc, m_cxScreen, m_cyScreen);
	dcMem.SelectObject(m_pBmp);
	//拷贝屏幕位图到内存设备环境中
	if (!dcMem.BitBlt(m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, &dc, 0, 0, SRCCOPY))
	{
		MessageBox(_T("抓取屏幕位图失败!"), _T("ERROR"), MB_ICONERROR);
	}

	//释放资源
	DeleteDC(dcMem);
	ReleaseDC(pDc);
	DeleteDC(dc);
	::ReleaseDC(NULL, hScreenDc);
	return TRUE;
}


// 绘制背景图片
BOOL CscreenshotDlg::OnPaintBackgroundImage(CDC * pDc)
{
	CDC *pTempDc = GetDC();
	CDC tempDcMem;
	tempDcMem.CreateCompatibleDC(pTempDc);
	CBitmap *pbmp = tempDcMem.SelectObject(m_pBmp);

	if (false == m_bMasked)
	{
		//设置位图透明度
		//先拷贝到内存设备环境中,一起绘制到窗口设备环境中
		//**需要改变背景透明度时,先将该修改好的位图存入兼容设备环境中
		BLENDFUNCTION ftn;
		ftn.BlendOp = AC_SRC_OVER;
		ftn.BlendFlags = 0;
		ftn.SourceConstantAlpha = m_nAlph; //透明度值
		ftn.AlphaFormat = 0;
		if (!AlphaBlend(pDc->GetSafeHdc(), m_xScreen, m_yScreen, m_cxScreen, m_cyScreen,
			tempDcMem.GetSafeHdc(), 0, 0, m_cxScreen, m_cyScreen, ftn)
			)
		{
			MessageBox(_T("设置背景透明度失败!"), _T("WARING"), MB_ICONWARNING);
		}
	}
	else
	{
		if (!pDc->BitBlt(m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, &tempDcMem, 0, 0, SRCCOPY))
		{
			MessageBox(_T("绘制背景失败!"), _T("WARING"), MB_ICONWARNING);
		}
	}
	//释放资源
	tempDcMem.SelectObject(pbmp);
	DeleteDC(tempDcMem);
	ReleaseDC(pTempDc);
	DeleteObject(pbmp);

	return TRUE;
}


// 绘制坐标线
BOOL CscreenshotDlg::DoPaintLine(CDC * pDc)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0)); //黑色画笔
	CPen *poldPen = pDc->SelectObject(&pen);

	//x坐标
	pDc->MoveTo(0, m_ptCurStart.y);
	pDc->LineTo(m_cxScreen, m_ptCurStart.y);
	//y坐标
	pDc->MoveTo(m_ptCurStart.x, 0);
	pDc->LineTo(m_ptCurStart.x, m_cyScreen);

	//释放资源
	pDc->SelectObject(poldPen);
	DeleteObject(poldPen);
	DeleteObject(pen);

	return TRUE;
}

//鼠标移动时，提示坐标线也会移动
void CscreenshotDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bLbnt)
	{
		//鼠标左键按下后开始获取截图区域
		m_bPaintOther = false; //鼠标左键按下且移动鼠标时, 辅助撤销
		m_ptCurEnd = point;
	//	InvalidateRect(rtRgn, FALSE);
		UpdateWindow();

	}
	else
	{
		//鼠标左键未按下, 移动起点坐标
		m_ptCurStart = point;
		UpdateWindow();
	}
	
	if (m_bFinsh)
	{
		if (PtInRect(m_rtRgn, point))
		{
			m_ptMoveEnd = point;

			//MoveRectange(m_ptMoveStart, m_ptMoveEnd);
		}
	}
	//InvalidateRect(NULL);
	//PostMessage(WM_PAINT);
	//CDialogEx::OnMouseMove(nFlags, point);
}


//BOOL CscreenshotDlg::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//	return 0;
//	//return CDialogEx::OnEraseBkgnd(pDC);
//}


// 绘制提示框
BOOL CscreenshotDlg::DoPaintTip(CDC * pDc)
{
	//绘制背景为灰色的矩形提示框
	CBrush brush;
	brush.CreateSolidBrush(RGB(100, 100, 100));
	CBrush *oldBrush = pDc->SelectObject(&brush);
	//矩形的宽高
	int cx = 200;
	int cy = 50;
	pDc->Rectangle(m_ptCurStart.x, m_ptCurStart.y, m_ptCurStart.x + cx, m_ptCurStart.y + cy);
	//在提示框内绘制提示文本信息
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *oldPen = pDc->SelectObject(&pen);
	CString strTip = _T("鼠标左键选择截图区域");
	CRect rect;
	rect.left = m_ptCurStart.x + 5;
	rect.top = m_ptCurStart.y + 5;
	rect.right = rect.left + cx;
	rect.bottom = rect.top + cy;
	SetBkMode(pDc->GetSafeHdc(), TRANSPARENT);
	SetBkColor(pDc->GetSafeHdc(), RGB(255, 255, 255));
	pDc->DrawText(strTip, rect, DT_VCENTER | DT_LEFT);

	//释放资源
	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
	//DeleteObject(pen);
	DeleteObject(oldPen);
	DeleteObject(brush);
	DeleteObject(oldBrush);

	return TRUE;
}

//鼠标左键点击, 改变透明度并且开始绘制需要截取的区域
void CscreenshotDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!m_bLbnt)
	{
		//改变背景透明度
		m_nAlph = 200;
		//鼠标左键按下
		m_bLbnt = true;
		m_ptCurStart = point;
		//鼠标外形改变
		SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
		ShowCursor(TRUE);
	}
	if (m_bFinsh)
	{
		//拖动截取的区间
		m_rtRgn.left = m_ptCurStart.x;
		m_rtRgn.top = m_ptCurStart.y;
		m_rtRgn.right = m_ptCurEnd.x;
		m_rtRgn.bottom = m_ptCurEnd.y;

		if (PtInRect(m_rtRgn, point))
		{
			m_ptMoveStart = point;
		}

	}
	//CDialogEx::OnLButtonDown(nFlags, point);
}


// 绘制截屏区域
BOOL CscreenshotDlg::DoCaptureRgn(CDC * pDc)
{
	//定义临时的内存设备环境
	CDC *pTempDc = GetDC();
	CDC tempDcMem;
	tempDcMem.CreateCompatibleDC(pTempDc);
	CBitmap *pbmpOld = tempDcMem.SelectObject(m_pBmp);

	//将选定的区域位图拷贝到设备环境中
	int iWidth = (int)abs(m_ptCurEnd.x - m_ptCurStart.x);
	int iHeight = (int)abs(m_ptCurEnd.y - m_ptCurStart.y);

	pDc->BitBlt(min(m_ptCurStart.x, m_ptCurEnd.x), min(m_ptCurStart.y, m_ptCurEnd.y), iWidth, iHeight, 
		&tempDcMem, min(m_ptCurStart.x, m_ptCurEnd.x), min(m_ptCurStart.y, m_ptCurEnd.y), SRCCOPY);	

	tempDcMem.SelectObject(pbmpOld);
	DeleteObject(pbmpOld);
	DeleteDC(tempDcMem);
	ReleaseDC(pTempDc);

	return TRUE;
}


// 绘制截取区的边框
BOOL CscreenshotDlg::DoPaintRectange(CDC * pDc)
{
	//绘制边框
	CPen pen(PS_SOLID, 2, RGB(255, 255, 255)); //白色画笔
	CPen *oldPen = pDc->SelectObject(&pen);
	pDc->MoveTo(m_ptCurStart.x, m_ptCurStart.y);
	pDc->LineTo(m_ptCurEnd.x, m_ptCurStart.y);
	pDc->LineTo(m_ptCurEnd.x, m_ptCurEnd.y);
	pDc->LineTo(m_ptCurStart.x, m_ptCurEnd.y);
	pDc->LineTo(m_ptCurStart.x, m_ptCurStart.y);

	return TRUE;
}

//鼠标左键释放,鼠标外形改变,截取区域确定
void CscreenshotDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_bLbnt)
	{
		//鼠标改变
		SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		ShowCursor(TRUE);
		//确定截取区域
		m_ptCurEnd = point;

		m_bFinsh = true;
		ValidateRect(NULL);
	}
	if (m_bFinsh)
	{
		//保存截图
	}
	//CDialogEx::OnLButtonUp(nFlags, point);
}


// 移动矩形区域
BOOL CscreenshotDlg::MoveRectange(CPoint pt1, CPoint pt2)
{
	//偏移坐标
	int offsetX = pt2.x - pt1.x;
	int offsetY = pt2.y - pt1.y;
	//截取区域宽高
	int iWidth = abs(m_ptCurEnd.x - m_ptCurStart.x);
	int iHeight = abs(m_ptCurEnd.y - m_ptCurStart.y);
	//改变截取区域坐标
	m_ptCurStart.x += offsetX;
	m_ptCurStart.y += offsetY;

	m_ptCurEnd.x = m_ptCurStart.x + iWidth;
	m_ptCurEnd.y = m_ptCurStart.y + iHeight;

	CDC *pDc = GetDC();
	DoCaptureRgn(pDc);
	DoPaintRectange(pDc);

	ReleaseDC(pDc);

	return TRUE;
}


// 指定图片名称和保存图片到指定位置
BOOL CscreenshotDlg::SaveImage(const CString & filePath, const CString & filename)
{
	//位图文件头信息
	BITMAPFILEHEADER bpfh;
	bpfh.bfType = 0;
	bpfh.bfReserved1 = 0;
	bpfh.bfReserved2 = 0;
	//位图头信息
	BITMAPINFO bpi;
	//位图数据
	

	return TRUE;
}
