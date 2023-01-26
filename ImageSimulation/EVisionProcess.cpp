// EVisionProcess.cpp : implementation file
//

#include "stdafx.h"
#include "ImageSimulation.h"

#include "afxdialogex.h"
#include "EVisionProcess.h"

<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c

#include <time.h>
#include <math.h>

<<<<<<< HEAD
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
EVisionProcess *pDlg_EP;
// EVisionProcess dialog

IMPLEMENT_DYNAMIC(EVisionProcess, CDialog)

EVisionProcess::EVisionProcess(CWnd* pParent /*=NULL*/)
	: CDialog(EVisionProcess::IDD, pParent)
	, m_bMatchMode(FALSE)
<<<<<<< HEAD
	, m_dbRtAngle(0)
	, m_nRSTh(50)
=======
<<<<<<< HEAD
<<<<<<< HEAD
	, m_dbRtAngle(0)
	, m_nRSTh(50)
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
{
	m_nP_ROI_y = 11;
	m_nP_ROI_x = 22;
	m_nP_ROI_width = 33;
	m_nP_ROI_height = 44;
	m_bShowP_ROI = FALSE;
	m_bP_ROISet = FALSE;
	m_fPAngleMax = 0.0f;
	m_fPAngleMin = 0.0f;
	//  m_fPScaleBothX = 0.0f;
	m_fPScaleBothMax = 100.0f;
	m_fPScaleBothMin = 100.0f;
	m_fPScaleEachMax = 100.0f;
	m_fPScaleEachMin = 100.0f;
	m_bPScalMode = FALSE;

	m_bLearnComplete = FALSE;
	m_nMatchNums = 0;
}

EVisionProcess::~EVisionProcess()
{
}

void EVisionProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_P_ROI_Y, m_nP_ROI_y);
	DDX_Text(pDX, IDC_EDIT_P_ROI_X, m_nP_ROI_x);
	DDX_Text(pDX, IDC_EDIT_P_ROI_WIDTH, m_nP_ROI_width);
	DDX_Text(pDX, IDC_EDIT_P_ROI_HEIGHT, m_nP_ROI_height);
	DDX_Check(pDX, IDC_CHECK_SHOW_P_ROI, m_bShowP_ROI);
	DDX_Check(pDX, IDC_CHECK_SET_P_ROI, m_bP_ROISet);
	DDX_Control(pDX, IDC_COMBO_CONTRAST, m_cmContrast);
	DDX_Control(pDX, IDC_COMBO_CORR_MODE, m_cmCorrMode);
	DDX_Text(pDX, IDC_EDIT_P_ANGLE_MAX, m_fPAngleMax);
	DDX_Text(pDX, IDC_EDIT_P_ANGLE_MIN, m_fPAngleMin);
	//  DDX_Text(pDX, IDC_EDIT_P_SCALE_BOTH_MAX, m_fPScaleBothX);
	DDX_Text(pDX, IDC_EDIT_P_SCALE_BOTH_MAX, m_fPScaleBothMax);
	DDX_Text(pDX, IDC_EDIT_P_SCALE_BOTH_MIN, m_fPScaleBothMin);
	DDX_Text(pDX, IDC_EDIT_P_SCALE_EACH_MAX, m_fPScaleEachMax);
	DDX_Text(pDX, IDC_EDIT_P_SCALE_EACH_MIN, m_fPScaleEachMin);
	DDX_Radio(pDX, IDC_RADIO_SCALE_MODE, m_bPScalMode);
	DDX_Radio(pDX, IDC_RADIO_MATCH_MODE, m_bMatchMode);
<<<<<<< HEAD
	DDX_Text(pDX, IDC_EDIT_ROTATE_ANGLE, m_dbRtAngle);
	DDX_Text(pDX, IDC_EDIT_RANSAC_TH, m_nRSTh);
=======
<<<<<<< HEAD
<<<<<<< HEAD
	DDX_Text(pDX, IDC_EDIT_ROTATE_ANGLE, m_dbRtAngle);
	DDX_Text(pDX, IDC_EDIT_RANSAC_TH, m_nRSTh);
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
}


