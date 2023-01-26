
// ImageSimulationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageSimulation.h"
#include "ImageSimulationDlg.h"
#include "afxdialogex.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CImageSimulationDlg g_Dlg_Main;

// CImageSimulationDlg g_Dlg_Main;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CImageSimulationDlg dialog




CImageSimulationDlg::CImageSimulationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageSimulationDlg::IDD, pParent)
	, m_nPyrRS(0)
	, m_nPyrDN(2)
	, m_nGKernel(15)
	, m_nTH(150)
	, m_nROI_x(100)
	, m_nROI_y(100)
	, m_nROI_width(200)
	, m_nROI_height(200)
	, m_bShowROI(FALSE)
	, m_bROISet(FALSE)
	, m_bImageDebug(FALSE)
	, m_sEVisionLicense(_T(""))
	, m_sPreLearnMode(_T(""))
	, m_sOpenCVVersion(_T(""))
	, m_bFolderInspect(FALSE)
	, m_dbFov(3.45)
	, m_fSamplingRatio(0.7f)
	, m_fMinCircleRadius(0)
	, m_nCircleROI_w(50)
	, m_nCircleROI_h(50)
	, m_nPerInRect(20)
	, m_nRangeInRect(50)
	, m_nCircleFindMode(0)
	, m_nRectFindMode(0)
	, m_nRectTH(50)
{

}

void CImageSimulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE_VIEW, m_pImageView);

	DDX_Text(pDX, IDC_EDIT_PYR_RESIZE, m_nPyrRS);
	DDX_Text(pDX, IDC_EDIT_PYR_DENOISE, m_nPyrDN);
	DDX_Text(pDX, IDC_EDIT_G_KERNEL, m_nGKernel);
	DDX_Text(pDX, IDC_EDIT_PYR_DENOISE3, m_nTH);
	DDX_Text(pDX, IDC_EDIT_ROI_X, m_nROI_x);
	DDX_Text(pDX, IDC_EDIT_ROI_Y, m_nROI_y);
	DDX_Text(pDX, IDC_EDIT_ROI_WIDTH, m_nROI_width);
	DDX_Text(pDX, IDC_EDIT_ROI_HEIGHT, m_nROI_height);
	DDX_Check(pDX, IDC_CHECK_SHOW_ROI, m_bShowROI);
	DDX_Check(pDX, IDC_CHECK_SET_ROI, m_bROISet);
	DDX_Check(pDX, IDC_CHECK_SHOW_ROI2, m_bImageDebug);
	DDX_Text(pDX, IDC_STATIC_EVISION_LISENSE, m_sEVisionLicense);
	DDX_Text(pDX, IDC_STATIC_PRE_LEARNMODE, m_sPreLearnMode);
	DDX_Text(pDX, IDC_STATIC_OPENCV_VERSION, m_sOpenCVVersion);
	DDX_Check(pDX, IDC_CHECK_FOLDER_INSPECT, m_bFolderInspect);
	DDX_Text(pDX, IDC_EDIT_FOV, m_dbFov);
	DDX_Text(pDX, IDC_EDIT_SAMPLING_RATIO, m_fSamplingRatio);
	DDX_Text(pDX, IDC_EDIT_MIN_CIRCLE_RADIUS, m_fMinCircleRadius);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_ROI_WIDTH, m_nCircleROI_w);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_ROI_HEIGHT, m_nCircleROI_h);
	DDX_Text(pDX, IDC_EDIT_RECT_INOFFSET, m_nPerInRect);
	DDX_Text(pDX, IDC_EDIT_RECT_RANGE, m_nRangeInRect);
	DDX_Radio(pDX, IDC_RADIO_CIRCLE_FIND_MODE, m_nCircleFindMode);
	DDX_Radio(pDX, IDC_RADIO_CIRCLE_FIND_MODE3, m_nRectFindMode);
	DDX_Text(pDX, IDC_EDIT_RECT_TH, m_nRectTH);
}

BEGIN_MESSAGE_MAP(CImageSimulationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CImageSimulationDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CImageSimulationDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CImageSimulationDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ROI, &CImageSimulationDlg::OnBnClickedCheckShowRoi)
	ON_BN_CLICKED(IDC_BUTTON_PRE_PROCESS, &CImageSimulationDlg::OnBnClickedButtonPreProcess)
	ON_BN_CLICKED(IDC_BUTTON_RANSAC_USE_BINARY, &CImageSimulationDlg::OnBnClickedButtonRansacUseBinary)
	ON_BN_CLICKED(IDC_BUTTON_RANSAC_USE_NOMAL_IMAGE, &CImageSimulationDlg::OnBnClickedButtonRansacUseNomalImage)
	ON_BN_CLICKED(IDC_CHECK_SET_ROI, &CImageSimulationDlg::OnBnClickedCheckSetRoi)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CImageSimulationDlg::OnBnClickedButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_MATCHING, &CImageSimulationDlg::OnBnClickedButtonPatternMatching)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ROI2, &CImageSimulationDlg::OnBnClickedCheckShowRoi2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_FOLDER_INSPECT, &CImageSimulationDlg::OnBnClickedCheckFolderInspect)
	ON_BN_CLICKED(IDC_BUTTON_INSPECT, &CImageSimulationDlg::OnBnClickedButtonInspect)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_FIND_MODE, &CImageSimulationDlg::OnBnClickedRadioCircleFindMode)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_FIND_MODE2, &CImageSimulationDlg::OnBnClickedRadioCircleFindMode2)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_FIND_MODE3, &CImageSimulationDlg::OnBnClickedRadioCircleFindMode3)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_FIND_MODE4, &CImageSimulationDlg::OnBnClickedRadioCircleFindMode4)
	ON_BN_CLICKED(IDC_BUTTON_CIRCLE_FIND, &CImageSimulationDlg::OnBnClickedButtonCircleFind)
END_MESSAGE_MAP()


// CImageSimulationDlg message handlers

BOOL CImageSimulationDlg::OnInitDialog()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CDialog::OnInitDialog();	
	
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

	pDlg_EP = new EVisionProcess();
	pDlg_EP->Create(IDD_EVISION_PROCESS, this);
	pDlg_EP->ShowWindow(SW_HIDE);

	InitParam();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImageSimulationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImageSimulationDlg::OnPaint()
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
		DrawImage(m_mat);
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImageSimulationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageSimulationDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];
	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;
	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}
void CImageSimulationDlg::DrawImage(Mat mat)
{
	if(mat.empty())
		return;

	CWnd* pWnd = GetDlgItem(IDC_IMAGE_VIEW);
	CClientDC dc(pWnd);
	RECT rect;
	pWnd->GetClientRect(&rect);

	Mat dp;
	cvtColor(mat, dp, CV_GRAY2BGR);

	if(m_bShowROI)
	{
		Rect showRect;
		showRect = SetCircleRect(m_rRect);

		rectangle(dp, cv::Point(m_nROI_x, m_nROI_y), cv::Point(m_nROI_x+m_nROI_width, m_nROI_y+m_nROI_height), Scalar(0, 255, 0), 2, 8, 0);
		rectangle(dp, cv::Point(showRect.x, showRect.y), cv::Point(showRect.x+showRect.width, showRect.y+showRect.height), Scalar(0, 255, 0), 2, 8, 0);
		
	}

	if(m_InsResult.mRS.ran_circ.bFind == TRUE || m_InsResult.mMC.bFind == TRUE)
		ShowFinalResult(dp, m_nPyrRS, m_rRect, m_InsResult);
		 
	IplImage* bgr_frame;
	IplImage copy;
	copy = dp;
	bgr_frame = &copy;

	CvvImage vImg;
	vImage.CopyOf(bgr_frame, 1);

	vImage.DrawToHDC(dc.m_hDC, &rect);
	
	if(m_nRectFindMode == RECTANGLE_EVIEION && m_InsResult.mMC.bFind)
	{
		float fscaleX, fscaleY;
		fscaleX = (float)rect.right / (float)pDlg_EP->ePatImage.GetWidth(); 
		fscaleY = (float)rect.bottom / (float)pDlg_EP->ePatImage.GetHeight();

		HPEN rPen = CreatePen(PS_DOT, 1, DPINK);
		HGDIOBJ rOldPen = (HPEN)SelectObject(dc, rPen);

		m_eMatch.DrawPositions(dc.m_hDC, FALSE, fscaleX, fscaleY,0,0);

		SelectObject(dc.m_hDC,rOldPen); // 이전브러쉬를 다시 선택
		DeleteObject(rPen); // 새로 생성한 브러쉬 삭제
	}
}

