
// TestMFC_1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TestMFC_1.h"
#include "TestMFC_1Dlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <chrono>
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


// CTestMFC1Dlg dialog

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}



CTestMFC1Dlg::CTestMFC1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTMFC_1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CTestMFC1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE_VIEWER, _imageViewer);
	DDX_Control(pDX, IDC_STATIC_CURR_PERCENT, _showCurrentPercent);
	DDX_Control(pDX, IDC_COMBO1CHANGEPERCENT, _changePercentZoom);
	DDX_Control(pDX, IDC_COMBO3ChangeFilters, _ChangeFilters);
	DDX_Control(pDX, IDC_STATIC_PROCESSINGTIME, _showProcessingTime);
	DDX_Control(pDX, IDC_SLIDER_GENERIC, _genericSlider0);
	DDX_Control(pDX, IDC_SLIDER_GENERIC1, _genericSlider1);
	DDX_Control(pDX, IDC_SLIDER_GENERIC2, _genericSlider2);
}

BEGIN_MESSAGE_MAP(CTestMFC1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTestMFC1Dlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, &CTestMFC1Dlg::OnBnClickedButtonSaveAs)
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_STN_CLICKED(IDC_STATIC_IMAGE_VIEWER, &CTestMFC1Dlg::OnStnClickedStaticImageViewer)
	ON_WM_CREATE()

	ON_BN_CLICKED(IDC_ZoomDown, &CTestMFC1Dlg::OnBnClickedZoomdown)
	ON_BN_CLICKED(IDC_ZoomUP, &CTestMFC1Dlg::OnBnClickedZoomup)
	ON_CBN_SELCHANGE(IDC_COMBO1CHANGEPERCENT, &CTestMFC1Dlg::OnCbnSelchangeCombo2changepercent)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

	ON_CBN_SELCHANGE(IDC_COMBO3ChangeFilters, &CTestMFC1Dlg::OnCbnSelchangeCombo3changefilters)
	ON_BN_CLICKED(IDC_BUTTON1ApplyFilter, &CTestMFC1Dlg::OnBnClickedButton1applyfilter)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CTestMFC1Dlg message handlers