BEGIN_MESSAGE_MAP(EVisionProcess, CDialog)
	
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &EVisionProcess::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &EVisionProcess::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK_SHOW_P_ROI, &EVisionProcess::OnBnClickedCheckShowPRoi)
	ON_BN_CLICKED(IDC_CHECK_SET_P_ROI, &EVisionProcess::OnBnClickedCheckSetPRoi)
	ON_BN_CLICKED(IDC_BUTTON3, &EVisionProcess::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO_SCALE_MODE, &EVisionProcess::OnBnClickedRadioScaleMode)
	ON_BN_CLICKED(IDC_RADIO_SCALE_MODE2, &EVisionProcess::OnBnClickedRadioScaleMode2)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &EVisionProcess::OnBnClickedButtonExecute)
	ON_BN_CLICKED(IDC_BUTTON4, &EVisionProcess::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_RADIO_MATCH_MODE, &EVisionProcess::OnBnClickedRadioMatchMode)
	ON_BN_CLICKED(IDC_RADIO_MATCH_MODE2, &EVisionProcess::OnBnClickedRadioMatchMode2)
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_CW, &EVisionProcess::OnBnClickedButtonRotateCw)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_CCW, &EVisionProcess::OnBnClickedButtonRotateCcw)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &EVisionProcess::OnBnClickedButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON5, &EVisionProcess::OnBnClickedButton5)
<<<<<<< HEAD
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
END_MESSAGE_MAP()


// EVisionProcess message handlers

BOOL EVisionProcess::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void EVisionProcess::SetZoom()
{
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	if(m_bMatchMode == MATCH_LEARN_IMAGE)
	{
		nImageSizeX = eSrcImage.GetWidth();				// 가로
		nImageSizeY = eSrcImage.GetHeight();			// 세로
	}
	else
	{
		nImageSizeX = ePatImage.GetWidth();				// 가로
		nImageSizeY = ePatImage.GetHeight();			// 세로
	}

	fscaleX = (float)nRectView.Width() / (float)nImageSizeX; 
	fscaleY = (float)nRectView.Height() / (float)nImageSizeY;
	  
<<<<<<< HEAD
=======
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
	nImageSizeX = eSrcImage.GetWidth();				// 가로
	nImageSizeY = eSrcImage.GetHeight();			// 세로

	fscaleX = (float)nRectView.Width() / (float)nImageSizeX; 
	fscaleY = (float)nRectView.Height() / (float)nImageSizeY;

<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	DrawEPImage(eSrcImage);
}

void EVisionProcess::OnPaint()
{
	//DrawImage(eSrcImage);
	CPaintDC dc(this); // device context for painting

	DrawImage(m_bMatchMode);
}

void EVisionProcess::InitParam()
{
	bEPEnable = FALSE;
	simgPath.Format("");
	m_pMapDC = new CClientDC(GetDlgItem(IDC_IMAGE_EP_VIEW));

	GetDlgItem(IDC_IMAGE_EP_VIEW)->GetClientRect(nRectView);

	if(m_dcMap.GetSafeHdc() == NULL)
	{
		m_dcMap.CreateCompatibleDC(m_pMapDC);
		if((m_pBitmap = new CBitmap()) == NULL) return;
		m_pBitmap->CreateCompatibleBitmap(m_pMapDC,nRectView.right, nRectView.bottom);
		m_pOldBitmap = m_dcMap.SelectObject(m_pBitmap);
		m_dcMap.SelectStockObject(ANSI_VAR_FONT);
	}

	m_pLMapDC = new CClientDC(GetDlgItem(IDC_IMAGE_EP_LEARN_VIEW));

	GetDlgItem(IDC_IMAGE_EP_LEARN_VIEW)->GetClientRect(nRectEPLView);

	if(m_dcLMap.GetSafeHdc() == NULL)
	{
		m_dcLMap.CreateCompatibleDC(m_pLMapDC);
		if((m_pBitmap = new CBitmap()) == NULL) return;
		m_pBitmap->CreateCompatibleBitmap(m_pLMapDC,nRectEPLView.right, nRectEPLView.bottom);
		m_pOldBitmap = m_dcLMap.SelectObject(m_pBitmap);
		m_dcLMap.SelectStockObject(ANSI_VAR_FONT);
	}

	m_cmContrast.AddString("Normal");
	m_cmContrast.AddString("Inverse");
	m_cmContrast.AddString("Any");
	m_cmContrast.SetCurSel(0);

	m_cmCorrMode.AddString("Standard");
	m_cmCorrMode.AddString("Offset normalized");
	m_cmCorrMode.AddString("Gain normalized");
	m_cmCorrMode.AddString("Normalized");
	m_cmCorrMode.SetCurSel(0);	

	m_nMatchNums = 0;
	m_nCBContrast = 0;
	m_nCBCorrMode = 0;
<<<<<<< HEAD

	m_dbAngle = 0.;
	//SetRotate(MODE_ABSOLUTE);
=======
<<<<<<< HEAD
<<<<<<< HEAD

	m_dbAngle = 0.;
	//SetRotate(MODE_ABSOLUTE);
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
}