void CImageSimulationDlg::OnBnClickedButton1()
{
	CString str = _T("Image files(*.jpg, *.bmp, *png)|*.jpg; *.bmp; *png|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	if (dlg.DoModal() == IDOK)
	{		
		InitResult(m_InsResult);

		m_strPathName = dlg.GetPathName();	
		
		Mat input;
		input = imread(string(CT2CA(m_strPathName)), CV_LOAD_IMAGE_GRAYSCALE);

		if(!m_mat.empty())
			m_mat.release();

		m_mat = ImageResize(input, m_nPyrRS, m_nPyrDN);
		
		DrawImageInformation(m_mat, m_strPathName);		
		CreateBitmapInfo(m_mat.cols, m_mat.rows, m_mat.channels()*8);
		DrawImage(m_mat);
	}
}

Mat CImageSimulationDlg::ImageResize(Mat mat, int rs, int dn)
{
	Mat _r = mat.clone();

	IP.PrymidResize(_r, rs);
	IP.PrymidDenoise(_r, dn);

	return _r;
}

void CImageSimulationDlg::DrawImageInformation(Mat mat, CString path)
{
	CString sTemp;
	GetDlgItem(IDC_STATIC_IMAGE_PATH)->SetWindowText(path);	

	sTemp.Format("  %d", mat.cols);		// 너비
	GetDlgItem(IDC_STATIC_IMAGE_WIDTH)->SetWindowText(sTemp);
	sTemp.Format("  %d", mat.rows);		// 높이
	GetDlgItem(IDC_STATIC_IMAGE_HEIGHT)->SetWindowText(sTemp);
	sTemp.Format("  %d", mat.channels()*8);	// 비트수준
	GetDlgItem(IDC_STATIC_IMAGE_DEPTH)->SetWindowText(sTemp);
	sTemp.Format("  %s", IP.CSChannel(mat.channels()));		// 흑백, 칼라
	GetDlgItem(IDC_STATIC_IMAGE_CHANNEL)->SetWindowText(sTemp);
	UpdateData(FALSE);
}

void CImageSimulationDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);

	if((m_nGKernel % 2) == 0)
	{
		AfxMessageBox("Kernel 값은 홀수로 동작합니다.");
		return;
	}

	m_rRect.x = m_nROI_x;
	m_rRect.y = m_nROI_y;
	m_rRect.width = m_nROI_width;
	m_rRect.height = m_nROI_height;

	SaveData(PARAMETER_PATH);
	UpdateData(FALSE);

	DrawImage(m_mat);
}


void CImageSimulationDlg::SaveData(CString filename)
{
	FILE *file;
	fopen_s(&file, filename,"w+");

	if(file == NULL)
		return;

	fprintf(file, "0001|m_nPyrRS =%d\n", m_nPyrRS);
	fprintf(file, "0002|m_nPyrDN =%d\n", m_nPyrDN);
	fprintf(file, "0003|m_nGKernel =%d\n", m_nGKernel);
	fprintf(file, "0004|m_nTH =%d\n", m_nTH);
	fprintf(file, "0005|m_nROI_x =%d\n", m_nROI_x);
	fprintf(file, "0006|m_nROI_y =%d\n", m_nROI_y);
	fprintf(file, "0007|m_nROI_width =%d\n", m_nROI_width);
	fprintf(file, "0008|m_nROI_height =%d\n", m_nROI_height);	
	fprintf(file, "0009|m_bShowROI =%d\n", m_bShowROI);
	fprintf(file, "0010|m_rROIRect.x =%d\n", pDlg_EP->m_rROIRect.x);
	fprintf(file, "0011|m_rROIRect.y =%d\n", pDlg_EP->m_rROIRect.y);
	fprintf(file, "0012|m_rROIRect.width =%d\n", pDlg_EP->m_rROIRect.width);
	fprintf(file, "0013|m_rROIRect.height =%d\n", pDlg_EP->m_rROIRect.height);	
	fprintf(file, "0014|m_bShowP_ROI =%d\n", pDlg_EP->m_bShowP_ROI);
	
	fprintf(file, "0015|m_nCBContrast =%d\n", pDlg_EP->m_nCBContrast);
	fprintf(file, "0016|m_nCBCorrMode =%d\n", pDlg_EP->m_nCBCorrMode);
	fprintf(file, "0017|m_fPAngleMax =%f\n", pDlg_EP->m_fPAngleMax);
	fprintf(file, "0018|m_fPAngleMin =%f\n", pDlg_EP->m_fPAngleMin);
	fprintf(file, "0019|m_fPScaleBothMax =%f\n", pDlg_EP->m_fPScaleBothMax);
	fprintf(file, "0020|m_fPScaleBothMin =%f\n", pDlg_EP->m_fPScaleBothMin);
	fprintf(file, "0021|m_fPScaleEachMax =%f\n", pDlg_EP->m_fPScaleEachMax);
	fprintf(file, "0022|m_fPScaleEachMin =%f\n", pDlg_EP->m_fPScaleEachMin);
	fprintf(file, "0023|m_bPScalMode =%d\n", pDlg_EP->m_bPScalMode);
	fprintf(file, "0024|m_bLearnComplete =%d\n", pDlg_EP->m_bLearnComplete);

	fprintf(file, "0025|m_dbFov =%.3f\n", m_dbFov);
	fprintf(file, "0026|m_fSamplingRatio =%.3f\n", m_fSamplingRatio);
	fprintf(file, "0027|m_fMinCircleRadius =%.3f\n", m_fMinCircleRadius);

	fprintf(file, "0028|m_nCircleROI_w =%d\n", m_nCircleROI_w);
	fprintf(file, "0029|m_nCircleROI_h =%d\n", m_nCircleROI_h);
	fprintf(file, "0030|m_nPerInRect =%d\n", m_nPerInRect);
	fprintf(file, "0031|m_nRangeInRect =%d\n", m_nRangeInRect);
	fprintf(file, "0032|m_nRectTH =%d\n", m_nRectTH);
	
	
	fclose(file);

	return;
}

