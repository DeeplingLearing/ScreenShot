
// screenshotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "screenshot.h"
#include "screenshotDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CscreenshotDlg �Ի���



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

// CscreenshotDlg ��Ϣ�������

BOOL CscreenshotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��Ļ��߼����Ͻ�����
	m_cxScreen = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_cyScreen = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);
	m_xScreen = ::GetSystemMetrics(SM_XVIRTUALSCREEN);
	m_yScreen = ::GetSystemMetrics(SM_YVIRTUALSCREEN);
	//ץȡ��Ļλͼ
	CaputerImage();
	//��ʼ������Ϊpop
	SetWindowLong(m_hWnd, GWL_STYLE, (LONG)WS_POPUP);
	//�������
	ShowCursor(FALSE);
	SetWindowPos(NULL, m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, SWP_SHOWWINDOW);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CscreenshotDlg::OnPaint()
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
		//CDialogEx::OnPaint();
		//˫����
		CDC *pDc = GetDC();
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDc);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDc, m_cxScreen, m_cyScreen);
		CBitmap *pOldbmp = dcMem.SelectObject(&bmp);
		OnPaintBackgroundImage(&dcMem);
		if (m_bPaintOther) //�Ƿ���Ҫ���Ƹ�����Ϣ
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CscreenshotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// ץȡͼƬ
BOOL CscreenshotDlg::CaputerImage()
{
	//��Ļ�豸����
	HDC hScreenDc = ::GetDC(::GetDesktopWindow());
	if (NULL == hScreenDc)
	{
		MessageBox(_T("��ȡ�����豸����ʧ��!"));
	}
	CDC dc;
	dc.Attach(hScreenDc);
	//�����豸����
	CDC *pDc = GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDc);
	m_pBmp = new CBitmap;
	m_pBmp->CreateCompatibleBitmap(pDc, m_cxScreen, m_cyScreen);
	dcMem.SelectObject(m_pBmp);
	//������Ļλͼ���ڴ��豸������
	if (!dcMem.BitBlt(m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, &dc, 0, 0, SRCCOPY))
	{
		MessageBox(_T("ץȡ��Ļλͼʧ��!"), _T("ERROR"), MB_ICONERROR);
	}

	//�ͷ���Դ
	DeleteDC(dcMem);
	ReleaseDC(pDc);
	DeleteDC(dc);
	::ReleaseDC(NULL, hScreenDc);
	return TRUE;
}


// ���Ʊ���ͼƬ
BOOL CscreenshotDlg::OnPaintBackgroundImage(CDC * pDc)
{
	CDC *pTempDc = GetDC();
	CDC tempDcMem;
	tempDcMem.CreateCompatibleDC(pTempDc);
	CBitmap *pbmp = tempDcMem.SelectObject(m_pBmp);

	if (false == m_bMasked)
	{
		//����λͼ͸����
		//�ȿ������ڴ��豸������,һ����Ƶ������豸������
		//**��Ҫ�ı䱳��͸����ʱ,�Ƚ����޸ĺõ�λͼ��������豸������
		BLENDFUNCTION ftn;
		ftn.BlendOp = AC_SRC_OVER;
		ftn.BlendFlags = 0;
		ftn.SourceConstantAlpha = m_nAlph; //͸����ֵ
		ftn.AlphaFormat = 0;
		if (!AlphaBlend(pDc->GetSafeHdc(), m_xScreen, m_yScreen, m_cxScreen, m_cyScreen,
			tempDcMem.GetSafeHdc(), 0, 0, m_cxScreen, m_cyScreen, ftn)
			)
		{
			MessageBox(_T("���ñ���͸����ʧ��!"), _T("WARING"), MB_ICONWARNING);
		}
	}
	else
	{
		if (!pDc->BitBlt(m_xScreen, m_yScreen, m_cxScreen, m_cyScreen, &tempDcMem, 0, 0, SRCCOPY))
		{
			MessageBox(_T("���Ʊ���ʧ��!"), _T("WARING"), MB_ICONWARNING);
		}
	}
	//�ͷ���Դ
	tempDcMem.SelectObject(pbmp);
	DeleteDC(tempDcMem);
	ReleaseDC(pTempDc);
	DeleteObject(pbmp);

	return TRUE;
}


// ����������
BOOL CscreenshotDlg::DoPaintLine(CDC * pDc)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0)); //��ɫ����
	CPen *poldPen = pDc->SelectObject(&pen);

	//x����
	pDc->MoveTo(0, m_ptCurStart.y);
	pDc->LineTo(m_cxScreen, m_ptCurStart.y);
	//y����
	pDc->MoveTo(m_ptCurStart.x, 0);
	pDc->LineTo(m_ptCurStart.x, m_cyScreen);

	//�ͷ���Դ
	pDc->SelectObject(poldPen);
	DeleteObject(poldPen);
	DeleteObject(pen);

	return TRUE;
}