void EVisionProcess::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}


void EVisionProcess::OnDestroy()
{
	CDialog::OnDestroy();

	bEPEnable = FALSE;
	// TODO: Add your message handler code here
}

void EVisionProcess::OnBnClickedButton1()
{
	if(m_bMatchMode == MATCH_PATRN_IMAGE)
	{
		if(eSrcImage.GetVoid())
		{
			AfxMessageBox("Learn Image를 로드하여 Learn을 우선 진행하십시오");
			return;
		}
	}

	CString str = _T("Image files(*.jpg, *.bmp, *png)|*.jpg; *.bmp; *png|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	if (dlg.DoModal() == IDOK)
		eImageLoad(m_bMatchMode, dlg.GetPathName());
}

void EVisionProcess::DrawImage(BOOL bMmode)
{
	if(bMmode == MATCH_LEARN_IMAGE)
	{
		if(bEPEnable && !eSrcImage.GetVoid())
		{
			DrawEPImage(eSrcImage);		
		}
	}
	else
	{
		if(bEPEnable && !ePatImage.GetVoid())
		{
			DrawEPImage(ePatImage);
		}
	}

	if(m_bLearnComplete)
		DrawEPLearnImage(eRoiImage);
}

void EVisionProcess::DrawEPImage(EImageBW8 EImage)
{
	if(bEPEnable && !EImage.GetVoid())
	{
		m_dcMap.FillSolidRect(nRectView, RGB(0,0,0));
		
		EImage.Draw(m_dcMap.m_hDC, fscaleX, fscaleY);

		if(m_bShowP_ROI)
		{
			EROIBW8 eROI;
			eROI.Detach();
			eROI.SetSize(nImageSizeX,nImageSizeY);
			eROI.Attach(&EImage);

			CRect rect_Roi, rTemp;
			rTemp.left = m_nP_ROI_x;
			rTemp.right = m_nP_ROI_x + m_nP_ROI_width;
			rTemp.top = m_nP_ROI_y;
			rTemp.bottom = m_nP_ROI_y + m_nP_ROI_height;

			rect_Roi = roi_Normalize(rTemp);
			m_nP_ROI_x = m_rP_ROIRect.x = rect_Roi.left;
			m_nP_ROI_width = m_rP_ROIRect.width = rect_Roi.right - m_nP_ROI_x;
			m_nP_ROI_y = m_rP_ROIRect.y = rect_Roi.top;
			m_nP_ROI_height = m_rP_ROIRect.height = rect_Roi.bottom - m_nP_ROI_y;

			eROI.SetPlacement(rect_Roi.left, rect_Roi.top, rect_Roi.Width(), rect_Roi.Height());
			
			HPEN rPen = CreatePen(PS_DOT, 1, GREEN);
			HGDIOBJ rOldPen = (HPEN)SelectObject(m_dcMap, rPen);

			eROI.DrawFrame(m_dcMap.m_hDC, E_FRAME_ON, TRUE, fscaleX, fscaleY,0,0);
			SelectObject(m_dcMap.m_hDC,rOldPen); // 이전브러쉬를 다시 선택
			DeleteObject(rPen); // 새로 생성한 브러쉬 삭제
		}

		if(m_nMatchNums > 0 && m_bMatchMode == MATCH_PATRN_IMAGE)
		{
			HPEN rPen = CreatePen(PS_DOT, 1, DPINK);
			HGDIOBJ rOldPen = (HPEN)SelectObject(m_dcMap, rPen);

			EMatch1.DrawPositions(m_dcMap.m_hDC, FALSE, fscaleX, fscaleY,0,0);

			SelectObject(m_dcMap.m_hDC,rOldPen); // 이전브러쉬를 다시 선택
			DeleteObject(rPen); // 새로 생성한 브러쉬 삭제
		}
		
		CString sText;
		if(m_bMatchMode == MATCH_LEARN_IMAGE)
			sText.Format("Learn Image");
		else
			sText.Format("Pattern Image");

	 	SetTextOut(m_dcMap.m_hDC, 30, 0, m_bMatchMode, sText);

		(m_pMapDC)->StretchBlt(
			0,
			0,
			nRectView.right,
			nRectView.bottom,
			&m_dcMap,
			0,
			0,
			nRectView.right,
			nRectView.bottom,
			SRCCOPY);
	}
}


void EVisionProcess::DrawEPLearnImage(EROIBW8 eRoi)
{
	if(bEPEnable && !eRoi.GetVoid())
	{
		m_dcLMap.FillSolidRect(nRectEPLView, RGB(0,0,0));
		
		int x = eRoi.GetOrgX();

		float fLscaleX = (float)nRectEPLView.Width() / (float)eRoi.GetWidth();
		float fLscaleY = (float)nRectEPLView.Height() / (float)eRoi.GetHeight();
		 
		eRoi.Draw(m_dcLMap.m_hDC, fLscaleX, fLscaleY);
		HPEN rPen = CreatePen(PS_DOT, 1, RGB(255,0,0));
		HGDIOBJ rOldPen = SelectObject(m_dcLMap, rPen);
		//::SetBkMode(m_dcMap.m_hDC, TRANSPARENT);
		//eRoi.DrawFrame(m_dcLMap.m_hDC, E_FRAME_ON, TRUE, fLscaleX, fLscaleY,-eRoi.GetOrgX(),-eRoi.GetOrgY());

		(m_pLMapDC)->StretchBlt(
			0,
			0,
			nRectEPLView.right,
			nRectEPLView.bottom,
			&m_dcLMap,
			0,
			0,
			nRectEPLView.right,
			nRectEPLView.bottom,
			SRCCOPY);
	}
}


void EVisionProcess::DrawImageInformation(int mode, CString path)
{
	CString sDP;
	sDP.Format("  %s", path);

	if(mode == MATCH_LEARN_IMAGE)
		GetDlgItem(IDC_STATIC_IMAGE_PATH)->SetWindowText(sDP);
	else
		GetDlgItem(IDC_STATIC_IMAGE_PATH2)->SetWindowText(sDP);
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
=======
	UpdateData(FALSE);	
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
	UpdateData(FALSE);	
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
}


void EVisionProcess::MapMessage(MSG* pMsg)
{
	if(eSrcImage.GetVoid())
		return;

	GetDlgItem(IDC_IMAGE_EP_VIEW)->GetClientRect(m_rCtrlClientRect);
	GetDlgItem(IDC_IMAGE_EP_VIEW)->GetWindowRect(m_rCtrlWndRect);

	CPoint pos;
	pos.x = pMsg->pt.x - m_rCtrlWndRect.left;
	pos.y = pMsg->pt.y - m_rCtrlWndRect.top;

	if(pos.x >= 0 && pos.y >= 0 && pos.x < m_rCtrlClientRect.right && pos.y < m_rCtrlClientRect.bottom && m_bP_ROISet)
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN:			
			{
				pos.x = long(pos.x * double(nImageSizeX / double(m_rCtrlClientRect.right)));
				pos.y = long(pos.y * double(nImageSizeY / double(m_rCtrlClientRect.bottom)));

				m_pDrawStart.x = pos.x;
				m_pDrawStart.y = pos.y;
				m_nP_ROI_x = m_pDrawStart.x;
				m_nP_ROI_y = m_pDrawStart.y;
			}
			m_bLDown = TRUE;
			break;
		case WM_LBUTTONUP:
			{
				pos.x = long(pos.x * double(nImageSizeX / double(m_rCtrlClientRect.right)));
				pos.y = long(pos.y * double(nImageSizeY / double(m_rCtrlClientRect.bottom)));
				m_nP_ROI_x = m_pDrawStart.x;
				m_nP_ROI_y = m_pDrawStart.y;
				m_nP_ROI_width = m_pDrawEnd.x - m_nP_ROI_x;
				m_nP_ROI_height = m_pDrawEnd.y - m_nP_ROI_y;

				UpdateData(FALSE);
			}
			m_bLDown = FALSE;
			break;
		case WM_MOUSEMOVE:
			if(m_bLDown == TRUE)
			{
				pos.x = long(pos.x * double(nImageSizeX / double(m_rCtrlClientRect.right)));
				pos.y = long(pos.y * double(nImageSizeY / double(m_rCtrlClientRect.bottom)));

				m_pDrawEnd.x = pos.x;
				m_pDrawEnd.y = pos.y;
				m_nP_ROI_width = m_pDrawEnd.x - m_nP_ROI_x;
				m_nP_ROI_height = m_pDrawEnd.y - m_nP_ROI_y;

				DrawImage(m_bMatchMode);
			}
			break;
		}
	}

	return;

}