void CImageSimulationDlg::InitParam()
{
	m_pBitmapInfo = NULL;	
	
	LoadData(PARAMETER_PATH);

	m_rRect.x = m_nROI_x;
	m_rRect.y = m_nROI_y;
	m_rRect.width = m_nROI_width;
	m_rRect.height = m_nROI_height;

	InitResult(m_InsResult);

//	SetTimer(TIMER_REFRESH, 2000, NULL);
	VersionCheck();
	pDlg_EP->UpLoadData();
	UpdateData(FALSE);
	return;
}
void CImageSimulationDlg::LoadData(CString filename)
{
	CStdioFile File;

	if(!File.Open((LPCTSTR)filename, CFile::modeRead | CFile::typeText )) 
		return;

	int index;//, start, cnt, sub_cnt;
	CString line, temp;

	while(NULL != File.ReadString(line))
	{
		if(line.GetAt(0) == '#' || line.GetAt(0) == ' ' || line.GetAt(0) == '\0' || line.GetAt(0) == '\n')
			continue;

		index = atoi(line.Mid(0, line.Find('|')));
		temp.Format("%s", line.Mid(line.Find('=')+1));	
		temp.Remove('\n');

		switch(index)
		{
		case 1:			m_nPyrRS = atoi(temp);						break;
		case 2:			m_nPyrDN = atoi(temp);						break;
		case 3:			m_nGKernel = atoi(temp);					break;
		case 4:			m_nTH = atoi(temp);							break;
		case 5:			m_nROI_x = atoi(temp);						break;
		case 6:			m_nROI_y = atoi(temp);						break;
		case 7:			m_nROI_width = atoi(temp);					break;
		case 8:			m_nROI_height = atoi(temp);					break;
		case 9:			m_bShowROI = atoi(temp);					break;
		case 10:		pDlg_EP->m_rROIRect.x = atoi(temp);			break;
		case 11:		pDlg_EP->m_rROIRect.y = atoi(temp);			break;
		case 12:		pDlg_EP->m_rROIRect.width = atoi(temp);		break;
		case 13:		pDlg_EP->m_rROIRect.height = atoi(temp);	break;
		case 14:		pDlg_EP->m_bShowP_ROI = atoi(temp);			break;

		case 15:		pDlg_EP->m_nCBContrast = atoi(temp);		break;
		case 16:		pDlg_EP->m_nCBCorrMode = atoi(temp);		break;
		case 17:		pDlg_EP->m_fPAngleMax = atof(temp);			break;
		case 18:		pDlg_EP->m_fPAngleMin = atof(temp);			break;
		case 19:		pDlg_EP->m_fPScaleBothMax = atof(temp);		break;
		case 20:		pDlg_EP->m_fPScaleBothMin = atof(temp);		break;
		case 21:		pDlg_EP->m_fPScaleEachMax = atof(temp);		break;
		case 22:		pDlg_EP->m_fPScaleEachMin = atof(temp);		break;
		case 23:		pDlg_EP->m_bPScalMode = atoi(temp);			break;
		case 24:		pDlg_EP->m_bLearnComplete = atoi(temp);		break;

		case 25:		m_dbFov = atof(temp);						break;
		case 26:		m_fSamplingRatio = atof(temp);				break;
		case 27:		m_fMinCircleRadius = atof(temp);			break;

		case 28:		m_nCircleROI_w = atoi(temp);				break;
		case 29:		m_nCircleROI_h = atoi(temp);				break;
		case 30:		m_nPerInRect = atoi(temp);					break;
		case 31:		m_nRangeInRect = atoi(temp);				break;
		case 32:		m_nRectTH = atoi(temp);						break;
			
		}
	}
	File.Close();

	return;
}

void CImageSimulationDlg::OnBnClickedButton3()
{
// 	pDlg_EP->simgPath = m_strPathName;
// 	char *pChar = (LPSTR)(LPCTSTR)(m_strPathName);
// 	pDlg_EP->eSrcImage.Load(pChar);
// 	pDlg_EP->SetZoom();
	//LoadData(PARAMETER_PATH);
	pDlg_EP->UpLoadData();
	pDlg_EP->bEPEnable = TRUE;
	pDlg_EP->ShowWindow(SW_SHOW);
}


void CImageSimulationDlg::OnBnClickedCheckShowRoi()
{
	UpdateData(TRUE);
}


void CImageSimulationDlg::OnBnClickedButtonPreProcess()
{	
	IP.RansacPreprocess(m_mat, m_tMat, m_nGKernel, m_nTH);	
}


void CImageSimulationDlg::ShowResultImage(string sw, Mat mat)
{
	namedWindow(sw, WINDOW_NORMAL);	
	imshow(sw, mat);
}


void CImageSimulationDlg::OnBnClickedButtonRansacUseBinary()
{
	if(m_mat.empty())
	{
		AfxMessageBox("이미지 로드를 먼저 진행해야 합니다.");
		return;
	}

//	destroyAllWindows();

	// 이미지 전처리 후 진행
//	IP.RansacPreprocess(m_mat, m_tMat, m_nGKernel, m_nTH);
	BOOL bRtn = FALSE;
	__Circle _c;
	bRtn = CircleFit(m_nPyrRS, m_mat, SetCircleRect(m_rRect), _c, TRUE, m_nGKernel, m_nTH, m_bImageDebug);
	SetRSCResult(bRtn, _c);

	if(!bRtn)		
		AfxMessageBox("Search Fail");
	DrawImage(m_mat);
}

double CImageSimulationDlg::change_PrymidValue(int rsize, double val)
{
	double _val, _r_val;
	_val = val;

	for(int r = 0; r < rsize; r++)
		_val /= 2;

	_r_val = _val;
	return _r_val;
}

Rect CImageSimulationDlg::rect_normalize(int rsize, Rect rect)
{
	Rect _r;
	_r = rect;
	for(int r = 0; r < rsize-1; r++)
	{
		_r.x /= 2;
		_r.y /= 2;
		_r.width /= 2;
		_r.height /= 2;
	}

	return _r;
}

BOOL CImageSimulationDlg::CircleFit(int rsize, Mat src, Rect roi, __Circle& rc, BOOL bBin, int gK, int nTH, BOOL bImgDebug)
{
	__Circle _c;

	Rect rect_roi;
	
	LARGE_INTEGER timer, start, end;

	QueryPerformanceFrequency(&timer);
	QueryPerformanceCounter(&start);

	rect_roi = rect_normalize(rsize, roi);
	//ShowResultImage(BINARY_IMAGE, src);

	_c = IP.RansacCircleFit(src, rect_roi, bBin, gK, nTH, bImgDebug, m_fMinCircleRadius, m_fSamplingRatio);	// 이미지 전처리 후 진행

	QueryPerformanceCounter(&end);

	if(_c.bFind == TRUE)
	{
		_c.tact = (end.QuadPart - start.QuadPart)/(float)(timer.QuadPart)*1000;

		rc = _c;
		return TRUE;
	}

	return FALSE;
}