BOOL CTestMFC1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	RECT wndRect;
	RECT pictureboxRect;
	GetWindowRect(&wndRect);
	_imageViewer.GetWindowRect(&pictureboxRect);
	_imageViewer.SetViewerRightPadding(wndRect.right - pictureboxRect.right);
	_imageViewer.SetViewerBottomPadding(wndRect.bottom - pictureboxRect.bottom);

	_imageViewer.SetBitmapInfoRef(&_bitmapInfo);

	if (!_imageViewer.Initialize())
	{
		TRACE(L"ERROR: _imageViewer.Initialize()\n");
	}

	// On Of buttons dialog
	UpdateButtons(false);
	// fille combobox
	FillComboBoxChangePercent();
	FillComboBoxChangeFilters();


	////////// Generic Sliders ///////////////

	_genericSlider0.CreateBuddySliderAndPos(GetDlgItem(IDC_STATICXMINVALUE), GetDlgItem(IDC_STATICXMIDDLEVALUE), GetDlgItem(IDC_STATICXMAXVALUE), GetDlgItem(IDC_STATICXCURRVALUE), GetDlgItem(IDC_STATICSLIDERONAME), { 20, 210 });

	_genericSlider0.ShowWindowXCslider(SW_HIDE);

	_genericSlider1.CreateBuddySliderAndPos(GetDlgItem(IDC_STATICXMINVALUE1), GetDlgItem(IDC_STATICXMIDDLEVALUE1), GetDlgItem(IDC_STATICXMAXVALUE1), GetDlgItem(IDC_STATICXCURRVALUE1), GetDlgItem(IDC_STATICSLIDER1NAME), { 20, 290 });

	_genericSlider1.ShowWindowXCslider(SW_HIDE);

	_genericSlider2.CreateBuddySliderAndPos(GetDlgItem(IDC_STATICXMINVALUE2), GetDlgItem(IDC_STATICXMIDDLEVALUE2), GetDlgItem(IDC_STATICXMAXVALUE2), GetDlgItem(IDC_STATICXCURRVALUE2), GetDlgItem(IDC_STATICSLIDER2NAME), { 20, 370 });

	_genericSlider2.ShowWindowXCslider(SW_HIDE);

	/////////////////////////



	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTestMFC1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestMFC1Dlg::OnPaint()
{
	// device context for painting
	CPaintDC dc(this);


	if (IsIconic())
	{

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
		if (_pOriginImage != nullptr)
		{

			_imageViewer.UpdatePreview();
			bool status = _imageViewer.DrawImage();

			if (!status)
			{
				// off  buttons
				UpdateButtons(false);

				TRACE("ERROR: in function DrawImage\n");
			}



		}
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestMFC1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestMFC1Dlg::setBitmapInfo() {
	_bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bitmapInfo.bmiHeader.biWidth = _pOriginImage->GetWidth();
	_bitmapInfo.bmiHeader.biHeight = -_pOriginImage->GetHeight();
	_bitmapInfo.bmiHeader.biPlanes = 1;
	_bitmapInfo.bmiHeader.biBitCount = _pOriginImage->GetBpp();
	_bitmapInfo.bmiHeader.biCompression = BI_RGB;
	_bitmapInfo.bmiHeader.biSizeImage = _pOriginImage->GetRowBytes();
	_bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	_bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	_bitmapInfo.bmiHeader.biClrUsed = 0;
	_bitmapInfo.bmiHeader.biClrImportant = 0;

}


void CTestMFC1Dlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	LPCTSTR pszFilter =
		_T("All Images (*.jpeg;*.jpg;*.png;*.bmp)|*.jpeg;*.jpg;*.png;*.bmp|")
		_T("jpeg (*.jpeg;*.jpg;)|*.jpeg;*.jpg|")
		_T("png (*.png)|*.png|")
		_T("bmp (*.bmp)|*.bmp|")
		_T("All Files (*.*)|*.*||");

	CFileDialog dlgFile(TRUE, _T("bmp"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, pszFilter, AfxGetMainWnd());
	if (IDOK == dlgFile.DoModal())
	{

		CString extension = dlgFile.GetFileExt();
		if (extension == _T("bmp") || extension == _T("png") || extension == _T("jpg") || extension == _T("jpeg"))
		{

			Gdiplus::Bitmap bitmap(dlgFile.GetPathName());

			auto imageRect = Gdiplus::Rect(0, 0, bitmap.GetWidth(), bitmap.GetHeight());

			Gdiplus::BitmapData inData;
			auto order_temp = PixelFormat24bppRGB; // get this information from opened image file

			auto status = bitmap.LockBits(&imageRect, Gdiplus::ImageLockMode::ImageLockModeRead, order_temp, &inData);

			if (status != Gdiplus::Ok) {

				TRACE(L" Cannot open Input image\n\n");
				return;
			}

			COLOR_ORDER imageOrder = COLOR_ORDER::_unknown;
			int bpp = 0;
			if (order_temp == PixelFormat24bppRGB)
			{
				imageOrder = COLOR_ORDER::_BGR;
				bpp = 24;

			}
			else if (order_temp == PixelFormat32bppARGB)
			{
				imageOrder = COLOR_ORDER::_ARGB;
				bpp = 32;

			}

			// Reset Origianl Image Data and Device Memory Context
			_pOriginImage.reset();
			_imageViewer.GetOriginalDMCRef()->DeleteWindowsMemoryDC();


			_pOriginImage = std::make_unique<xImage>(inData.Width, inData.Height, inData.Stride, bpp, imageOrder, (uint8_t*)inData.Scan0);

			bitmap.UnlockBits(&inData);

			/////////////////////////////////////////////////////////////

			if (_pOriginImage)
			{
				// w = 2048 h = 1024 <= mode true else mode false
				// Fill BITMAPINFO
				setBitmapInfo();

				// Rest and copy original image to transformed image Delete Device Memory Context
				_pTransformedImage.reset();
				_imageViewer.GetTransformedDMCRef()->DeleteWindowsMemoryDC();

				_pTransformedImage = std::make_unique<xImage>(*_pOriginImage);

				if (_pTransformedImage == nullptr)
				{
					AfxMessageBox(L"ERROR! The transormed image didn't create");
					return;
				}

				_imageViewer.SetOrignalTransformedImageDataRef(_pOriginImage.get(), _pTransformedImage.get());

				// Set Data Ref For Device Memory Context/////
				_imageViewer.GetOriginalDMCRef()->SetDataRef((uint8_t*)_pOriginImage->GetData());
				_imageViewer.GetTransformedDMCRef()->SetDataRef((uint8_t*)_pTransformedImage->GetData());
				_imageViewer.GetOriginalDMCRef()->setMode(_pOriginImage->GetWidth(), _pOriginImage->GetHeight());
				_imageViewer.GetTransformedDMCRef()->setMode(_pTransformedImage->GetWidth(), _pTransformedImage->GetHeight());
				/////////////////////////////////////////////

				_imageViewer.SetCalcByPercentWidthHeight(false);
				_imageViewer.SetMouseMoveImage(false);
				//_imageViewer.UpdatePreview();

				CString strTmp;
				strTmp.Format(L"Current Percent %.2f", _imageViewer.GetCurrentPercent());
				_showCurrentPercent.SetWindowText(strTmp);
				UpdateButtons(true);
				_changePercentZoom.SetCurSel(0);
				_ChangeFilters.SetCurSel(0);

				_isImageOpened = true;

				CWnd* applyboton = GetDlgItem(IDC_BUTTON1ApplyFilter);
				if (applyboton) {
					applyboton->EnableWindow(FALSE);
				}
				// hide change generic slider
				_genericSlider0.ShowWindowXCslider(SW_HIDE);
				_genericSlider1.ShowWindowXCslider(SW_HIDE);
				_genericSlider2.ShowWindowXCslider(SW_HIDE);

				CString str;
				str.Format(L"Test Dialog   %s %dx%d", dlgFile.GetFileName(), (INT)_pOriginImage->GetWidth(), (INT)_pOriginImage->GetHeight());

				_showProcessingTime.SetWindowText(L"Processing Time");

				this->SetWindowTextW(str);

				CRect imageRect;
				_imageViewer.GetWindowRect(&imageRect);
				ScreenToClient(&imageRect);
				InvalidateRect(imageRect);

			}
			else
			{
				AfxMessageBox(L"ERROR! the last status isn't Ok");
			}
		}
		else
		{
			AfxMessageBox(L"ERROR! cant open selected file");
		}
	}


}


void CTestMFC1Dlg::OnBnClickedButtonSaveAs()
{
	if (_pOriginImage)
	{
		// TODO: Add your control notification handler code here
		LPCTSTR pszFilter =
			_T("jpeg (*.jpeg)|*.jpeg|")
			_T("png (*.png)|*.png|")
			_T("bmp (*.bmp)|*.bmp|");
		CFileDialog dlgFile(FALSE, _T("jpg"), _T("united"), OFN_OVERWRITEPROMPT, pszFilter, AfxGetMainWnd());

		if (IDOK == dlgFile.DoModal())
		{
			// 1. Get file extension
			// 2. select correct codec/encoder
			// 3. save bitmap image
			CLSID Clsid;
			if (dlgFile.GetFileExt() == _T("bmp")) {
				//CLSID bmpClsid;
				if (GetEncoderClsid(L"image/bmp", &Clsid) == -1) {
					AfxMessageBox(L"error can't save picture");
					return;
				}

			}
			else if (dlgFile.GetFileExt() == _T("jpg") || dlgFile.GetFileExt() == _T("jpeg")) {
				//CLSID jpgClsid;
				if (GetEncoderClsid(L"image/jpeg", &Clsid) == -1) {
					AfxMessageBox(L"error can't encode picture");
					return;
				}

			}
			else if (dlgFile.GetFileExt() == _T("png")) {
				//CLSID pngClsid;
				if (GetEncoderClsid(L"image/png", &Clsid) == -1) {
					AfxMessageBox(L"error can't encode picture");
					return;
				}

			}

			Gdiplus::PixelFormat pixelFormat = PixelFormatUndefined;
			if (_pTransformedImage->GetBpp() == 24 && _pTransformedImage->GetOrder() == COLOR_ORDER::_BGR)
				pixelFormat = PixelFormat24bppRGB;
			else if (_pTransformedImage->GetBpp() == 32 && _pTransformedImage->GetOrder() == COLOR_ORDER::_ARGB)
				pixelFormat = PixelFormat32bppARGB;

			Gdiplus::Bitmap saveImage(_pTransformedImage->GetWidth(),
				_pTransformedImage->GetHeight(), _pTransformedImage->GetRowBytes(),
				pixelFormat, (uint8_t*)_pTransformedImage->GetData());

			if (saveImage.Save(dlgFile.GetPathName(), &Clsid) == Gdiplus::Aborted) {
				AfxMessageBox(L"can't save picture");
				return;
			}

		}

	}
	else
	{
		AfxMessageBox(L"Please first open image!");
	}

}


void CTestMFC1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CTestMFC1Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMinTrackSize.x = _minDlgWidth;
	lpMMI->ptMinTrackSize.y = _minDlgHeight;
	//TRACE("_X %d _Y %d", lpMMI->ptMaxTrackSize.x, lpMMI->ptMaxTrackSize.y);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CTestMFC1Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (_imageViewer.GetSafeHwnd() != nullptr)
	{
		if (nType == SIZE_MINIMIZED) {
			_minimizeWindow = true;
		}



		if (_minimizeWindow == false && _isImageOpened) {
			// Delete Original and transformed Device Memory Context
			if (_imageViewer.GetOriginalDMCRef())
				_imageViewer.GetOriginalDMCRef()->ResizingWindow();
			if (_imageViewer.GetTransformedDMCRef())
				_imageViewer.GetTransformedDMCRef()->ResizingWindow();
			/////////////////
		}

		if (nType != SIZE_MINIMIZED)
			_minimizeWindow = false;




		_imageViewer.SetMouseMoveImage(false);
		_imageViewer.UpdatePos();
		CString strTmp;
		strTmp.Format(L"Current Percent %.2f", _imageViewer.GetCurrentPercent());
		_showCurrentPercent.SetWindowText(strTmp);

		// for calculate new pading for image
	}
	CDialogEx::OnSize(nType, cx, cy);
}

void CTestMFC1Dlg::OnSizing(UINT fwSide, LPRECT pRect)
{


	CDialogEx::OnSizing(fwSide, pRect);
}

void CTestMFC1Dlg::OnStnClickedStaticImageViewer()
{
	// TODO: Add your control notification handler code here


}

int CTestMFC1Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	RECT dlgRect;
	GetWindowRect(&dlgRect);

	_minDlgWidth = dlgRect.right - dlgRect.left;
	_minDlgHeight = dlgRect.bottom - dlgRect.top;

	return 0;
}