void EVisionProcess::OnBnClickedButton2()
{
	UpdateData(TRUE);
	
	m_rROIRect.x = m_nP_ROI_x;
	m_rROIRect.y = m_nP_ROI_y;
	m_rROIRect.width = m_nP_ROI_width;
	m_rROIRect.height = m_nP_ROI_height;

	m_nCBContrast = m_cmContrast.GetCurSel();
	m_nCBCorrMode = m_cmCorrMode.GetCurSel();

	g_Dlg_Main.SaveData(PARAMETER_PATH);
	DrawImage(m_bMatchMode);
	UpdateData(FALSE);
}


void EVisionProcess::OnBnClickedCheckShowPRoi()
{
	UpdateData(TRUE);
	DrawImage(m_bMatchMode);
}


void EVisionProcess::OnBnClickedCheckSetPRoi()
{
	UpdateData(TRUE);

	if(!m_bP_ROISet)
		m_bLDown = FALSE;

	DrawImage(m_bMatchMode);
}


void EVisionProcess::UpLoadData()
{	
	m_nP_ROI_x = m_rP_ROIRect.x = m_rROIRect.x;
	m_nP_ROI_y = m_rP_ROIRect.y = m_rROIRect.y;
	m_nP_ROI_width = m_rP_ROIRect.width = m_rROIRect.width;
	m_nP_ROI_height = m_rP_ROIRect.height = m_rROIRect.height;

	m_cmContrast.SetCurSel(m_nCBContrast);
	m_cmCorrMode.SetCurSel(m_nCBCorrMode);	
	SetScaleMode(m_bPScalMode);

	if(m_bLearnComplete)
	{		
		PreprocessLearn();
		DrawImage(m_bMatchMode);
	}
	UpdateData(FALSE);
}