void CImageSimulationDlg::InspectResult(Mat mat, int rsize, Rect roi, __Circle c)
{
	// 결과 표시
	Mat _t = mat.clone();	

	if(mat.empty())
		return;

	Mat _rm;

	if(_t.channels() == 1)
		cvtColor(_t, _rm, CV_GRAY2BGR);
	else
		_rm = _t.clone();

	CString st;
	char buf1[100];
	
// 	sprintf_s(buf1, "Tact [ %8.3f ms ]", c.tact);
// 	putText(_rm, buf1, cv::Point(20, 30), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(150, 150, 150), 2, 1);

	sprintf_s(buf1, "Center pos : %d, %d ", c.center.x, c.center.y);
	putText(_rm, buf1, cv::Point(20, 30), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(150, 150, 150), 2, 1);

	circle(_rm, c.center, c.radius, cv::Scalar(255,255,0),2);
	circle(_rm, c.center, 2, Scalar(255,0,0),2);

	ShowResultImage(RESULT_WINDOW, _rm);
}
void CImageSimulationDlg::ShowFinalResult(Mat mat, int rsize, Rect roi, __MatchResult mr)
{
	// 결과 표시
	Mat _t = mat;

	if(mat.empty())
		return;

	Mat _rm;

	if(_t.channels() == 1)
		cvtColor(_t, _rm, CV_GRAY2BGR);
	else
		_rm = _t;

	CString st;
	char buf1[100];

	if(mr.mRS.ran_circ.bFind== TRUE)
	{
		sprintf_s(buf1, "Circle Center pos : %d, %d ", mr.mRS.ran_circ.center.x, mr.mRS.ran_circ.center.y);
		putText(_rm, buf1, cv::Point(mr.mRS.ran_circ.center.x, mr.mRS.ran_circ.center.y-20), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 2, 1);

		circle(_rm, mr.mRS.ran_circ.center, mr.mRS.ran_circ.radius, cv::Scalar(255,255,0),2);
		circle(_rm, mr.mRS.ran_circ.center, 3, Scalar(0,0,255),2);		
	}	

	if(m_nRectFindMode == RECTANGLE_EVIEION)
	{
		if(mr.mMC.bFind == TRUE)
		{
			CString sText;		
			circle(_rm, cv::Point(mr.mMC.mMCpos.m_f32CenterX, mr.mMC.mMCpos.m_f32CenterY), 2, Scalar(0,255,0),2);

			sprintf_s(buf1, "Rect Center pos : %.f, %.f ", mr.mMC.mMCpos.m_f32CenterX, mr.mMC.mMCpos.m_f32CenterY);
			putText(_rm, buf1, cv::Point((int)mr.mMC.mMCpos.m_f32CenterX, (int)mr.mMC.mMCpos.m_f32CenterY + 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0), 2, 1);

			sprintf_s(buf1, "Pattern Matching Rate : %.3f", mr.mMC.mMCpos.m_f32Score*100);
			putText(_rm, buf1, cv::Point(20, 70), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);
		}

		if(mr.mRS.ran_circ.bFind && mr.mMC.bFind)
		{
			sprintf_s(buf1, "Circle - Rect Dist : %.3f [pixel]", mr.dist);
			putText(_rm, buf1, cv::Point(20, 110), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);

			sprintf_s(buf1, "Distance w : %.3f, H : %.3f [um]", (mr.mMC.mMCpos.m_f32CenterX - mr.mRS.ran_circ.center.x) * m_dbFov, (mr.mMC.mMCpos.m_f32CenterY - mr.mRS.ran_circ.center.y) * m_dbFov);
			putText(_rm, buf1, cv::Point(20, 150), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);

			line(_rm, mr.mRS.ran_circ.center, cv::Point(mr.mMC.mMCpos.m_f32CenterX, mr.mMC.mMCpos.m_f32CenterY), Scalar(0, 255, 255), 2,8,0);

			sprintf_s(buf1, "Tact : %.3f", mr.mRS.ran_circ.tact + mr.mMC.tact);
			putText(_rm, buf1, cv::Point(20, 30), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);
		}
	}
	else
	{
		if(mr.mRS.ran_rect.bFind == TRUE)
		{
			CString sText;		
			circle(_rm, mr.mRS.ran_rect.center, 2, Scalar(0,255,0),2);

			sprintf_s(buf1, "Rect Center pos : %d, %d ", mr.mRS.ran_rect.center.x, mr.mRS.ran_rect.center.y);
			putText(_rm, buf1, cv::Point((int)mr.mRS.ran_rect.center.x, (int)mr.mRS.ran_rect.center.y + 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,0), 2, 1);

			for(int i = 0; i < 4; i++)
			{
				if( i == 3)
				{
					cv::line(_rm, mr.mRS.ran_rect.vertex[i], mr.mRS.ran_rect.vertex[0], Scalar(0,0,255), 1);

					sprintf_s(buf1, "angle : %.3f", mr.mRS.ran_rect.angle);
					putText(_rm, buf1, cv::Point(mr.mRS.ran_rect.vertex[i].x+10, mr.mRS.ran_rect.vertex[i].y+10), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(150, 150, 150), 2, 1);
				}

				circle(_rm, mr.mRS.ran_rect.vertex[i],2,Scalar(255,255,0), 3);

				if(i > 0)
				{
					cv::line(_rm, mr.mRS.ran_rect.vertex[i-1], mr.mRS.ran_rect.vertex[i], Scalar(0,0,255), 1);
				}
			}

			// 두직선의 교점 구하기
			// 좌상단, 우하단의 좌표와 우상단, 좌하단의 좌표를 가진 직선을 활용해 교차점을 구하자

			cv::line(_rm, mr.mRS.ran_rect.vertex[0], mr.mRS.ran_rect.vertex[2], Scalar(0,255,255), 1);
			cv::line(_rm, mr.mRS.ran_rect.vertex[1], mr.mRS.ran_rect.vertex[3], Scalar(0,255,255), 1);

			circle(_rm, mr.mRS.ran_rect.center,2,Scalar(0,255,0), 3);
		}

		if(mr.mRS.ran_circ.bFind &&mr.mRS.ran_rect.bFind)
		{
			sprintf_s(buf1, "Circle - Rect Dist : %.3f [pixel]", mr.dist);
			putText(_rm, buf1, cv::Point(20, 70), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);

			sprintf_s(buf1, "Distance w : %.3f, H : %.3f [um]", (mr.mRS.ran_rect.center.x - mr.mRS.ran_circ.center.x) * m_dbFov, (mr.mRS.ran_rect.center.y - mr.mRS.ran_circ.center.y) * m_dbFov);
			putText(_rm, buf1, cv::Point(20, 110), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);

			line(_rm, mr.mRS.ran_circ.center, mr.mRS.ran_rect.center, Scalar(0, 255, 255), 2,8,0);

			sprintf_s(buf1, "Tact : %.3f", mr.mRS.ran_circ.tact + mr.mRS.ran_rect.tact);
			putText(_rm, buf1, cv::Point(20, 30), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(150, 150, 150), 3, 1);
		}
	}
	
}

void CImageSimulationDlg::OnBnClickedButtonRansacUseNomalImage()
{
	if(m_mat.empty())
	{
		AfxMessageBox("이미지 로드를 먼저 진행해야 합니다.");
		return;
	}	

	BOOL bRtn = FALSE;
	__Circle _c;
	bRtn = CircleFit(m_nPyrRS, m_mat, SetCircleRect(m_rRect), _c, FALSE, m_nGKernel, m_nTH, m_bImageDebug);
	SetRSCResult(bRtn, _c);

	if(!bRtn)		
		AfxMessageBox("Search Fail");
	DrawImage(m_mat);
}

