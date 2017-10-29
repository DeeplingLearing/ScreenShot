
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
	// 鼠标开始坐标
	CPoint m_ptCurStart;
	//鼠标结束坐标
	CPoint m_ptCurEnd;
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
	// 鼠标左键点击
	bool m_bLbnt;
public:
	// 绘制截屏区域
	BOOL DoCaptureRgn(CDC * pDc);
	// 绘制截取区的边框
	BOOL DoPaintRectange(CDC * pDc);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	// 截取释放完成
	bool m_bFinsh;
	// 鼠标拖动起点坐标
	CPoint m_ptMoveStart;
	// 鼠标拖动结束的坐标
	CPoint m_ptMoveEnd;
	// 截取的区间
	CRect m_rtRgn;
public:
	// 移动矩形区域
	BOOL MoveRectange(CPoint pt1, CPoint pt2);
	// 指定图片名称和保存图片到指定位置
	BOOL SaveImage(const CString & filePath, const CString & filename);
};