BOOL EVisionProcess::PreTranslateMessage(MSG* pMsg)
{
	MapMessage(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}


void EVisionProcess::OnBnClickedButton3()
{
	if(eSrcImage.GetVoid())
	{
		AfxMessageBox("로드된 Learn 이미지가 없습니다.");
		return;
	}

	if( m_bMatchMode == MATCH_PATRN_IMAGE )
	{
		AfxMessageBox("Pattern Image Mode 입니다. Mode를 변경해주십시오");
		return;
	}
	TempleteMatch(eSrcImage, ePatImage, eRoiImage, m_rP_ROIRect, m_bP_ROISet, TRUE, FALSE);

	DrawImage(m_bMatchMode);	
	m_bP_ROISet = FALSE;
	UpdateData(FALSE);
}


CRect EVisionProcess::roi_Normalize(CRect rect)
{
	CRect _rR;
	_rR = rect;
	if (rect.left	< 0) _rR.left = 0;
	if (rect.left	> nImageSizeX-100) _rR.left = nImageSizeX-100;

	if (rect.right	< 100) _rR.right = 100;
	if (rect.right	> nImageSizeX) _rR.right = nImageSizeX;

	if (rect.top	< 0) _rR.top = 0;
	if (rect.top	> nImageSizeY-100) _rR.top = nImageSizeY-100;

	if (rect.bottom < 100) _rR.bottom = 100;
	if (rect.bottom > nImageSizeY) _rR.bottom = nImageSizeY;

	return _rR;	
}

void EVisionProcess::OnBnClickedRadioScaleMode()
{
	UpdateData(TRUE);
	SetScaleMode(m_bPScalMode);
}


void EVisionProcess::OnBnClickedRadioScaleMode2()
{
	UpdateData(TRUE);
	SetScaleMode(m_bPScalMode);
}


void EVisionProcess::SetScaleMode(int mode)
{
	if(m_bPScalMode == SCALE_BOTH)
	{
		GetDlgItem(IDC_STATIC_SCALE_BOTH)->SetWindowText("Scale (%)");
		GetDlgItem(IDC_STATIC_SCALE_EACH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SCALE_EACH_MIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_P_SCALE_EACH_MIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SCALE_EACH_MAX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_P_SCALE_EACH_MAX)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_SCALE_BOTH)->SetWindowText("Scale X (%)");
		GetDlgItem(IDC_STATIC_SCALE_EACH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SCALE_EACH_MIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_P_SCALE_EACH_MIN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_SCALE_EACH_MAX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_P_SCALE_EACH_MAX)->ShowWindow(SW_SHOW);		
	}
}

