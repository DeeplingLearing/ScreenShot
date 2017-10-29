
// screenshotDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"


// CscreenshotDlg �Ի���
class CscreenshotDlg : public CDialogEx
{
// ����
public:
	CscreenshotDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CscreenshotDlg();         //��������
// �Ի�������
	enum { IDD = IDD_SCREENSHOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// ��Ļ���
	int m_cxScreen;
	// ��Ļ��
	int m_cyScreen;
	// ��Ļ���Ͻ�x����
	int m_xScreen;
	// ��Ļ���Ͻ�y����
	int m_yScreen;
	// ����ͼƬ
	CBitmap *m_pBmp;
protected:
	// ץȡͼƬ
	BOOL CaputerImage();
public:
	// ���Ʊ���ͼƬ
	BOOL OnPaintBackgroundImage(CDC * pDc);
private:
	// �Ƿ����͸������ɫ
	bool m_bMasked;
	// ��꿪ʼ����
	CPoint m_ptCurStart;
	//����������
	CPoint m_ptCurEnd;
public:
	// ����������
	BOOL DoPaintLine(CDC * pDc);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// ������ʾ��	
	BOOL DoPaintTip(CDC * pDc);
private:
	// ͸����ֵ
	int m_nAlph;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	// �Ƿ���Ҫ���Ƹ�����Ϣ
	bool m_bPaintOther;
	// ���������
	bool m_bLbnt;
public:
	// ���ƽ�������
	BOOL DoCaptureRgn(CDC * pDc);
	// ���ƽ�ȡ���ı߿�
	BOOL DoPaintRectange(CDC * pDc);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	// ��ȡ�ͷ����
	bool m_bFinsh;
	// ����϶��������
	CPoint m_ptMoveStart;
	// ����϶�����������
	CPoint m_ptMoveEnd;
	// ��ȡ������
	CRect m_rtRgn;
public:
	// �ƶ���������
	BOOL MoveRectange(CPoint pt1, CPoint pt2);
	// ָ��ͼƬ���ƺͱ���ͼƬ��ָ��λ��
	BOOL SaveImage(const CString & filePath, const CString & filename);
};