void CTestMFC1Dlg::UpdateButtons(bool ch) const {
	CWnd* saveas = GetDlgItem(IDC_BUTTON_SAVE_AS);
	if (saveas) {
		saveas->EnableWindow(ch);
	}
	CWnd* zoomplus = GetDlgItem(IDC_ZoomUP);
	if (zoomplus) {
		zoomplus->EnableWindow(ch);
	}
	CWnd* zoomminus = GetDlgItem(IDC_ZoomDown);
	if (zoomminus) {
		zoomminus->EnableWindow(ch);
	}
	CWnd* comboboxPercent = GetDlgItem(IDC_COMBO1CHANGEPERCENT);
	if (comboboxPercent) {
		comboboxPercent->EnableWindow(ch);
	}
	CWnd* comboboxFilter = GetDlgItem(IDC_COMBO3ChangeFilters);
	if (comboboxFilter) {
		comboboxFilter->EnableWindow(ch);
	}
	CWnd* applyFilter = GetDlgItem(IDC_BUTTON1ApplyFilter);
	if (applyFilter) {
		applyFilter->EnableWindow(ch);
	}


}

void CTestMFC1Dlg::FillComboBoxChangePercent() {
	CString str;
	str.Format(_T("Best Fit"));
	_changePercentZoom.AddString(str);
	for (const auto& i : _imageViewer._zoomPercent) {
		str.Format(_T("%d"), i);
		_changePercentZoom.AddString(str);

	}
	_changePercentZoom.SetCurSel(0);
}