void EVisionProcess::EPSetContrastMode(int mode, EMatch& match)
{
	MCH_CONTRAST_MODE index;
	switch(mode)
	{
	case 0:			index = MCH_CONTRAST_NORMAL;		break;
	case 1:			index = MCH_CONTRAST_INVERSE;		break;
	case 2:			index = MCH_CONTRAST_ANY;			break;
	//default:		index = MCH_CONTRAST_NORMAL;		break;
	}

	match.SetContrastMode(index);
}
void EVisionProcess::EPSetCorrelationMode(int mode, EMatch& match)
{
	E_CORRELATION_MODE index;
	switch(mode)
	{
	case 0:			index = E_MATCH_STANDARD;				break;
	case 1:			index = E_MATCH_OFFSET_NORMALIZED;		break;
	case 2:			index = E_MATCH_GAIN_NORMALIZED;		break;
	case 3:			index = E_MATCH_NORMALIZED;				break;
	//default:		index = E_MATCH_STANDARD;				break;
	}

	match.SetCorrelationMode(index);
}

void EVisionProcess::OnBnClickedButtonExecute()
{
	UpdateData(TRUE);

	if(ePatImage.GetVoid())
	{
		AfxMessageBox("Load 한 대상 이미지가 없습니다.");
		return;
	}

	TempleteMatch(eSrcImage, ePatImage, eRoiImage, m_rP_ROIRect, m_bP_ROISet, FALSE, TRUE);	

	DrawImage(MATCH_PATRN_IMAGE);

	if(m_nMatchNums > 0)
		ShowResult(TRUE);
	else
		ShowResult(FALSE);
}

<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
void EVisionProcess::OnBnClickedButton4()
{
	UpdateData(TRUE);

	ImageProcess IP;
	Mat rans;
	rans = imread(string(CT2CA(m_sPatPath)), CV_LOAD_IMAGE_GRAYSCALE);

	IP.RansacProcess(RANSAC_LINE, rans, m_rP_ROIRect, TRUE, 15, m_nRSTh, FALSE, 100);

<<<<<<< HEAD
=======
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618

void EVisionProcess::OnBnClickedButton4()
{
	CString str = _T("Image files(*.jpg, *.bmp, *png)|*.jpg; *.bmp; *png|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	if (dlg.DoModal() == IDOK)
	{
		m_sPatPath = dlg.GetPathName();	

		// Mat input;
		// input = imread(string(CT2CA(m_sPath)), CV_LOAD_IMAGE_UNCHANGED);

		// 	pDlg_EP->simgPath = m_strPathName;
		char *pChar = (LPSTR)(LPCTSTR)(m_sPatPath);
		ePatImage.Load(pChar);
		CString sDP;
		sDP.Format("  %s", m_sPatPath);
		GetDlgItem(IDC_STATIC_IMAGE_PATH2)->SetWindowText(sDP);
		//DrawImageInformation(m_sPath);
	}
<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
}


void EVisionProcess::OnBnClickedRadioMatchMode()
{
	UpdateData(TRUE);
	DrawImage(m_bMatchMode);
}


void EVisionProcess::OnBnClickedRadioMatchMode2()
{
	UpdateData(TRUE);
	DrawImage(m_bMatchMode);
}


void EVisionProcess::ShowResult(BOOL bSucess)
{
	CString _s;
	if(bSucess)
	{
		_s.Format("SUCCESS");
		GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_s);
		_s.Format("%.3f", pPosition->m_f32Score*100);
		GetDlgItem(IDC_STATIC_RESULT_SCORE)->SetWindowText(_s);
		_s.Format("%.3f", pPosition->m_f32CenterX);
		GetDlgItem(IDC_STATIC_RESULT_CENTER_X)->SetWindowText(_s);
		_s.Format("%.3f", pPosition->m_f32CenterY);
		GetDlgItem(IDC_STATIC_RESULT_CENTER_Y)->SetWindowText(_s);
		_s.Format("%.3f", pPosition->m_f32Angle);
		GetDlgItem(IDC_STATIC_RESULT_ANGLE)->SetWindowText(_s);
		_s.Format("%.3f", pPosition->m_f32ScaleX*100);
		GetDlgItem(IDC_STATIC_RESULT_SCALE_X)->SetWindowText(_s);
		_s.Format("%.3f", pPosition->m_f32ScaleY*100);
		GetDlgItem(IDC_STATIC_RESULT_SCALE_Y)->SetWindowText(_s);
		_s.Format("%d", pPosition->m_bInterpolated);
		GetDlgItem(IDC_STATIC_RESULT_INTERPOLATE)->SetWindowText(_s);
	}
	else
	{
		_s.Format("Fail");
		GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_s);
		_s.Format(" - ");
		GetDlgItem(IDC_STATIC_RESULT_SCORE)->SetWindowText(_s);
		GetDlgItem(IDC_STATIC_RESULT_CENTER_X)->SetWindowText(_s);
		GetDlgItem(IDC_STATIC_RESULT_CENTER_Y)->SetWindowText(_s);
		GetDlgItem(IDC_STATIC_RESULT_ANGLE)->SetWindowText(_s);
		GetDlgItem(IDC_STATIC_RESULT_SCALE_X)->SetWindowText(_s);
		GetDlgItem(IDC_STATIC_RESULT_SCALE_Y)->SetWindowText(_s);
		GetDlgItem(IDC_STATIC_RESULT_INTERPOLATE)->SetWindowText(_s);
	}

	UpdateData(FALSE);
}

