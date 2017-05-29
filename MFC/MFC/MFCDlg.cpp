
// MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDlg dialog



CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
	, m_pDC(NULL)
	, m_alpha(0)
	, m_beta(0)
	, m_gamma(0)
	, m_day(0)
	, m_time(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIRECTION, m_Pdirection);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCDlg message handlers

BOOL CMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// Create OpenGL context
	if (!GetRenderingContext())
	{
		AfxMessageBox("GetRenderingContext() error");
		return -1;
	}

	glClear(GL_COLOR_BUFFER_BIT);	// using double buffer and RGB color model
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	SetTimer(1000, 30, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		AfxMessageBox(CString("wglDeleteContext failed"));
	}
}


void CMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);

	

	ch_drawCapsule();
}


BOOL CMFCDlg::GetRenderingContext()
{
	//픽처 컨트롤에만 그리도록 DC 생성
	//참고 https://goo.gl/CK36zE
	CWnd* pImage = GetDlgItem(IDC_DIRECTION);
	CRect rc;
	pImage->GetWindowRect(rc);
	m_pDC = pImage->GetDC();


	if (NULL == m_pDC)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}


	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}

	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(CString("GLEW could not be initialized!"));
		return FALSE;
	}


	//참고 http://gamedev.stackexchange.com/a/30443
	GLint attribs[] =
	{
		// Here we ask for OpenGL 2.1
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		// Uncomment this for forward compatibility mode
		//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		// Uncomment this for Compatibility profile
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		// We are using Core profile here
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};


	HGLRC CompHRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	if (CompHRC && wglMakeCurrent(m_pDC->GetSafeHdc(), CompHRC))
		m_hRC = CompHRC;
	glClearColor(0.f, 0.f, 0.f, 0.5f);

	return TRUE;
}


BOOL CMFCDlg::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                        // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

	//Make it current. 
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		AfxMessageBox(CString("wglMakeCurrent failed"));
		return FALSE;
	}
	return TRUE;
}


BOOL CMFCDlg::SetupPixelFormat()
{
	//This is a modern pixel format attribute list.
	//It has an extensible structure. Just add in more argument pairs 
	//befroe the null to request more features.
	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
		WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,     32,
		WGL_DEPTH_BITS_ARB,     24,
		WGL_STENCIL_BITS_ARB,   8,
		0, 0  //End
	};


	unsigned int numFormats;
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;

	//Select a pixel format number
	wglChoosePixelFormatARB(m_pDC->GetSafeHdc(), attribList, NULL, 1, &pixelFormat, &numFormats);

	//Optional: Get the pixel format's description. We must provide a 
	//description to SetPixelFormat(), but its contents mean little.
	//According to MSDN: 
	//  The system's metafile component uses this structure to record the logical
	//  pixel format specification. The structure has no other effect upon the
	//  behavior of the SetPixelFormat function.
	//DescribePixelFormat(m_pDC->GetSafeHdc(), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	//Set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SelectPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}


