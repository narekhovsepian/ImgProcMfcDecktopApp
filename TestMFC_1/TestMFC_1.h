
// TestMFC_1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestMFC1App:
// See TestMFC_1.cpp for the implementation of this class
//

class CTestMFC1App : public CWinApp
{
protected:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

public:
	CTestMFC1App();

	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestMFC1App theApp;