void CTestMFC1Dlg::FillComboBoxChangeFilters() {

	for (auto& it : filter::StrNames)
	{
		_ChangeFilters.AddString(it.c_str());
	}
	_ChangeFilters.SetCurSel(0);
}



void CTestMFC1Dlg::OnBnClickedZoomdown()
{
	// TODO: Add your control notification handler code here

	_imageViewer._CalcCurrPercent(false, false);
	CString strTmp;
	strTmp.Format(L"Current Percent %.2f", _imageViewer.GetCurrentPercent());
	_showCurrentPercent.SetWindowText(strTmp);
	_imageViewer.SetCalcByPercentWidthHeight(true);
	_imageViewer.SetMouseMoveImage(false);
	int i = (int)_imageViewer.GetZoomPercentIndex();
	if (i + 1 > 0) {
		_changePercentZoom.SetCurSel(i + 1);
	}

	CWnd* ptr = GetDlgItem(IDC_ZoomDown);
	if (ptr) {
		if (_imageViewer.GetZoomPercentIndex() == 0) {
			ptr->EnableWindow(FALSE);
		}
		else {
			ptr->EnableWindow(TRUE);
		}
	}
	CWnd* ptrup = GetDlgItem(IDC_ZoomUP);
	if (ptrup) {
		ptrup->EnableWindow(TRUE);

	}

	// Delete Original and transformed Device Memory Context
	_imageViewer.GetOriginalDMCRef()->DeleteWindowsMemoryDC();
	_imageViewer.GetTransformedDMCRef()->DeleteWindowsMemoryDC();
	/////////////////

	CRect imgREct;
	_imageViewer.GetWindowRect(&imgREct);
	ScreenToClient(&imgREct);
	InvalidateRect(&imgREct);
}


void CTestMFC1Dlg::OnBnClickedZoomup()
{
	// TODO: Add your control notification handler code here

	_imageViewer._CalcCurrPercent(false, true);
	CString strTmp;
	strTmp.Format(L"Current Percent %.2f", _imageViewer.GetCurrentPercent());
	_showCurrentPercent.SetWindowText(strTmp);

	_imageViewer.SetCalcByPercentWidthHeight(true);
	_imageViewer.SetMouseMoveImage(false); //anjatel em vor mouse ov hashvi kordinatnery
	_changePercentZoom.SetCurSel((int)_imageViewer.GetZoomPercentIndex() + 1);
	CWnd* ptrdown = GetDlgItem(IDC_ZoomDown);
	if (ptrdown) {
		if (_imageViewer.GetZoomPercentIndex() == 0) {
			ptrdown->EnableWindow(FALSE);
		}
		else {
			ptrdown->EnableWindow(TRUE);
		}
	}
	CWnd* ptrup = GetDlgItem(IDC_ZoomUP);
	if (ptrup) {
		if (_imageViewer.GetZoomPercentIndex() + 1 == _imageViewer._zoomPercent.size()) {
			ptrup->EnableWindow(FALSE);
		}
		else {
			ptrup->EnableWindow(TRUE);

		}
	}


	// Delete Original and transformed Device Memory Context
	_imageViewer.GetOriginalDMCRef()->DeleteWindowsMemoryDC();
	_imageViewer.GetTransformedDMCRef()->DeleteWindowsMemoryDC();
	/////////////////

	CRect imgREct;
	_imageViewer.GetWindowRect(&imgREct);
	ScreenToClient(&imgREct);
	InvalidateRect(&imgREct);

}


