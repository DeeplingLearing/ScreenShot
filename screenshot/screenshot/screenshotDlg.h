
// screenshotDlg.h : ͷ�ļ�
//

#pragma once


// CscreenshotDlg �Ի���
class CscreenshotDlg : public CDialogEx
{
// ����
public:
	CscreenshotDlg(CWnd* pParent = NULL);	// ��׼���캯��

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
};
