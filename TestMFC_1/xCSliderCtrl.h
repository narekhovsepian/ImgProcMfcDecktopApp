#pragma once


// xCSliderCtrl

class xCSliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(xCSliderCtrl)

public:
	xCSliderCtrl();
	virtual ~xCSliderCtrl();


	void SetPos(int nPos);
	void SetMiddle(int nPos);
	void SetName(const wchar_t*);
	void ReflectedScrollMessage();
	void ShowWindowXCslider(bool ch);

	// GetDlgItem(IDC_STATICXMINVALUE)    Show Min value of slider
	// GetDlgItem(IDC_STATICXMIDDLEVALUE)  Show Middle Value of slider
	// GetDlgItem(IDC_STATICXMAXVALUE)	   Show Max value of Slider
	// GetDlgItem(IDC_STATICXCURRVALUE)	   Show Curr Value of Slider
	// GetDlgItem(IDC_STATICSLIDERNAME)	   Show Name of Slider
	// x and y draw slider and Cstatic values 
	HWND CreateBuddySliderAndPos(CWnd* pDlgMin, CWnd* pDlgMiddle, CWnd* pDlgMax, CWnd* pDlgCurr, CWnd* pDlgName, CPoint drawpoint);

private:
	int _middleValue{  };
	CWnd* _pBuddyWndCurrentPos;
	CWnd* _pBuddyWndMinPos;
	CWnd* _pBuddyWndMaxPos;
	CWnd* _pBuddyWndMidlePos;
	CWnd* _pBuddyName;
	CPoint _drawCordinate;
	CString _name;
protected:
	DECLARE_MESSAGE_MAP()
};