void CTestMFC1Dlg::OnCbnSelchangeCombo2changepercent()
{
	// TODO: Add your control notification handler code here
	int i = _changePercentZoom.GetCurSel();
	CString strTmp;
	if (i == 0) {
		_imageViewer.SetCalcByPercentWidthHeight(false);
		//	_imageViewer.UpdatePreview();

		_imageViewer.SetZoomPercentAndIndex(i);

	}
	else {
		_imageViewer.SetZoomPercentAndIndex(i);
		_imageViewer.SetCalcByPercentWidthHeight(true);
	}

	CWnd* ptr = GetDlgItem(IDC_ZoomDown);
	if (ptr) {
		if (i == 0) {
			ptr->EnableWindow(FALSE);
		}
		else {
			ptr->EnableWindow(TRUE);
		}
	}
	CWnd* ptrup = GetDlgItem(IDC_ZoomUP);
	if (ptrup) {
		if (_imageViewer.GetZoomPercentIndex() == _imageViewer._zoomPercent.size()) {
			ptrup->EnableWindow(FALSE);
		}
		else {
			ptrup->EnableWindow(TRUE);

		}
	}
	strTmp.Format(L"Current Percent %.2f", _imageViewer.GetCurrentPercent());
	_showCurrentPercent.SetWindowText(strTmp);
	_imageViewer.SetMouseMoveImage(false);


	// Delete Original and transformed Device Memory Context
	_imageViewer.GetOriginalDMCRef()->DeleteWindowsMemoryDC();
	_imageViewer.GetTransformedDMCRef()->DeleteWindowsMemoryDC();
	/////////////////


	CRect imgREct;
	_imageViewer.GetWindowRect(&imgREct);
	ScreenToClient(&imgREct);
	InvalidateRect(&imgREct);

}