BOOL CImageSimulationDlg::PreTranslateMessage(MSG* pMsg)
{
	MapMessage(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}


void CImageSimulationDlg::MapMessage(MSG* pMsg)
{
	if(m_mat.empty())
		return;

	GetDlgItem(IDC_IMAGE_VIEW)->GetClientRect(m_rCtrlClientRect);
	GetDlgItem(IDC_IMAGE_VIEW)->GetWindowRect(m_rCtrlWndRect);

	CPoint pos;
	pos.x = pMsg->pt.x - m_rCtrlWndRect.left;
	pos.y = pMsg->pt.y - m_rCtrlWndRect.top;

	if(pos.x >= 0 && pos.y >= 0 && pos.x < m_rCtrlClientRect.right && pos.y < m_rCtrlClientRect.bottom && m_bROISet)
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN:			
			{
				pos.x = long(pos.x * double(m_mat.cols / double(m_rCtrlClientRect.right)));
				pos.y = long(pos.y * double(m_mat.rows / double(m_rCtrlClientRect.bottom)));

				m_pDrawStart.x = pos.x;
				m_pDrawStart.y = pos.y;

// 				m_rRefROIRect.left = pos.x;
// 				m_rRefROIRect.top = pos.y;
// 				m_rRefROIRect.right = m_rRefROIRect.left;
// 				m_rRefROIRect.bottom = m_rRefROIRect.top;
				m_nROI_x = m_pDrawStart.x;
				m_nROI_y = m_pDrawStart.y;
			}
			m_bLDown = TRUE;
			break;
		case WM_LBUTTONUP:
			{
				pos.x = long(pos.x * double(m_mat.cols / double(m_rCtrlClientRect.right)));
				pos.y = long(pos.y * double(m_mat.rows / double(m_rCtrlClientRect.bottom)));

// 				m_rRefROIRect.right = pos.x;
// 				m_rRefROIRect.bottom = pos.y;

				// 				msg.rect.NormalizeRect();
				// 				m_rDrawRect.right = pos.x;
				// 				m_rDrawRect.bottom = pos.y;

				/*				msg.rect = m_rDrawRect;*/

				/*				msg.nEvent = WM_CAM_CLICK_RECT;*/
				m_nROI_x = m_pDrawStart.x;
				m_nROI_y = m_pDrawStart.y;
				m_nROI_width = m_pDrawEnd.x - m_nROI_x;
				m_nROI_height = m_pDrawEnd.y - m_nROI_y;

				UpdateData(FALSE);
			}
			m_bLDown = FALSE;
			break;
		case WM_MOUSEMOVE:
			if(m_bLDown == TRUE)
			{
				pos.x = long(pos.x * double(m_mat.cols  / double(m_rCtrlClientRect.right)));
				pos.y = long(pos.y * double(m_mat.rows / double(m_rCtrlClientRect.bottom)));

				m_pDrawEnd.x = pos.x;
				m_pDrawEnd.y = pos.y;
				m_nROI_width = m_pDrawEnd.x - m_nROI_x;
				m_nROI_height = m_pDrawEnd.y - m_nROI_y;
				//Mat labeled = Mat::zeros(m_mat.rows, m_mat.cols, CV_8SC3);
				//cvtColor(m_mat, labeled, CV_GRAY2BGR);

				//CvScalar green = cvScalar(0, 255, 0);
				//rectangle(labeled, cv::Point(m_pDrawStart.x, m_pDrawStart.y), cv::Point(m_pDrawEnd.x, m_pDrawEnd.y), Scalar(0, 255, 0), 2, 8, 0);
				
				//cvDrawRect(labeled, m_pDrawStart, m_pDrawEnd, green, 3);

				DrawImage(m_mat);				
			}
			break;
		}
	}

	return;

}

void CImageSimulationDlg::OnBnClickedCheckSetRoi()
{
	UpdateData(TRUE);

	if(!m_bROISet)
		m_bLDown = FALSE;
}

void CImageSimulationDlg::OnBnClickedButtonReload()
{
	LoadData(PARAMETER_PATH);

	if(m_strPathName != "")
	{
		m_mat.release();
		m_mat = imread(string(CT2CA(m_strPathName)), CV_LOAD_IMAGE_GRAYSCALE);
		m_tMat.release();
		m_rMat.release();
	}

	UpdateData(FALSE);
}


void CImageSimulationDlg::OnBnClickedButtonPatternMatching()
{
	if(m_strPathName == "")
	{
		AfxMessageBox("대상 이미지가 Laod 되지 않았습니다.");
		return;
	}
	if(!pDlg_EP->m_bLearnComplete)
	{
		AfxMessageBox("Pattern Matching Recipe를 통해 Learn을 진행해주십시오");
		return;
	}	

	if(m_nRectFindMode == RECTANGLE_EVIEION)
	{
		InitEMATCHParam();

		if(pDlg_EP->m_bLearnComplete)
		{
			pDlg_EP->PreprocessLearn();
			pDlg_EP->eImageLoad(MATCH_PATRN_IMAGE, m_strPathName);		

			m_eMatch = pDlg_EP->TempleteMatch(pDlg_EP->eSrcImage, pDlg_EP->ePatImage, pDlg_EP->eRoiImage, pDlg_EP->m_rROIRect, TRUE, FALSE, TRUE);
			SetEMResult(m_eMatch);		
		}
	}
	else
	{
		InitRANSACParam(1);

		__RANSAC _r;
		_r = IP.RansacProcess(RANSAC_LINE, m_mat, m_rRect, TRUE, m_nGKernel, m_nRectTH, m_bImageDebug, m_nRangeInRect, m_fSamplingRatio, m_nPerInRect);
		SetRSRResult(_r.ran_rect.bFind, _r.ran_rect);
	}

	DrawImage(m_mat);
}

void CImageSimulationDlg::SetEMResult(EMatch mch)
{	
	EMatchPosition* position;
	if(mch.GetNumPositions() > 0)
	{
		position = mch.GetPosition(0);
		
		m_InsResult.mMC.bFind = TRUE;
		m_InsResult.mMC.mMCpos.m_bInterpolated = position->m_bInterpolated;
		m_InsResult.mMC.mMCpos.m_f32Angle = position->m_f32Angle;
		m_InsResult.mMC.mMCpos.m_f32AreaRatio = position->m_f32AreaRatio;
		m_InsResult.mMC.mMCpos.m_f32CenterX = (float)change_PrymidValue(m_nPyrRS, (double)position->m_f32CenterX);
		m_InsResult.mMC.mMCpos.m_f32CenterY = (float)change_PrymidValue(m_nPyrRS, (double)position->m_f32CenterY);
		m_InsResult.mMC.mMCpos.m_f32Reserved = position->m_f32Reserved;
		m_InsResult.mMC.mMCpos.m_f32Scale = position->m_f32Scale;
		m_InsResult.mMC.mMCpos.m_f32ScaleX = position->m_f32ScaleX;
		m_InsResult.mMC.mMCpos.m_f32ScaleY = position->m_f32ScaleY;
		m_InsResult.mMC.mMCpos.m_f32Score = position->m_f32Score;
	}
	else
		InitEMATCHParam();

	if(m_InsResult.mMC.bFind && m_InsResult.mRS.ran_circ.bFind)
	{
		m_InsResult.dist = Distance(m_InsResult.mRS.ran_circ.center, cv::Point(m_InsResult.mMC.mMCpos.m_f32CenterX, m_InsResult.mMC.mMCpos.m_f32CenterY));
	}
}

void CImageSimulationDlg::SetRSCResult(BOOL bSuccess, __Circle cc)
{	
	if(bSuccess)
	{
		m_InsResult.mRS.ran_circ.bFind = bSuccess;
		m_InsResult.mRS.ran_circ.radius = cc.radius;
		m_InsResult.mRS.ran_circ.center.x = cc.center.x;
		m_InsResult.mRS.ran_circ.center.y = cc.center.y;
		m_InsResult.mRS.ran_circ.tact = cc.tact;
	}
	else
		InitRANSACParam(0);

	if(m_nRectFindMode == RECTANGLE_EVIEION)
	{
		if(m_InsResult.mMC.bFind && m_InsResult.mRS.ran_circ.bFind)
		{
			m_InsResult.dist = Distance(m_InsResult.mRS.ran_circ.center, cv::Point(m_InsResult.mMC.mMCpos.m_f32CenterX, m_InsResult.mMC.mMCpos.m_f32CenterY));
		}
		else
			m_InsResult.dist = -1;
	}
	else
	{
		if(m_InsResult.mRS.ran_rect.bFind && m_InsResult.mRS.ran_circ.bFind)
		{
			m_InsResult.dist = Distance(m_InsResult.mRS.ran_circ.center, m_InsResult.mRS.ran_rect.center);
		}
		else
			m_InsResult.dist = -1;
	}
	
}

