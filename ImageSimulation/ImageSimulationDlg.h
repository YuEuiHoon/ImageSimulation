
// ImageSimulationDlg.h : header file
//

#pragma once

//#include "ImageProcess.h"

class ImageProcess;

// CImageSimulationDlg dialog
class CImageSimulationDlg : public CDialog
{
// Construction
public:
	CImageSimulationDlg(CWnd* pParent = NULL);	// standard constructor

	ImageProcess IP;
	// UI ����
	CStatic			m_pImageView;
	CString			m_strPathName;
	Rect			m_rRect;
	CvvImage		vImage;
	BITMAPINFO		*m_pBitmapInfo; // Bitmap ������ ��� �ִ� ����ü.

	// UI �� �Լ�
	void			CreateBitmapInfo(int w, int h, int bpp); // Bitmap ������ �����ϴ� �Լ�.
	void			DrawImage(Mat mat);
	void			DrawImageInformation(Mat mat, CString path);
	void			ShowResultImage(string sw, Mat mat);
	// ó�� �ε��� ���� �̹��� 
	Mat				m_mat;
	// �̹��� ����� Ŭ ��� ����� ����� �̹���
	Mat				m_tMat;

	Mat				m_rMat;

	Mat				ImageResize(Mat mat, int rs = 0, int dn = 0);
	double			change_PrymidValue(int rsize, double val);
	Rect			rect_normalize(int rsize, Rect rect);
	// System.ini ���Ϸ� �Ķ���� ���� �� �ε� 
	void			InitParam();
	void			SaveData(CString filename);
	void			LoadData(CString filename);

	// Function
	BOOL			CircleFit(int rsize, Mat src, Rect roi, __Circle& rc, BOOL bBin = TRUE, int gK = 15, int nTH = 30, BOOL bImgDebug = FALSE);	
	void			InspectResult(Mat mat, int rsize, Rect roi, __Circle c);

	// ROI Setting 
	void			MapMessage(MSG* pMsg);
	CRect			m_rCtrlWndRect;
	CRect			m_rCtrlClientRect;
	BOOL			m_bLDown;
	cv::Point		m_pDrawStart;
	cv::Point		m_pDrawEnd;

	void			MakeDir(CString path);
	EMatch			m_eMatch;

	void			InitResult(__MatchResult& mr);
	void			InitRANSACParam(bool bType);
	void			InitEMATCHParam();

	__MatchResult	m_InsResult;
	void			SetEMResult(EMatch mch);
	void			SetRSCResult(BOOL bSuccess, __Circle cc);
	void			SetRSRResult(BOOL bSuccess, __Rectangle rt);
	void			ShowResult();

	void			ShowFinalResult(Mat mat, int rsize, Rect roi, __MatchResult mr);
	double			Distance(cv::Point p1, cv::Point p2);
	BOOL			m_beVisionLisence;	
	void			VersionCheck();
	bool			HDCToFile(const char* FilePath, HDC Context, RECT Area, int BitsPerPixel = 24);
	void			SaveBitmapToDirectFile(CDC* pDC, CRect BitmapSize, int BitCount, CString strFilePath);

	cv::Point		RotatePosition(cv::Point rpos, double angle);
	void			SaveResultImage(Mat mat, EMatch match, CString path);
	BOOL			Inspection(CString path);
	Rect			SetCircleRect(Rect rect);

// Dialog Data
	enum { IDD = IDD_IMAGESIMULATION_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	int m_nPyrRS;
	int m_nPyrDN;
	afx_msg void OnBnClickedButton3();
	int m_nGKernel;
	int m_nTH;
	int m_nROI_x;
	int m_nROI_y;
	int m_nROI_width;
	int m_nROI_height;
	afx_msg void OnBnClickedCheckShowRoi();
	BOOL m_bShowROI;
	afx_msg void OnBnClickedButtonPreProcess();
	afx_msg void OnBnClickedButtonRansacUseBinary();
	afx_msg void OnBnClickedButtonRansacUseNomalImage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_bROISet;
	afx_msg void OnBnClickedCheckSetRoi();	
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonPatternMatching();
	BOOL m_bImageDebug;
	afx_msg void OnBnClickedCheckShowRoi2();
	CString m_sEVisionLicense;
	CString m_sPreLearnMode;
	CString m_sOpenCVVersion;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_bFolderInspect;
	afx_msg void OnBnClickedCheckFolderInspect();
	afx_msg void OnBnClickedButtonInspect();
	double m_dbFov;
	float m_fSamplingRatio;
	float m_fMinCircleRadius;
	int m_nCircleROI_w;
	int m_nCircleROI_h;
	int m_nPerInRect;
	int m_nRangeInRect;
	int m_nCircleFindMode;
	int m_nRectFindMode;
	afx_msg void OnBnClickedRadioCircleFindMode();
	afx_msg void OnBnClickedRadioCircleFindMode2();
	afx_msg void OnBnClickedRadioCircleFindMode3();
	afx_msg void OnBnClickedRadioCircleFindMode4();
	afx_msg void OnBnClickedButtonCircleFind();
	int m_nRectTH;
};

extern CImageSimulationDlg g_Dlg_Main;