void CTestMFC1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (_isImageOpened) {
		auto padingImage = _imageViewer.GetXYPadingImageBackup();

		RECT stViewerClient = { 0,0,0,0 };
		_imageViewer.GetWindowRect(&stViewerClient);
		ScreenToClient(&stViewerClient);
		if ((padingImage.x < 0 || padingImage.y < 0)) {



			if (::PtInRect(&stViewerClient, point))
			{
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

				if (_mouseLeftButtonClick)
				{
					if (_imageViewer.GetXYMouseMoveStatus()) {

						auto x = _imageViewer.GetXYMouseClickedCordinates();

						_imageViewer.SetXYMouseClickedCordinatesBackup(x);
						_imageViewer.SetXYMouseMoveStatus(false);

						_imageViewer.SetXYCordinateClickedDiferenceLastStatus({ 0,0 });

					}

					_imageViewer.SetXYMouseMoveCordinates(point);

					// For move image in rectnagle
					_imageViewer.SetMouseMoveImage(true);

					// calculate update x y for draw
					_imageViewer.UpdatePreview();

					// For cMemoryDeviceContext 
					_imageViewer.GetOriginalDMCRef()->UpdateMouseImagePos();
					_imageViewer.GetTransformedDMCRef()->UpdateMouseImagePos();
					////////////

					// draw image moved
					_imageViewer.DrawImage();
				}
			}
			else
			{
				_mouseLeftButtonClick = false;
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				//TRACE(L"Mouse is out rect\n");
			}
		}
		//TRACE("leftRect %d topRect %d RightRect %d bottomRect %d \n", stViewerClient.left, stViewerClient.top, stViewerClient.right, stViewerClient.bottom);
	}

	//TRACE(L"\nX %d Y %d\n", point.x, point.y);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CTestMFC1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (_isImageOpened) {
		_mouseLeftButtonClick = true;
		_imageViewer.SetXYMouseMoveStatus(true);
		_imageViewer.SetXYMouseClickedCordinates(point);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTestMFC1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (_isImageOpened) {
		_mouseLeftButtonClick = false;

		_imageViewer.SetXYMouseMoveStatus(false);
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CTestMFC1Dlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (_isImageOpened) {
		// true for draw Original image
		RECT stViewer = { 0,0,0,0 };
		_imageViewer.GetWindowRect(&stViewer);
		ScreenToClient(&stViewer);
		if (::PtInRect(&stViewer, point))
		{
			_imageViewer.OriginalOrTransformedImageMouseRightClick(true);


			InvalidateRect(&stViewer);
		}
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CTestMFC1Dlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (_isImageOpened) {

		RECT stViewer = { 0,0,0,0 };
		_imageViewer.GetWindowRect(&stViewer);
		ScreenToClient(&stViewer);
		if (::PtInRect(&stViewer, point))
		{
			// false draw transformed image
			_imageViewer.OriginalOrTransformedImageMouseRightClick(false);

			InvalidateRect(&stViewer);
		}

	}
	CDialogEx::OnRButtonUp(nFlags, point);
}


void CTestMFC1Dlg::OnCbnSelchangeCombo3changefilters()
{
	// TODO: Add your control notification handler code here
	int iCurrentFilter = _ChangeFilters.GetCurSel();
	CString str;
	if (iCurrentFilter == 0) {
		CWnd* applyboton = GetDlgItem(IDC_BUTTON1ApplyFilter);
		if (applyboton) {
			applyboton->EnableWindow(FALSE);
		}
	}
	else {
		CWnd* applybuton = GetDlgItem(IDC_BUTTON1ApplyFilter);
		if (applybuton) {
			applybuton->EnableWindow(TRUE);
		}
	}

	switch ((filter::FilterNames)iCurrentFilter)
	{
	case filter::FilterNames::_Grayscale:
		_genericSlider0.ShowWindowXCslider(SW_HIDE);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_BlackAndWhite:
		_genericSlider0.SetRange(0, 100);
		_genericSlider0.SetMiddle(50);
		_genericSlider0.SetPos(50);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_OnlyRed:
		_genericSlider0.ShowWindowXCslider(false);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_OnlyGreen:
		_genericSlider0.ShowWindowXCslider(false);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_OnlyBlue:
		_genericSlider0.ShowWindowXCslider(false);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_GraycaleNegative:
		_genericSlider0.ShowWindowXCslider(false);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_Negative:
		_genericSlider0.ShowWindowXCslider(false);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_B_W_Negative:
		_genericSlider0.SetRange(0, 100);
		_genericSlider0.SetPos(50);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_Contrast:
		_genericSlider0.SetRange(-100, 100);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_Brightness:
		_genericSlider0.SetRange(-50, 50);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_Gamma:
		_genericSlider0.SetRange(-99, 100);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_GaussianBlurClassic:

		_genericSlider0.SetRange(0, 100);
		_genericSlider0.SetMiddle(50);
		_genericSlider0.SetPos(0);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_RecursiveGaussianBlurVliet:
		_genericSlider0.SetRange(0, 100, true);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(50);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_GaussianBlurSeperableConvolutionOpencvKernel:
		_genericSlider0.SetRange(0, 100, true);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(50);
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_HSVColorSpace:

		_genericSlider0.SetRange(-180, 180, true);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.SetName(L"Hue");
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);

		_genericSlider1.SetRange(-100, 100, true);
		_genericSlider1.SetPos(0);
		_genericSlider1.SetMiddle(0);
		_genericSlider1.SetName(L"Saturation");
		_genericSlider1.ShowWindowXCslider(SW_NORMAL);

		_genericSlider2.SetRange(-100, 100, true);
		_genericSlider2.SetPos(0);
		_genericSlider2.SetMiddle(0);
		_genericSlider2.SetName(L"Value");
		_genericSlider2.ShowWindowXCslider(SW_NORMAL);

		break;
	case filter::FilterNames::_HSLColorSpace:

		_genericSlider0.SetRange(-180, 180, true);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.SetName(L"Hue");
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);

		_genericSlider1.SetRange(-100, 100, true);
		_genericSlider1.SetPos(0);
		_genericSlider1.SetMiddle(0);
		_genericSlider1.SetName(L"Saturation");
		_genericSlider1.ShowWindowXCslider(SW_NORMAL);

		_genericSlider2.SetRange(-100, 100, true);
		_genericSlider2.SetPos(0);
		_genericSlider2.SetMiddle(0);
		_genericSlider2.SetName(L"Lightness");
		_genericSlider2.ShowWindowXCslider(SW_NORMAL);

		break;
	case filter::FilterNames::_YCbCrColorSpace:

		_genericSlider0.SetRange(-100, 100, true);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.SetName(L"Y");
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);

		_genericSlider1.SetRange(-100, 100, true);
		_genericSlider1.SetPos(0);
		_genericSlider1.SetMiddle(0);
		_genericSlider1.SetName(L"Cb");
		_genericSlider1.ShowWindowXCslider(SW_NORMAL);

		_genericSlider2.SetRange(-100, 100, true);
		_genericSlider2.SetPos(0);
		_genericSlider2.SetMiddle(0);
		_genericSlider2.SetName(L"Cr");
		_genericSlider2.ShowWindowXCslider(SW_NORMAL);
		break;
	case filter::FilterNames::_YUVColorSpace:

		_genericSlider0.SetRange(-128, 127, true);
		_genericSlider0.SetPos(0);
		_genericSlider0.SetMiddle(0);
		_genericSlider0.SetName(L"Y");
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);

		_genericSlider1.SetRange(-128, 127, true);
		_genericSlider1.SetPos(0);
		_genericSlider1.SetMiddle(0);
		_genericSlider1.SetName(L"U");
		_genericSlider1.ShowWindowXCslider(SW_NORMAL);

		_genericSlider2.SetRange(-128, 127, true);
		_genericSlider2.SetPos(0);
		_genericSlider2.SetMiddle(0);
		_genericSlider2.SetName(L"V");
		_genericSlider2.ShowWindowXCslider(SW_NORMAL);
		break;
	case filter::FilterNames::_SobelEdgeDetection:
		_genericSlider0.SetRange(0, 7, true);
		_genericSlider0.SetPos(3);
		_genericSlider0.SetMiddle(3);
		_genericSlider0.SetName(L"");
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_PrewittEdgeDetection:
		_genericSlider0.SetRange(0, 7, true);
		_genericSlider0.SetPos(3);
		_genericSlider0.SetMiddle(3);
		_genericSlider0.SetName(L"");
		_genericSlider0.ShowWindowXCslider(SW_NORMAL);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_CannyEdgeDetection:
		_genericSlider0.ShowWindowXCslider(SW_HIDE);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	case filter::FilterNames::_test:
		_genericSlider0.ShowWindowXCslider(SW_HIDE);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);

		break;
	case filter::FilterNames::_None:
		_genericSlider0.ShowWindowXCslider(SW_HIDE);
		_genericSlider1.ShowWindowXCslider(SW_HIDE);
		_genericSlider2.ShowWindowXCslider(SW_HIDE);
		break;
	}

}


