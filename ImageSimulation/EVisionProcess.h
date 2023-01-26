
#pragma once
#include "afxwin.h"

// EVisionProcess dialog
#define SCALE_BOTH				0
#define SCALE_EACH				1

#define MATCH_LEARN_IMAGE		0
#define MATCH_PATRN_IMAGE		1

<<<<<<< HEAD
#define MODE_RELATIVE			0
#define MODE_ABSOLUTE			1
=======
<<<<<<< HEAD
<<<<<<< HEAD
#define MODE_RELATIVE			0
#define MODE_ABSOLUTE			1
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c

class EVisionProcess : public CDialog
{
	DECLARE_DYNAMIC(EVisionProcess)

public:
	EVisionProcess(CWnd* pParent = NULL);   // standard constructor
	virtual ~EVisionProcess();
	
	void					InitParam();
	void					UpLoadData();
	CString					simgPath;

	// UI 갱신
	CFont					cFont[2];
	HBITMAP					hBitmap;
	CBitmap*				m_pBitmap;
	CBitmap*				m_pOldBitmap;

	CDC						m_dcMap;
	CClientDC*				m_pMapDC;

	CDC						m_dcLMap;
	CClientDC*				m_pLMapDC;

	void					SetTextOut(HDC hDC, int x, int y, BOOL mode, CString csText);

	BOOL					bEPEnable;
 	enum E_HANDLES			eHandle;
 	enum INS_SHAPE_TYPES	eGaugeType;

	EImageBW8				eSrcImage;		// EImageBW8 instance
	EImageBW8				ePatImage;		// EImageBW8 instance
	EMatchPosition			*pPosition;			// 매칭 포지션
	EROIBW8					eRoiImage;			// ROI 리스트	
	EMatch					EMatch1;

	float					fscaleX;
	float					fscaleY;
	int						nImageSizeX;
	int						nImageSizeY;

	void					SetZoom();
	CRect					nRectView;
	CRect					nRectEPLView;

	CString					m_sPath;
	CString					m_sPatPath;
	void					DrawImage(BOOL bMmode);
	void					DrawEPImage(EImageBW8 EImage);
	void					DrawEPLearnImage(EROIBW8 eRoi);
	void					DrawImageInformation(int mode, CString path);

	// ROI Setting 
	void					MapMessage(MSG* pMsg);
	CRect					m_rCtrlWndRect;
	CRect					m_rCtrlClientRect;
	BOOL					m_bLDown;
	cv::Point				m_pDrawStart;
	cv::Point				m_pDrawEnd;

	CRect					roi_Normalize(CRect rect);
	void					SetScaleMode(int mode);

	void					EPSetContrastMode(int mode, EMatch& match);
	void					EPSetCorrelationMode(int mode, EMatch& match);

	BOOL					m_bLearnComplete;
	void					ShowResult(BOOL bSucess);
	int						m_nMatchNums;
	void					eImageLoad(BOOL bMode, CString path);
	void					LearnImage(EImageBW8& eSrc, EImageBW8& ePat, EROIBW8& eRoi, Rect roi, BOOL bRoiUse);
	EMatch					TempleteMatch(EImageBW8& eSrc, EImageBW8& ePat, EROIBW8& eRoi, Rect roi, BOOL bRoiUse, BOOL bLearn, BOOL bmatch);
	Rect					m_rROIRect;
	Rect					m_rP_ROIRect;

	int						m_nCBContrast;
	int						m_nCBCorrMode;
	void					PreprocessLearn();
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	bool					ConvertChannel(CString path, EImageBW8 &eImg);
	bool					MatToEImage(Mat src, EImageBW8 &dst);
	double					m_dbAngle;
	void					SetAngle(double angle);
	double					GetAngle()	{return m_dbAngle;}
	void					SetRotate(BOOL bMode);
	Mat						m_mLoadImage;
<<<<<<< HEAD
=======
=======

>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======

>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	// Dialog Data
	enum { IDD = IDD_EVISION_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	int m_nP_ROI_y;
	int m_nP_ROI_x;
	int m_nP_ROI_width;
	int m_nP_ROI_height;
	BOOL m_bShowP_ROI;
	BOOL m_bP_ROISet;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheckShowPRoi();
	afx_msg void OnBnClickedCheckSetPRoi();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton3();
	CComboBox m_cmContrast;
	CComboBox m_cmCorrMode;
	float m_fPAngleMax;
	float m_fPAngleMin;
//	float m_fPScaleBothX;
	float m_fPScaleBothMax;
	float m_fPScaleBothMin;
	float m_fPScaleEachMax;
	float m_fPScaleEachMin;
	BOOL m_bPScalMode;
	afx_msg void OnBnClickedRadioScaleMode();
	afx_msg void OnBnClickedRadioScaleMode2();
	afx_msg void OnBnClickedButtonExecute();
	afx_msg void OnBnClickedButton4();
	BOOL m_bMatchMode;
	afx_msg void OnBnClickedRadioMatchMode();
	afx_msg void OnBnClickedRadioMatchMode2();
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	afx_msg void OnBnClickedButtonRotateCw();
	afx_msg void OnBnClickedButtonRotateCcw();
	double m_dbRtAngle;
	afx_msg void OnBnClickedButtonRotate();
	afx_msg void OnBnClickedButton5();
	int m_nRSTh;
<<<<<<< HEAD
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
};

extern EVisionProcess *pDlg_EP;