void CImageSimulationDlg::SetRSRResult(BOOL bSuccess, __Rectangle rt)
{	
	if(bSuccess)
	{
		m_InsResult.mRS.ran_rect.bFind = bSuccess;
		m_InsResult.mRS.ran_rect.angle = rt.angle;
		m_InsResult.mRS.ran_rect.center = rt.center;
		m_InsResult.mRS.ran_rect.vertex[0] = rt.vertex[0];
		m_InsResult.mRS.ran_rect.vertex[1] = rt.vertex[1];
		m_InsResult.mRS.ran_rect.vertex[2] = rt.vertex[2];
		m_InsResult.mRS.ran_rect.vertex[3] = rt.vertex[3];		
		m_InsResult.mRS.ran_rect.tact = rt.tact;
	}
	else
		InitRANSACParam(0);

	if(m_InsResult.mMC.bFind && m_InsResult.mRS.ran_circ.bFind)
	{
		m_InsResult.dist = Distance(m_InsResult.mRS.ran_circ.center, cv::Point(m_InsResult.mMC.mMCpos.m_f32CenterX, m_InsResult.mMC.mMCpos.m_f32CenterY));
	}
}

void CImageSimulationDlg::MakeDir(CString path)
{
	int pos1=0,pos=0,len;
	CString str;
	if(path.Right(1) != _T('\\'))
	{
		path +='\\';
	}
	pos1=path.Find("\\\\");
	len = path.GetLength ();
	if(pos1!=-1) //nhs \\\\192.168.0.1 구분하기 위함..
		pos=path.Find('\\',pos+2);
	else
		pos = path.Find ('\\');
	do{
		pos = path.Find ('\\',pos+1);
		str = path.Left(pos);
		::CreateDirectory (str,NULL);
	}while(len!=str.GetLength ()+1);
}

void CImageSimulationDlg::InitResult(__MatchResult& mr)
{
	mr.mRS.ran_circ.bFind = FALSE;
	mr.mRS.ran_circ.radius = 0.;
	mr.mRS.ran_circ.center = cv::Point(0,0);
	mr.mRS.ran_circ.tact = 0.;

	mr.mRS.ran_rect.bFind = FALSE;
	mr.mRS.ran_rect.angle = 0.;
	mr.mRS.ran_rect.vertex[0] = cv::Point(0,0);
	mr.mRS.ran_rect.vertex[1] = cv::Point(0,0);
	mr.mRS.ran_rect.vertex[2] = cv::Point(0,0);
	mr.mRS.ran_rect.vertex[3] = cv::Point(0,0);
	mr.mRS.ran_rect.center = cv::Point(0,0);
	mr.mRS.ran_rect.tact = 0.;

	mr.mMC.bFind = FALSE;
	mr.mMC.mMCpos.m_bInterpolated = FALSE;
	mr.mMC.mMCpos.m_f32Angle = 0.;
	mr.mMC.mMCpos.m_f32AreaRatio = 0.;
	mr.mMC.mMCpos.m_f32CenterX = 0.;
	mr.mMC.mMCpos.m_f32CenterY = 0.;
	mr.mMC.mMCpos.m_f32Reserved = 0.;
	mr.mMC.mMCpos.m_f32Scale = 0.;
	mr.mMC.mMCpos.m_f32ScaleX = 0.;
	mr.mMC.mMCpos.m_f32ScaleY = 0.;
	mr.mMC.mMCpos.m_f32Score = 0.;
}

void CImageSimulationDlg::InitRANSACParam(bool bType)
{
	if(bType == 0)
	{
		m_InsResult.mRS.ran_circ.bFind = FALSE;
		m_InsResult.mRS.ran_circ.radius = 0.;
		m_InsResult.mRS.ran_circ.center = cv::Point(0,0);
		m_InsResult.mRS.ran_circ.tact = 0.;
	}
	else
	{
		m_InsResult.mRS.ran_rect.bFind = FALSE;
		m_InsResult.mRS.ran_rect.angle = 0.;
		m_InsResult.mRS.ran_rect.vertex[0] = cv::Point(0,0);
		m_InsResult.mRS.ran_rect.vertex[1] = cv::Point(0,0);
		m_InsResult.mRS.ran_rect.vertex[2] = cv::Point(0,0);
		m_InsResult.mRS.ran_rect.vertex[3] = cv::Point(0,0);
		m_InsResult.mRS.ran_rect.center = cv::Point(0,0);
		m_InsResult.mRS.ran_rect.tact = 0.;
	}
}
void CImageSimulationDlg::InitEMATCHParam()
{
	m_InsResult.mMC.bFind = FALSE;
	m_InsResult.mMC.mMCpos.m_bInterpolated = FALSE;
	m_InsResult.mMC.mMCpos.m_f32Angle = 0.;
	m_InsResult.mMC.mMCpos.m_f32AreaRatio = 0.;
	m_InsResult.mMC.mMCpos.m_f32CenterX = 0.;
	m_InsResult.mMC.mMCpos.m_f32CenterY = 0.;
	m_InsResult.mMC.mMCpos.m_f32Reserved = 0.;
	m_InsResult.mMC.mMCpos.m_f32Scale = 0.;
	m_InsResult.mMC.mMCpos.m_f32ScaleX = 0.;
	m_InsResult.mMC.mMCpos.m_f32ScaleY = 0.;
	m_InsResult.mMC.mMCpos.m_f32Score = 0.;
}

void CImageSimulationDlg::ShowResult()
{

}

double CImageSimulationDlg::Distance(cv::Point p1, cv::Point p2)
{
	double distance;
	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	distance = sqrt(pow(abs((double)p1.x - (double)p2.x), 2) + pow(abs((double)p1.y - (double)p2.y), 2));
	return distance;
}

void CImageSimulationDlg::OnBnClickedCheckShowRoi2()
{
	UpdateData(TRUE);
}


void CImageSimulationDlg::VersionCheck()
{
	m_beVisionLisence = ECheckLicense(E_EVISION_LICENSE_EASYMATCH);

	if(m_beVisionLisence == TRUE)
		m_sEVisionLicense.Format("유");
	else
		m_sEVisionLicense.Format("무");

	if(pDlg_EP->m_bLearnComplete)
		m_sPreLearnMode.Format("유");
	else
		m_sPreLearnMode.Format("진행필요");

	m_sOpenCVVersion.Format("%s", CV_VERSION);

	UpdateData(FALSE);
}

void CImageSimulationDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_REFRESH:
//		VersionCheck();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CImageSimulationDlg::OnBnClickedCheckFolderInspect()
{
	UpdateData(TRUE);
}


void CImageSimulationDlg::OnBnClickedButtonInspect()
{
	UpdateData(TRUE);// TODO: Add your control notification handler code here
	if(m_bFolderInspect)
	{
		// 초기 선택 폴더
		CString strInitPath = _T("D:\\User\\3. Image\\2. Image File\\0. 시료 높이 측정\\41개 시료 테스트\\");
		// 폴더 선택 다이얼로그
		CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);

		if (Picker.DoModal() == IDOK)
		{
			// 선택된 폴더 경로얻음
			CString strFolderPath = Picker.GetPathName();
			CString filePath, ff_name, ff_path, ff_sPath, ff_sName, temp;
			BOOL ff_flag = FALSE;
			
			CFileFind ff;
			filePath.Format("%s\\*.bmp", strFolderPath);
			ff_flag = ff.FindFile(filePath);
			
			while(ff_flag)
			{
				ff_flag = ff.FindNextFile();
				if(!ff.IsDirectory() && !ff.IsDots())
				{
					ff_name = ff.GetFileName();
					ff_path = ff.GetFilePath();
					temp.Format(ff_path.Mid(0, ff_path.ReverseFind('\\')));
					ff_sPath.Format("%s\\Result\\", temp);
					ff_sName.Format("%s%s", ff_sPath, ff_name);
					MakeDir(ff_sPath);
					
					if(Inspection(ff_path))
						SaveResultImage(m_mat, m_eMatch, ff_sName);
//					Mat input, gray;
//
//					input = imread(string(CT2CA(ff_path)), CV_LOAD_IMAGE_GRAYSCALE);
//					cvtColor(input, gray, CV_BGR2GRAY);
//					imwrite(string(CT2CA(ff_sName)), gray);



				}
			}
			// 경로(strFolderPath)를 이용하여 이후작업 추가
		}
	}
	else
	{
		if(m_strPathName == "")
		{
			AfxMessageBox("대상 이미지가 Laod 되지 않았습니다.");
			return;
		}
		if(!pDlg_EP->m_bLearnComplete)
		{
			AfxMessageBox("Pattern Matching Recipe를 통해 Learn을 진행해주십시오");
			return;
		}

		if(Inspection(m_strPathName))
		{
			CString temp, ff_sPath, ff_sName, ff_name;

			temp.Format(m_strPathName.Mid(0, m_strPathName.ReverseFind('\\')));
			ff_name.Format(m_strPathName.Mid(m_strPathName.ReverseFind('\\') + 1, m_strPathName.GetLength()));
			ff_sPath.Format("%s\\Result\\", temp);
			ff_sName.Format("%s%s", ff_sPath, ff_name);
			MakeDir(ff_sPath);

			SaveResultImage(m_mat, m_eMatch, ff_sName);
		}		
	}

	DrawImage(m_mat);
}

