
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
	// �������
	CPoint m_ptCur;
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
};