void EVisionProcess::SetTextOut(HDC hDC, int x, int y, BOOL mode, CString csText)
{
	if (mode == MATCH_LEARN_IMAGE)
	{
		::SetTextColor(hDC, GREEN);		
	}
	else
	{
		::SetTextColor(hDC, DPINK);
	}

	::TextOut(hDC, x, y, csText, csText.GetLength());
}

EMatch EVisionProcess::TempleteMatch(EImageBW8& eSrc, EImageBW8& ePat, EROIBW8& eRoi, Rect roi, BOOL bRoiUse, BOOL bLearn, BOOL bmatch)
{
	EMatch _rMatch;	

	m_nCBContrast = m_cmContrast.GetCurSel();
	m_nCBCorrMode = m_cmCorrMode.GetCurSel();

	if(bLearn)
	{
		LearnImage(eSrc, ePat, eRoi, roi, bRoiUse);
		m_bLearnComplete = TRUE;
	}
	
	if(bmatch)
	{
		EPSetContrastMode(m_nCBContrast, EMatch1);
		EPSetCorrelationMode(m_nCBCorrMode, EMatch1);
		//match.SetCorrelationMode(index);

		if(m_fPAngleMin < -360)
			m_fPAngleMin = 0;

		if(m_fPAngleMax > 360)
			m_fPAngleMax = 360;

		EMatch1.SetMinAngle((FLOAT32)(m_fPAngleMin));
		EMatch1.SetMaxAngle((FLOAT32)(m_fPAngleMax));

		if(m_bPScalMode == SCALE_BOTH)
		{
			EMatch1.SetMinScale((FLOAT32)(m_fPScaleBothMin / 100));
			EMatch1.SetMaxScale((FLOAT32)(m_fPScaleBothMax / 100));
		}
		else
		{
			EMatch1.SetMinScaleX((FLOAT32)(m_fPScaleBothMin / 100));
			EMatch1.SetMaxScaleX((FLOAT32)(m_fPScaleBothMax / 100));
			EMatch1.SetMinScaleY((FLOAT32)(m_fPScaleEachMin / 100));
			EMatch1.SetMaxScaleY((FLOAT32)(m_fPScaleEachMax / 100));
		}

		EMatch1.Match(&ePatImage);
		_rMatch = EMatch1;
		m_nMatchNums = EMatch1.GetNumPositions();
		pPosition = EMatch1.GetPosition(0);
	}

	return _rMatch;
}

void EVisionProcess::LearnImage(EImageBW8& eSrc, EImageBW8& ePat, EROIBW8& eRoi, Rect roi, BOOL bRoiUse)
{
	if(bRoiUse)
	{
		eRoi.Detach();
		eRoi.Attach(&eSrc);
		eRoi.SetPlacement(roi.x, roi.y, roi.width, roi.height);
		EMatch1.LearnPattern(&eRoi);
	}
	else
		EMatch1.LearnPattern(&eSrc);

	g_Dlg_Main.MakeDir(LEARN_IMAGE_PATH);

	CString f_path;
	f_path.Format("%s\\LearnImage.bmp", LEARN_IMAGE_PATH);
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c

	char *pChar = (LPSTR)(LPCTSTR)(f_path);

	eSrc.Save(pChar);
	//CopyFile(m_sPath, f_path, FALSE);
<<<<<<< HEAD
=======
=======
	CopyFile(m_sPath, f_path, FALSE);
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
	CopyFile(m_sPath, f_path, FALSE);
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
}