void CTestMFC1Dlg::OnBnClickedButton1applyfilter()
{
	// TODO: Add your control notification handler code here

	if (_pOriginImage.get() != nullptr && _pTransformedImage.get() != nullptr)
	{


		auto inData = _pOriginImage.get();
		auto outData = _pTransformedImage.get();



		int iCurrFilter = _ChangeFilters.GetCurSel();
		float x{};
		int slider{};


		BeginWaitCursor();
		auto beginTime = std::chrono::high_resolution_clock::now();
		switch ((filter::FilterNames)iCurrFilter)
		{
		case filter::FilterNames::_Grayscale:
			//filter::xFilters::Grayscale(inData, outData);
			filter::xFilters::GrayscaleMultiThread(inData, outData);
			break;
		case filter::FilterNames::_BlackAndWhite:
		{
			//min = 0 and max = 255 positive number
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_BlackAndWhite, slider, 0, 255/*min no max = 255*/);
			//filter::xFilters::BlackAndWhite(inData, outData, x);

			filter::xFilters::BlackAndWhiteMultiThread(inData, outData, x);
			break;
		}
		case filter::FilterNames::_OnlyRed:
			//filter::xFilters::OnlyRed(inData, outData);
			filter::xFilters::OnlyRedMultiThread(inData, outData);
			break;
		case filter::FilterNames::_OnlyGreen:
			//filter::xFilters::OnlyGreen(inData, outData);
			filter::xFilters::OnlyGreenMultiThread(inData, outData);
			break;
		case filter::FilterNames::_OnlyBlue:
			//filter::xFilters::OnlyBlue(inData, outData);
			filter::xFilters::OnlyBlueMultiThread(inData, outData);
			break;
		case filter::FilterNames::_GraycaleNegative:
			//filter::xFilters::GraycaleNegative(inData, outData);
			filter::xFilters::GraycaleNegativeMultiThread(inData, outData);
			break;
		case filter::FilterNames::_Negative:
			//filter::xFilters::Negative(inData, outData);
			filter::xFilters::NegativeMultiThread(inData, outData);
			break;
		case filter::FilterNames::_B_W_Negative:
		{	// min = 0 max = 255 positive number
			int slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_B_W_Negative, slider, 0, 255 /*min no max = 255*/);
			//filter::xFilters::BlackAndWhiteNegative(inData, outData, x);
			filter::xFilters::BlackAndWhiteNegativeMultiThread(inData, outData, x);
			break;
		}
		case filter::FilterNames::_Contrast:
		{
			// min = 0 max = 255 positive number
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_Contrast, slider, 0, 255/*min no max = 255*/);
			//filter::xFilters::Contrast(inData, outData, x);
			filter::xFilters::ContrastMultiThread(inData, outData, x);
			break;
		}
		case filter::FilterNames::_Brightness:
		{
			// min = no value max = 255 positive number - 50 to 50 points
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_Brightness, slider, 0, 255/*min no max = 255*/);
			//filterfilter::xFilters::Brightness(inData, outData, (int)x);
			filter::xFilters::BrightnessMultiThread(inData, outData, (int)x);
			break;
		}
		case filter::FilterNames::_Gamma:
		{	// min = 100 for divide[0.99.... 0.1] points value max = 4 - 1[1....4] positive number no input number
			// not set min and max
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_Gamma, slider /*min [0.99...0.1] max [1....4]*/);
			//filter::xFilters::Gamma(inData, outData, x);
			filter::xFilters::GammaMultiThread(inData, outData, x);
			break;
		}
		case filter::FilterNames::_GaussianBlurClassic:
		{	// min value = 3,positive primen number, max = 15
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_GaussianBlurClassic, slider, 3, 49/*min max kernel size*/);
			filter::xFilters::GaussianBlurRealClassic(inData, outData, (int)x);
			break;
		}
		case filter::FilterNames::_RecursiveGaussianBlurVliet:
		{	// min = 0.5 and max = infiniti positive nunmber
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_RecursiveGaussianBlurVliet, slider, 0.5/*min sigma*/, 31/*max sigma*/);
			filter::xFilters::RecursiveGaussianBlurVliet(inData, outData, x);
			break;
		}
		case filter::FilterNames::_GaussianBlurSeperableConvolutionOpencvKernel:
		{	// min value = positive prime number, max = 2 * 100 + (min - 2) //  infiniti
			slider = _genericSlider0.GetPos();
			if (slider == 0) {
				filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
				break;
			}
			x = filter::calculateSliderValueForCurrentFilter(filter::FilterNames::_GaussianBlurSeperableConvolutionOpencvKernel, slider, 3/*min value*/, 111/*max value*/);
			filter::xFilters::GaussianBlurSeperableConvolution(inData, outData, (int)x, 0/*sigma calculate for size kernel*/);
			break;
		}
		case filter::FilterNames::_HSVColorSpace:
		{
			filter::xFilters::GeenericColorSpace2BGR(inData, outData, filter::xFilters::RGB2HSV, filter::xFilters::HSV2RGB, filter::HSVLSlider, _genericSlider0.GetPos(), _genericSlider1.GetPos(), _genericSlider2.GetPos());
			break;
		}
		case filter::FilterNames::_HSLColorSpace:
		{
			filter::xFilters::GeenericColorSpace2BGR(inData, outData, filter::xFilters::RGB2HSL, filter::xFilters::HSL2RGB, filter::HSVLSlider, _genericSlider0.GetPos(), _genericSlider1.GetPos(), _genericSlider2.GetPos());

			break;
		}
		case filter::FilterNames::_YCbCrColorSpace:
		{
			filter::xFilters::GeenericColorSpace2BGR(inData, outData, filter::xFilters::RGB2YCbCr, filter::xFilters::YCbCr2RGB, filter::YCbCrYUVLSlider, _genericSlider0.GetPos(), _genericSlider1.GetPos(), _genericSlider2.GetPos());
			break;
		}
		case filter::FilterNames::_YUVColorSpace:
		{
			filter::xFilters::GeenericColorSpace2BGR(inData, outData, filter::xFilters::RGB2YUV, filter::xFilters::YUV2RGB, filter::YCbCrYUVLSlider, _genericSlider0.GetPos(), _genericSlider1.GetPos(), _genericSlider2.GetPos());


			break;
		}
		case filter::FilterNames::_SobelEdgeDetection:
		{
			filter::xFilters::Sobel(inData, outData, _genericSlider0.GetPos() % 2 == 0 ? _genericSlider0.GetPos() + 1 : _genericSlider0.GetPos());

			break;
		}
		case filter::FilterNames::_PrewittEdgeDetection:
		{
			filter::xFilters::Prewitt(inData, outData, _genericSlider0.GetPos() % 2 == 0 ? _genericSlider0.GetPos() + 1 : _genericSlider0.GetPos());

			break;
		}
		case filter::FilterNames::_CannyEdgeDetection:
		{
			filter::xFilters::Canny(inData, outData);
			break;
		}
		case filter::FilterNames::_test:
			filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
			break;
		case filter::FilterNames::_None:
			//filter::xFilters::CopyOriginalToTransformedImage(inData, outData);
			break;
		}
		auto endTime = std::chrono::high_resolution_clock::now();

		EndWaitCursor();

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);

		CString str{};
		str.Format(L"Processing Time %d ms", ms);
		_showProcessingTime.SetWindowText(str);
		//_changeContrastBrightness.ShowWindow(SW_HIDE);


		// For Device Memory Context ////
		_imageViewer.GetTransformedDMCRef()->UpdateContext();
		//////

		CRect imgREct;
		_imageViewer.GetWindowRect(&imgREct);
		ScreenToClient(&imgREct);
		InvalidateRect(&imgREct);

	}
}

