
// screenshotDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"


// CscreenshotDlg 对话框
class CscreenshotDlg : public CDialogEx
{
// 构造
public:
	CscreenshotDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CscreenshotDlg();         //析构函数
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
	// 背景图片
	CBitmap *m_pBmp;
protected:
	// 抓取图片
	BOOL CaputerImage();
public:
	// 绘制背景图片
	BOOL OnPaintBackgroundImage(CDC * pDc);
private:
	// 是否绘制透明背景色
	bool m_bMasked;
	// 鼠标坐标
	CPoint m_ptCur;
public:
	// 绘制坐标线
	BOOL DoPaintLine(CDC * pDc);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// 绘制提示框	
	BOOL DoPaintTip(CDC * pDc);
private:
	// 透明度值
	int m_nAlph;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	// 是否需要绘制辅助信息
	bool m_bPaintOther;
};