void EVisionProcess::eImageLoad(BOOL bMode, CString path)
{
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	if(bMode == MATCH_LEARN_IMAGE)
	{
		ConvertChannel(path, eSrcImage);
		//eSrcImage.Load(pChar);
	}
	else
	{
		ConvertChannel(path, ePatImage);
	}
	SetZoom();
	m_sPatPath = path;	
<<<<<<< HEAD
=======
=======
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
	char *pChar = (LPSTR)(LPCTSTR)(path);

	if(bMode == MATCH_LEARN_IMAGE)
	{
		m_sPath = path;
		eSrcImage.Load(pChar);
		SetZoom();
	}
	else
	{
		m_sPatPath = path;	
		ePatImage.Load(pChar);		
	}

<<<<<<< HEAD
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
	DrawImage(bMode);
	DrawImageInformation(bMode, path);
}

void EVisionProcess::PreprocessLearn()
{
	CString path;
	path.Format("%s\\LearnImage.bmp", LEARN_IMAGE_PATH);

	eImageLoad(MATCH_LEARN_IMAGE, path);
	int match_cnt = 0;
	TempleteMatch(eSrcImage, ePatImage, eRoiImage, m_rP_ROIRect, TRUE, TRUE, FALSE);
<<<<<<< HEAD
=======
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
}

bool EVisionProcess::ConvertChannel(CString path, EImageBW8 &eImg)
{
	// 칼라, 흑백 영상 확인용 이미지
	if(!m_mLoadImage.empty())
		m_mLoadImage.release();

	m_mLoadImage = imread(string(CT2CA(path)), CV_LOAD_IMAGE_GRAYSCALE);

	MatToEImage(m_mLoadImage, eImg);

	return TRUE;
}

bool EVisionProcess::MatToEImage(Mat src, EImageBW8 &dst)
{
	if(src.empty())
		return FALSE;

	dst.SetSize(src.cols, src.rows);

	size_t nSize = src.cols*src.rows*src.channels();
	byte* pImageBuffer = new byte [nSize];
	memcpy( pImageBuffer, src.data, nSize );

	EImageBW8 eTemp;
	eTemp.SetImagePtr(src.cols, src.rows, (char*)(pImageBuffer));
	ImgCopy(&eTemp, &dst);

	delete pImageBuffer;

	return TRUE;
}


void EVisionProcess::OnBnClickedButtonRotateCw()
{
	UpdateData(TRUE);

	SetAngle(++m_dbAngle);
	SetRotate(MODE_RELATIVE);
}

void EVisionProcess::SetAngle(double angle)
{
	m_dbAngle = angle;
	double temp;

	if(m_dbAngle >= 360 || m_dbAngle <= -360)
	{
		temp = (int)m_dbAngle % 360;
		m_dbAngle = temp;
	}
}


void EVisionProcess::OnBnClickedButtonRotateCcw()
{
	UpdateData(TRUE);

	SetAngle(--m_dbAngle);
	SetRotate(MODE_RELATIVE);
}


void EVisionProcess::OnBnClickedButtonRotate()
{
	UpdateData(TRUE);

	SetAngle(m_dbRtAngle);
	SetRotate(MODE_ABSOLUTE);
}


void EVisionProcess::SetRotate(BOOL bMode)
{
	ImageProcess IP;
	Mat rtt;

	IP.RotateMat(m_mLoadImage, rtt, GetAngle());	

	if(m_bMatchMode == MATCH_LEARN_IMAGE)
		MatToEImage(rtt, eSrcImage);
	else
		MatToEImage(rtt, ePatImage);

	DrawImage(m_bMatchMode);

	CString sDP;
	sDP.Format("   %-3f", m_dbAngle);
	GetDlgItem(IDC_STATIC_ROTATION)->SetWindowText(sDP);
	UpdateData(FALSE);

}

void EVisionProcess::OnBnClickedButton5()
{
	ImageProcess IP;
	Mat rtt;

	IP.RotateMat(m_mLoadImage, rtt, GetAngle());
	imwrite("Rotate Image.bmp", rtt);
}
<<<<<<< HEAD
=======
=======
}
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
=======
}
>>>>>>> 31da9830354ebe6cb7c2c77e401a5d58f0f34618
>>>>>>> e8c4d1a5b95cf70f7bec4b408aa87ebae0aa0e2c
