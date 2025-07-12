
// TestMFC_1Dlg.h : header file
//

#pragma once
#include <memory>
#include "ImageViewer.h"
#include "xFilters.h"
#include "xImage.h"
#include "cMemoryDeviceContext.h"
#include "xCSliderCtrl.h"
// CTestMFC1Dlg dialog
class CTestMFC1Dlg : public CDialogEx
{
	std::unique_ptr<xImage> _pOriginImage{ nullptr };		//std::unique_ptr<Gdiplus::Bitmap> _pOriginImage{ nullptr };
	std::unique_ptr<xImage> _pTransformedImage{ nullptr };	//std::unique_ptr<Gdiplus::Bitmap> _pTransformedImage{ nullptr };

	BITMAPINFO _bitmapInfo;

	void setBitmapInfo();

	int _minDlgWidth = 0;
	int _minDlgHeight = 0;
	bool _isImageOpened{};
	bool _mouseLeftButtonClick{};
	bool _minimizeWindow{ false };

	// Construction
public:
	CTestMFC1Dlg(CWnd* pParent = nullptr);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTMFC_1_DIALOG };
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

	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSaveAs();
	CImageViewer _imageViewer;
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnStnClickedStaticImageViewer();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStnClickedPercentimage();
private:
	CStatic _showCurrentPercent;
public:

	void UpdateButtons(bool ch) const;
	void FillComboBoxChangePercent();
	void FillComboBoxChangeFilters();
	afx_msg void OnBnClickedZoomdown();
	afx_msg void OnBnClickedZoomup();
	afx_msg void OnCbnSelchangeCombo2changepercent();
protected:
	CComboBox _changePercentZoom;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	CComboBox _ChangeFilters;
public:
	afx_msg void OnCbnSelchangeCombo3changefilters();
	afx_msg void OnBnClickedButton1applyfilter();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	CStatic _showProcessingTime;

public:

protected:

	////////////// Genereic sliders ///////////////
	xCSliderCtrl _genericSlider0;
	xCSliderCtrl _genericSlider1;
	xCSliderCtrl _genericSlider2;
	///////
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	// Support OpenCl Functions for initialize Context /////////////////////////////
	cl::Context _context;

	// int 0 device connectex pci GPU
	// int 1 device onboard in cpu GPU 
	bool initializeOpenClContext(int g = 0);

	//////////// CPU or GPU  computing //////////////////////////////////////////;

	enum CpuGpu {
		CPUMULTITHREAD = 0,
		CPU,
		GPU,
	};

	CpuGpu _cpuGpu;

};