bool CImageSimulationDlg::HDCToFile(const char* FilePath, HDC Context, RECT Area, int BitsPerPixel)
{
	int Width = Area.right - Area.left;
	int Height = Area.bottom - Area.top;

	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));
	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = Width;
	Info.bmiHeader.biHeight = Height;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = BitsPerPixel;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	char* Pixels = NULL;
	HDC MemDC = CreateCompatibleDC(Context);
	HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, (void**)&Pixels, 0, 0);
	DeleteObject(SelectObject(MemDC, Section));
	BitBlt(MemDC, 0, 0, Width, Height, Context, Area.left, Area.top, SRCCOPY);
	DeleteDC(MemDC);

	std::fstream hFile(FilePath, std::ios::out | std::ios::binary);
	if (hFile.is_open())
	{
		hFile.write((char*)&Header, sizeof(Header));
		hFile.write((char*)&Info.bmiHeader, sizeof(Info.bmiHeader));
		hFile.write(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
		hFile.close();
		DeleteObject(Section);
		return true;
	}

	DeleteObject(Section);
	return false;
}

void CImageSimulationDlg::SaveBitmapToDirectFile(CDC* pDC, CRect BitmapSize, int BitCount, CString strFilePath)
{
	CBitmap bmp, *pOldBmp;
	CDC dcMem;  
	BITMAP                  bm;
	BITMAPINFOHEADER        bi;
	LPBITMAPINFOHEADER      lpbi;
	DWORD                   dwLen;
	HANDLE                  handle;
	HANDLE                  hDIB;   
	HPALETTE                hPal=NULL;
	/*----- CDC의 내용을 Bitmap으로 전송 ----*/
	dcMem.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC ,BitmapSize.Width(),BitmapSize.Height());    
	pOldBmp = (CBitmap*) dcMem.SelectObject(&bmp);
	dcMem.BitBlt( 0,0, BitmapSize.Width(), BitmapSize.Height(), pDC, 0,0, SRCCOPY); 
	dcMem.SelectObject(pOldBmp);
	if (strFilePath == "")          return;
	/*------------------------- 비트멥 헤더를 기록함 -------------------------*/
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
	GetObject(HBITMAP(bmp), sizeof(BITMAP), &bm);
	bi.biSize               = sizeof(BITMAPINFOHEADER);
	bi.biWidth              = bm.bmWidth;
	bi.biHeight             = bm.bmHeight;
	bi.biPlanes             = 1;
	bi.biBitCount           = 32;       
	bi.biCompression        = BI_RGB;
	bi.biSizeImage          = bm.bmWidth * bm.bmHeight * 3;
	bi.biXPelsPerMeter      = 0;
	bi.biYPelsPerMeter      = 0;
	bi.biClrUsed            = 0;
	bi.biClrImportant       = 0;
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);
	hPal = SelectPalette(pDC->GetSafeHdc(),hPal,FALSE);
	RealizePalette(pDC->GetSafeHdc());
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	*lpbi = bi;
	GetDIBits(pDC->GetSafeHdc(), 
		HBITMAP(bmp), 
		0, 
		(DWORD)bi.biHeight,
		(LPBYTE)NULL, 
		(LPBITMAPINFO)lpbi, 
		(DWORD)DIB_RGB_COLORS);
	bi = *lpbi;
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
			* bi.biHeight;
	}
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	GetDIBits(pDC->GetSafeHdc(), 
		HBITMAP(bmp),
		0,                    
		(DWORD)bi.biHeight,      
		(LPBYTE)lpbi        
		+ (bi.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi,   
		(DWORD)DIB_RGB_COLORS);
	BITMAPFILEHEADER      hdr;
	hdr.bfType        = ((WORD) ('M' << 8) | 'B');        
	hdr.bfSize        = GlobalSize (hDIB) + sizeof(hdr);   
	hdr.bfReserved1   = 0;                                 
	hdr.bfReserved2   = 0;                                 
	hdr.bfOffBits=(DWORD)(sizeof(hdr)+lpbi->biSize + nColors * sizeof(RGBQUAD));
	char* pBmpBuf; 
	DWORD FileSize; 
	FileSize=sizeof(hdr)+GlobalSize(hDIB);
	pBmpBuf = new char[FileSize];
	memcpy(pBmpBuf,&hdr,sizeof(hdr));
	memcpy(pBmpBuf+sizeof(hdr),lpbi,GlobalSize(hDIB));
	/*--------------------- 실제 파일에 기록함 --------------------------*/
	CFile file;
	file.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	file.Write(pBmpBuf,FileSize);
	file.Close();
	/*------------------------ 임시로 할당한 메모리를 해제.. -------------*/
	delete[] pBmpBuf;
	if(hDIB) 
	{       
		GlobalFree(hDIB);
	}
	SelectPalette(pDC->GetSafeHdc(),hPal,FALSE);  
}

cv::Point CImageSimulationDlg::RotatePosition(cv::Point rpos, double angle)
{
	cv::Point _rPos = cv::Point(0,0);

	_rPos.x = rpos.x * cos(angle) + rpos.y * sin(angle);
	_rPos.y = rpos.y * cos(angle) - rpos.x * sin(angle);
	return _rPos;
}

