
// MFCDlg.h : header file
//

#pragma once

#define PI 3.14159265

// include header file with relative path
#include ".\include\GL\glew.h"
#include ".\include\GL\wglew.h"
#include ".\include\GL\freeglut.h"
#include ".\include\GL\freeglut_ext.h"
#include "afxwin.h"

// setting library to use
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

// CMFCDlg dialog
class CMFCDlg : public CDialogEx
{
// Construction
public:
	CMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Pdirection;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	HGLRC m_hRC;
	HGLRC m_hRC2;
	CDC* m_pDC;
	CDC* m_pDC2;
public:
	BOOL GetRenderingContext();
protected:
	virtual BOOL GetOldStyleRenderingContext();
	virtual BOOL SetupPixelFormat();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	float m_alpha;
	float m_beta;
	float m_gamma;

	// TODO : test
public:
	GLUquadricObj *IDquadric;
	int m_day;
	int m_time;
	void ch_drawCapsule();
	DIRECTION ch_dir;
	int m_dir;
};