void CTestMFC1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (pScrollBar == (CScrollBar*)&_genericSlider0) {

		_genericSlider0.ReflectedScrollMessage();
	}
	if (pScrollBar == (CScrollBar*)&_genericSlider1) {

		_genericSlider1.ReflectedScrollMessage();
	}
	if (pScrollBar == (CScrollBar*)&_genericSlider2) {

		_genericSlider2.ReflectedScrollMessage();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CTestMFC1Dlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	if (_isImageOpened) {

		CPoint tmp = pt;
		::ScreenToClient(_imageViewer.GetSafeHwnd(), &tmp);
		RECT stViewerClient = { 0,0,0,0 };

		_imageViewer.GetWindowRect(&stViewerClient);
		_imageViewer.ScreenToClient(&stViewerClient);

		if (::PtInRect(&stViewerClient, tmp)) {

			//TRACE("\nzf %f\n", _imageViewer.GetCurrentPercent());

			float zoomfactor = zDelta > 0 ? 1.1f : 0.9f;

			auto oldZoomFactor = _imageViewer.GetCurrentPercent();

			float currentZoomfactor = oldZoomFactor;

			currentZoomfactor *= zoomfactor;

			float mouseX = tmp.x / oldZoomFactor;
			float mouseY = tmp.y / oldZoomFactor;


			TRACE("\n x%d y%d \n", tmp.x, tmp.y);
		}

	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