//����ƶ�ʱ����ʾ������Ҳ���ƶ�
void CscreenshotDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bLbnt)
	{
		//���������º�ʼ��ȡ��ͼ����
		m_bPaintOther = false; //�������������ƶ����ʱ, ��������
		m_ptCurEnd = point;
	//	InvalidateRect(rtRgn, FALSE);
		UpdateWindow();

	}
	else
	{
		//������δ����, �ƶ��������
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
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	return 0;
//	//return CDialogEx::OnEraseBkgnd(pDC);
//}


// ������ʾ��
BOOL CscreenshotDlg::DoPaintTip(CDC * pDc)
{
	//���Ʊ���Ϊ��ɫ�ľ�����ʾ��
	CBrush brush;
	brush.CreateSolidBrush(RGB(100, 100, 100));
	CBrush *oldBrush = pDc->SelectObject(&brush);
	//���εĿ��
	int cx = 200;
	int cy = 50;
	pDc->Rectangle(m_ptCurStart.x, m_ptCurStart.y, m_ptCurStart.x + cx, m_ptCurStart.y + cy);
	//����ʾ���ڻ�����ʾ�ı���Ϣ
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *oldPen = pDc->SelectObject(&pen);
	CString strTip = _T("������ѡ���ͼ����");
	CRect rect;
	rect.left = m_ptCurStart.x + 5;
	rect.top = m_ptCurStart.y + 5;
	rect.right = rect.left + cx;
	rect.bottom = rect.top + cy;
	SetBkMode(pDc->GetSafeHdc(), TRANSPARENT);
	SetBkColor(pDc->GetSafeHdc(), RGB(255, 255, 255));
	pDc->DrawText(strTip, rect, DT_VCENTER | DT_LEFT);

	//�ͷ���Դ
	pDc->SelectObject(oldPen);
	pDc->SelectObject(oldBrush);
	//DeleteObject(pen);
	DeleteObject(oldPen);
	DeleteObject(brush);
	DeleteObject(oldBrush);

	return TRUE;
}

//���������, �ı�͸���Ȳ��ҿ�ʼ������Ҫ��ȡ������
void CscreenshotDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bLbnt)
	{
		//�ı䱳��͸����
		m_nAlph = 200;
		//����������
		m_bLbnt = true;
		m_ptCurStart = point;
		//������θı�
		SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
		ShowCursor(TRUE);
	}
	if (m_bFinsh)
	{
		//�϶���ȡ������
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


// ���ƽ�������
BOOL CscreenshotDlg::DoCaptureRgn(CDC * pDc)
{
	//������ʱ���ڴ��豸����
	CDC *pTempDc = GetDC();
	CDC tempDcMem;
	tempDcMem.CreateCompatibleDC(pTempDc);
	CBitmap *pbmpOld = tempDcMem.SelectObject(m_pBmp);

	//��ѡ��������λͼ�������豸������
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


// ���ƽ�ȡ���ı߿�
BOOL CscreenshotDlg::DoPaintRectange(CDC * pDc)
{
	//���Ʊ߿�
	CPen pen(PS_SOLID, 2, RGB(255, 255, 255)); //��ɫ����
	CPen *oldPen = pDc->SelectObject(&pen);
	pDc->MoveTo(m_ptCurStart.x, m_ptCurStart.y);
	pDc->LineTo(m_ptCurEnd.x, m_ptCurStart.y);
	pDc->LineTo(m_ptCurEnd.x, m_ptCurEnd.y);
	pDc->LineTo(m_ptCurStart.x, m_ptCurEnd.y);
	pDc->LineTo(m_ptCurStart.x, m_ptCurStart.y);

	return TRUE;
}

//�������ͷ�,������θı�,��ȡ����ȷ��
void CscreenshotDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bLbnt)
	{
		//���ı�
		SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		ShowCursor(TRUE);
		//ȷ����ȡ����
		m_ptCurEnd = point;

		m_bFinsh = true;
		ValidateRect(NULL);
	}
	if (m_bFinsh)
	{
		//�����ͼ
	}
	//CDialogEx::OnLButtonUp(nFlags, point);
}


// �ƶ���������
BOOL CscreenshotDlg::MoveRectange(CPoint pt1, CPoint pt2)
{
	//ƫ������
	int offsetX = pt2.x - pt1.x;
	int offsetY = pt2.y - pt1.y;
	//��ȡ������
	int iWidth = abs(m_ptCurEnd.x - m_ptCurStart.x);
	int iHeight = abs(m_ptCurEnd.y - m_ptCurStart.y);
	//�ı��ȡ��������
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


// ָ��ͼƬ���ƺͱ���ͼƬ��ָ��λ��
BOOL CscreenshotDlg::SaveImage(const CString & filePath, const CString & filename)
{
	//λͼ�ļ�ͷ��Ϣ
	BITMAPFILEHEADER bpfh;
	bpfh.bfType = 0;
	bpfh.bfReserved1 = 0;
	bpfh.bfReserved2 = 0;
	//λͼͷ��Ϣ
	BITMAPINFO bpi;
	//λͼ����
	

	return TRUE;
}