void CImageSimulationDlg::SaveResultImage(Mat mat, EMatch match, CString path)
{

	/*
	Mat save;
	if(save.channels() == 1)
		cvtColor(mat, save, CV_GRAY2BGR);
	else
		save = mat.clone();

	cv::Point s[4], temp;
	double angle = match.GetPosition(0)->m_f32Angle * PI / 180 * -1;
	double angle2 = match.GetPosition(0)->m_f32Angle;
	Mat sMat;
	double center_x, center_y;
	int test ;
	test = m_eMatch.GetPattern(0)->GetOrgX();
	center_x = match.GetPosition(0)->m_f32CenterX;// -  m_n32TrfXMin;
	center_y = match.GetPosition(0)->m_f32CenterY;
	
	double width, height;
	width = m_eMatch.GetPattern(0)->GetWidth();
	height = m_eMatch.GetPattern(0)->GetHeight();
	
	sMat = Mat::zeros(m_mat.rows, m_mat.cols, CV_8SC3);
	cvtColor(m_mat, sMat, CV_GRAY2BGR);
	temp.x = center_x - width/2;
	temp.y = center_y - height/2;
	s[0] = RotatePosition(temp, angle);

	temp.x = match.GetPosition(0)->m_f32CenterX + m_eMatch.GetPattern(0)->GetWidth()/2;
	temp.y = match.GetPosition(0)->m_f32CenterY - m_eMatch.GetPattern(0)->GetHeight()/2;
	s[1] = RotatePosition(temp, angle);

	temp.x = match.GetPosition(0)->m_f32CenterX + m_eMatch.GetPattern(0)->GetWidth()/2;
	temp.y = match.GetPosition(0)->m_f32CenterY + m_eMatch.GetPattern(0)->GetHeight()/2;
	s[2] = RotatePosition(temp, angle);

	temp.x = match.GetPosition(0)->m_f32CenterX - m_eMatch.GetPattern(0)->GetWidth()/2;
	temp.y = match.GetPosition(0)->m_f32CenterY + m_eMatch.GetPattern(0)->GetHeight()/2;
	s[3] = RotatePosition(temp, angle);

	line(save, s[0], s[1], Scalar(0, 255, 255), 2,8,0);
	line(save, s[1], s[2], Scalar(0, 255, 255), 2,8,0);
	line(save, s[2], s[3], Scalar(0, 255, 255), 2,8,0);
	line(save, s[3], s[0], Scalar(0, 255, 255), 2,8,0);

	imwrite(string(CT2CA(path)), save);

	*/
	Mat save;
	if(save.channels() == 1)
		cvtColor(mat, save, CV_GRAY2BGR);
	else
		save = mat.clone();

	ShowFinalResult(save, m_nPyrRS, m_rRect, m_InsResult);

	IplImage* bgr_frame;
	IplImage copy;
	copy = save;
	bgr_frame = &copy;

	CvvImage vImg;
	vImg.CopyOf(bgr_frame, 1);
	
	RECT rect2;
	rect2.left = 0;
	rect2.top = 0;
	rect2.right = mat.cols;
	rect2.bottom = mat.rows;

	HDC hdc,hMemDC; 
	HBITMAP MyBitmap; 

	hdc=CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL); 
	hMemDC=CreateCompatibleDC(hdc); 
	MyBitmap=CreateCompatibleBitmap(hdc,rect2.right,rect2.bottom); 
	SelectObject(hMemDC,MyBitmap); 

	vImg.DrawToHDC(hMemDC, &rect2);

	if(m_nRectFindMode == RECTANGLE_EVIEION)
	{
		HPEN rPen2 = CreatePen(PS_DOT, 1, DPINK);
		HGDIOBJ rOldPen2 = (HPEN)SelectObject(hMemDC, rPen2);

		m_eMatch.DrawPositions(hMemDC, FALSE, 1, 1, 0, 0);
	}
	
	SaveBitmapToDirectFile(CDC::FromHandle(hMemDC), rect2, 24, path);	

	DeleteDC(hMemDC); 
	DeleteDC(hdc); 
	DeleteObject(MyBitmap); 
}

BOOL CImageSimulationDlg::Inspection(CString path)
{
	if(!m_mat.empty())
		m_mat.release();

	m_mat = imread(string(CT2CA(path)), CV_LOAD_IMAGE_GRAYSCALE);

	InitResult(m_InsResult);
	__RANSAC _r;

	BOOL bRtn = FALSE, bRtn2 = FALSE;

	if(m_nRectFindMode == RECTANGLE_EVIEION)
	{
		InitEMATCHParam();

		if(pDlg_EP->m_bLearnComplete)
		{
			LARGE_INTEGER timer, start, end;

			QueryPerformanceFrequency(&timer);
			QueryPerformanceCounter(&start);

			pDlg_EP->PreprocessLearn();
			pDlg_EP->eImageLoad(MATCH_PATRN_IMAGE, path);		

			m_eMatch = pDlg_EP->TempleteMatch(pDlg_EP->eSrcImage, pDlg_EP->ePatImage, pDlg_EP->eRoiImage, pDlg_EP->m_rROIRect, TRUE, FALSE, TRUE);
			QueryPerformanceCounter(&end);

			if(m_eMatch.GetNumPositions() > 0)
			{
				m_InsResult.mMC.tact = (end.QuadPart - start.QuadPart)/(float)(timer.QuadPart)*1000;
			}

			SetEMResult(m_eMatch);

			if(m_InsResult.mMC.bFind == TRUE)
				bRtn = TRUE;
		}
	}
	else
	{
		InitRANSACParam(1);
		
		_r = IP.RansacProcess(RANSAC_LINE, m_mat, m_rRect, TRUE, m_nGKernel, m_nRectTH, m_bImageDebug, m_nRangeInRect, m_fSamplingRatio, m_nPerInRect);
		SetRSRResult(_r.ran_rect.bFind, _r.ran_rect);

		if(_r.ran_rect.bFind == TRUE)
			bRtn = TRUE;

		if(!bRtn)
			AfxMessageBox("Rectangle Search Fail");
	}

	if(bRtn == TRUE || m_nRectFindMode == RECTANGLE_EVIEION)
	{
		InitRANSACParam(0);
		
		//bRtn = CircleFit(m_nPyrRS, m_mat, SetCircleRect(m_rRect), _r.ran_circ, TRUE, m_nGKernel, m_nTH, m_bImageDebug);
		_r = IP.RansacProcess(RANSAC_CIRCLE, m_mat, SetCircleRect(m_rRect), TRUE, m_nGKernel, m_nTH, m_bImageDebug, m_nRangeInRect, m_fSamplingRatio, m_nPerInRect);
		SetRSCResult(_r.ran_circ.bFind, _r.ran_circ);
		
		if(m_InsResult.mRS.ran_circ.bFind == TRUE)
			bRtn2 = TRUE;

		if(!bRtn2)
			AfxMessageBox("Circle Search Fail");

	}
	else
	{

	}
		
	return bRtn || bRtn2;
}

void CImageSimulationDlg::OnBnClickedRadioCircleFindMode()
{
	UpdateData(TRUE);
	DrawImage(m_mat);
}


void CImageSimulationDlg::OnBnClickedRadioCircleFindMode2()
{
	UpdateData(TRUE);
	DrawImage(m_mat);
}


void CImageSimulationDlg::OnBnClickedRadioCircleFindMode3()
{
	UpdateData(TRUE);
	DrawImage(m_mat);
}


void CImageSimulationDlg::OnBnClickedRadioCircleFindMode4()
{
	UpdateData(TRUE);
	DrawImage(m_mat);
}


void CImageSimulationDlg::OnBnClickedButtonCircleFind()
{
	if(m_mat.empty())
	{
		AfxMessageBox("이미지 로드를 먼저 진행해야 합니다.");
		return;
	}
	
	if(m_nRectFindMode == RECTANGLE_OPENCV && m_InsResult.mRS.ran_rect.bFind == FALSE)
	{
		AfxMessageBox("Opencv 사용 시 바운더리 사각형을 먼저 찾으십시오.");
		return;
	}

	InitRANSACParam(0);
	BOOL bRtn = FALSE;

	__Circle _c;
	bRtn = CircleFit(m_nPyrRS, m_mat, SetCircleRect(m_rRect), _c, m_nCircleFindMode, m_nGKernel, m_nTH, m_bImageDebug);
	SetRSCResult(bRtn, _c);

	if(!bRtn)		
		AfxMessageBox("Search Fail");

	DrawImage(m_mat);
}

Rect CImageSimulationDlg::SetCircleRect(Rect rect)
{
	Rect cRect;
	cRect.x = rect.x + rect.width / 2 - m_nCircleROI_w;
	cRect.y = rect.y + rect.height / 2 - m_nCircleROI_h;
	cRect.width = m_nCircleROI_w*2;
	cRect.height = m_nCircleROI_h*2;

	if(cRect.x < rect.x
		|| cRect.y < rect.y 
		|| cRect.x+cRect.width >  rect.x+rect.width
		|| cRect.y+cRect.height >  rect.y+rect.height)
		cRect = rect;

	return cRect;
}
