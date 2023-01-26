
// ImageSimulation.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "ImageProcess.h"
#include "ImageSimulationDlg.h"
#include "EVisionProcess.h"
// CImageSimulationApp:
// See ImageSimulation.cpp for the implementation of this class
//

class CImageSimulationApp : public CWinApp
{
public:
	CImageSimulationApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CImageSimulationApp theApp;