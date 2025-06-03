// xCSliderCtrl.cpp : implementation file
//

#include "pch.h"
#include "TestMFC_1.h"
#include "xCSliderCtrl.h"

// xCSliderCtrl

IMPLEMENT_DYNAMIC(xCSliderCtrl, CSliderCtrl)

xCSliderCtrl::xCSliderCtrl()
{

}

xCSliderCtrl::~xCSliderCtrl()
{
}


HWND xCSliderCtrl::CreateBuddySliderAndPos(CWnd* pDlgMin, CWnd* pDlgMiddle, CWnd* pDlgMax, CWnd* pDlgCurr, CWnd* pDlgName, CPoint drawPoint)
{
	int x = drawPoint.x;
	int y = drawPoint.y;

	_pBuddyWndMinPos = pDlgMin;
	_pBuddyWndMidlePos = pDlgMiddle;
	_pBuddyWndMaxPos = pDlgMax;
	_pBuddyWndCurrentPos = pDlgCurr;
	_pBuddyName = pDlgName;

	CRect Sr;

	this->GetWindowRect(&Sr);
	this->GetParent()->ScreenToClient(&Sr);



	this->SetWindowPos(nullptr, x, y, Sr.Width(), Sr.Height(), SWP_SHOWWINDOW);

	ASSERT(::IsWindow(_pBuddyWndCurrentPos->m_hWnd));
	_pBuddyWndCurrentPos->SetOwner(this);
	_pBuddyWndCurrentPos->ShowWindow(SW_SHOW);

	ASSERT(::IsWindow(_pBuddyWndMinPos->m_hWnd));
	_pBuddyWndMinPos->SetOwner(this);
	_pBuddyWndMinPos->ShowWindow(SW_SHOW);

	ASSERT(::IsWindow(_pBuddyWndMidlePos->m_hWnd));
	_pBuddyWndMidlePos->SetOwner(this);
	_pBuddyWndMidlePos->ShowWindow(SW_SHOW);

	ASSERT(::IsWindow(_pBuddyWndMaxPos->m_hWnd));
	_pBuddyWndMaxPos->SetOwner(this);
	_pBuddyWndMaxPos->ShowWindow(SW_SHOW);

	ASSERT(::IsWindow(_pBuddyName->m_hWnd));
	_pBuddyName->SetOwner(this);
	_pBuddyName->ShowWindow(SW_SHOW);


	TRACE(L"\n\nSx %d Sy %d Sw %d Sh %d\n\n", Sr.left, Sr.top, Sr.Width(), Sr.Height());


	UINT check = SWP_SHOWWINDOW | SWP_HIDEWINDOW;

	HWND hWndMin = (HWND)SendMessage(TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)_pBuddyWndMinPos->m_hWnd);
	_pBuddyWndMinPos->SetWindowPos(nullptr, x, y + 28, 25, 20, check);

	HWND hWndMidle = (HWND)SendMessage(TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)_pBuddyWndMidlePos->m_hWnd);
	_pBuddyWndMidlePos->SetWindowPos(nullptr, x + 70, y + 28, 25, 20, check);

	HWND hWndMax = (HWND)SendMessage(TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)_pBuddyWndMaxPos->m_hWnd);
	_pBuddyWndMaxPos->SetWindowPos(nullptr, x + 135, y + 28, 25, 20, check);


	HWND hWndCurr = (HWND)SendMessage(TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)_pBuddyWndCurrentPos->m_hWnd);
	_pBuddyWndCurrentPos->SetWindowPos(nullptr, x + 135, y - 20, 25, 20, check);

	HWND hWndName = (HWND)SendMessage(TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)_pBuddyName->m_hWnd);
	_pBuddyName->SetWindowPos(nullptr, x, y - 20, 50, 20, check);


	CString text;
	text.Format(L"%d", GetRangeMin());
	_pBuddyWndMinPos->SetWindowTextW(text);

	text.Format(L"%d", GetRangeMax());
	_pBuddyWndMaxPos->SetWindowTextW(text);

	text.Format(L"%d", _middleValue);
	_pBuddyWndMidlePos->SetWindowTextW(text);

	text.Format(L"");
	_pBuddyName->SetWindowTextW(text);

	ReflectedScrollMessage();
	return NULL;
}


void xCSliderCtrl::ReflectedScrollMessage()
{
	// update buddy's text, if there is a buddy
	if (_pBuddyWndCurrentPos)
	{
		int iPos = GetPos();
		CString text;
		text.Format(L"%d", iPos);
		_pBuddyWndCurrentPos->SetWindowText(text);

	}
}


void xCSliderCtrl::SetPos(int nPos)
{
	CSliderCtrl::SetPos(nPos);

	ReflectedScrollMessage();
}


void xCSliderCtrl::SetMiddle(int nPos) {
	if (nPos > GetRangeMin() && nPos < GetRangeMax()) {
		_middleValue = nPos;
	}
}


void xCSliderCtrl::SetName(const wchar_t* n) {

	_name.Format(L"%s", n);
	_pBuddyName->SetWindowTextW(_name);
}


void xCSliderCtrl::ShowWindowXCslider(bool ch) {

	_pBuddyWndMinPos->ShowWindow(ch);
	_pBuddyWndMidlePos->ShowWindow(ch);
	_pBuddyWndMaxPos->ShowWindow(ch);
	_pBuddyWndCurrentPos->ShowWindow(ch);
	_pBuddyName->ShowWindow(ch);
	this->ShowWindow(ch);
	
	if (!ch) {
		_name = L"";
	}


	if (ch) {
		CString text;
		text.Format(L"%d", GetRangeMin());
		_pBuddyWndMinPos->SetWindowTextW(text);

		text.Format(L"%d", GetRangeMax());
		_pBuddyWndMaxPos->SetWindowTextW(text);

		text.Format(L"%d", _middleValue);
		_pBuddyWndMidlePos->SetWindowTextW(text);	

		text.Format(L"%s", _name);
		_pBuddyName->SetWindowTextW(text);

	}

}



BEGIN_MESSAGE_MAP(xCSliderCtrl, CSliderCtrl)

END_MESSAGE_MAP()



// xCSliderCtrl message handlers