BOOL CMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam)
		{
		case 65:
			m_alpha = 4;
			m_dir = 1;
			break;
		case 68:
			m_alpha = -4;
			m_dir = 1;
			break;
		case 87:
			m_beta = -4;
			m_dir = 0;
			break;
		case 83:
			m_beta = 4;
			m_dir = 0;
			break;
		case 49:
			m_dir = 2;
			break;
		case 50:
			m_dir = 3;
			break;
		case 51:
			m_dir = 4;
			break;
		default:
			break;
		}
	}
	if (pMsg->message == WM_KEYUP) {
		switch (pMsg->wParam)
		{
		case 65:
			m_dir = 5;
			break;
		case 68:
			m_dir = 5;
			break;
		case 87:
			m_dir = 5;
			break;
		case 83:
			m_dir = 5;
			break;
		default:
			break;
		}
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMFCDlg::ch_drawCapsule()
{
	glClear(GL_COLOR_BUFFER_BIT);	// using double buffer and RGB color model
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_LINES);
		glVertex3f(-1.f, 0.f, 0.f);
		glVertex3f(1.f, 0.f, 0.f);
		//glVertex3f(0.f, 0.f, -1.f);
		//glVertex3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, -1.f, 0.f);
		glVertex3f(0.f, 1.f, 0.f);
	glEnd();

	GLUquadricObj *pQuad;
	pQuad = gluNewQuadric();
	gluQuadricDrawStyle(pQuad, GLU_LINE);

	/**
	*
	* gluCylinder make cylinder
	*
	* @param baseRadius : radius from current point(base point)
	* @param topRadius : radius from current point + height(z axis), (top point)
	* @param height : length of cylinder from basePoint to TopPoint
	* @param slices : # of vertical lines
	* @param stacks : # of horizontal lines
	*/
	/// Cylindrical illustration
	int slices = 30;	// horizontal slices
	int stacks = 10;	// # of vertical stack
	int p = 20;
	int q = 20;
	float R = 0.3;

	switch (m_dir)
	{
	case X:
		glRotatef(m_beta, 1.f, 0.f, 0.f);
		break;
	case Y:
		glRotatef(m_alpha, 0.f, 1.f, 0.f);
		break;
	case AX:
		glLoadIdentity();
		glRotatef(90, 1.0, 0.0, 0.0);
		break;
	case AY:
		glLoadIdentity();
		glRotatef(90, 0.0, 1.0, 0.0);
		break;
	case AZ:
		glLoadIdentity();
		glRotatef(0, 0.0, 0.0, 1.0);
		break;
	default:
		break;
	}
	glPushMatrix();
		glTranslatef(0.f, 0.f, 0.3f);
		glRotatef(90.f, 1.f, 0.f, 0.f);
		glColor3f(1.f, 0.f, 0.f);
		glPointSize(7.f);
		glBegin(GL_POINTS);
			glVertex3f(0.f, 0.3f, 0.0f);
		glEnd();
		glColor3f(1.f, 1.f, 1.f);
		
		for (int j = 0; j < q; j++)
		{
			// One latitudinal triangle strip.
			glBegin(GL_TRIANGLE_STRIP_ADJACENCY);
			for (int i = 0; i <= p; i++)
			{
				glVertex3f(R * cos((float)(j + 1) / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)(j + 1) / q * PI / 2.0),
					R * cos((float)(j + 1) / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
				glVertex3f(R * cos((float)j / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)j / q * PI / 2.0),
					R * cos((float)j / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
			}
			glEnd();
		}
		glColor3f(1.f, 0.f, 0.f);
		for (int j = 0; j < p; j++)
		{
			glBegin(GL_LINES);
			glLineWidth(0.3f);
			for (int i = 0; i <= q; i++)
			{
				glVertex3f(R * cos((float)(j + 1) / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)(j + 1) / q * PI / 2.0),
					R * cos((float)(j + 1) / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
				glVertex3f(R * cos((float)j / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)j / q * PI / 2.0),
					R * cos((float)j / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
			}
			glEnd();
		}
		glColor3f(1.f, 1.f, 1.f);
		glRotatef(-90.f, 1.f, 0.f, 0.f);
		glTranslatef(0.f, 0.f, -0.3f);
		gluCylinder(pQuad, 0.3, 0.3, 0.3, slices, stacks);
		glTranslatef(0.f, 0.f, -0.3f);
		gluCylinder(pQuad, 0.3, 0.3, 0.3, slices, stacks);
		glRotatef(-90.f, 1.f, 0.f, 0.f);
		for (int j = 0; j < q; j++)
		{
			// One latitudinal triangle strip.
			glBegin(GL_TRIANGLE_STRIP_ADJACENCY);
			for (int i = 0; i <= p; i++)
			{
				glVertex3f(R * cos((float)(j + 1) / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)(j + 1) / q * PI / 2.0),
					R * cos((float)(j + 1) / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
				glVertex3f(R * cos((float)j / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)j / q * PI / 2.0),
					R * cos((float)j / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
			}
			glEnd();
		}
		for (int j = 0; j < p; j++)
		{
			glBegin(GL_LINES);
			glLineWidth(0.3f);
			for (int i = 0; i <= q; i++)
			{
				glVertex3f(R * cos((float)(j + 1) / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)(j + 1) / q * PI / 2.0),
					R * cos((float)(j + 1) / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
				glVertex3f(R * cos((float)j / q * PI / 2.0) * cos(2.0 * (float)i / p * PI),
					R * sin((float)j / q * PI / 2.0),
					R * cos((float)j / q * PI / 2.0) * sin(2.0 * (float)i / p * PI));
			}
			glEnd();
		}
	glPopMatrix();

	//화면 업데이트
	SwapBuffers(m_pDC->GetSafeHdc());
}
