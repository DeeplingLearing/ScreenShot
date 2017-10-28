
// screenshotDlg.h : 头文件
//

#pragma once


// CscreenshotDlg 对话框
class CscreenshotDlg : public CDialogEx
{
// 构造
public:
	CscreenshotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENSHOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 屏幕宽度
	int m_cxScreen;
	// 屏幕高
	int m_cyScreen;
	// 屏幕左上角x坐标
	int m_xScreen;
	// 屏幕做上角y坐标
	int m_yScreen;
};
