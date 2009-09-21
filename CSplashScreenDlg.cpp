#include "CSplashScreenDlg.h"

BEGIN_MESSAGE_MAP(CSplashScreenDlg, CDialog)
//{{AFX_MSG_MAP(CSplashScreenDlg)
	ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Default constructor. Loads the splash screen from resources.
/// 
/// @author	Clark Gaebel
/// @date	8/10/2009
/// 
/// @param	resourceID	Identifier of the resource. 
////////////////////////////////////////////////////////////////////////////////////////////////////
CSplashScreenDlg::CSplashScreenDlg(UINT splashImgID, UINT _dialogID)
{
	splashImage.LoadBitmapA(splashImgID);
	dialogID = _dialogID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Draws the splash screen onto the background of the pseudo-dialog.
/// 
/// @author	Clark Gaebel
/// @date	8/10/2009
/// 
/// @param	pDC	The device-context. 
/// 
/// @return	CDialog::OnEraseBkgnd's return value.
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSplashScreenDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRet = CDialog::OnEraseBkgnd(pDC);

	// let the things happen after this call else it will erase everything

	BITMAP bmp;
	splashImage.GetBitmap(&bmp);

	// create memory DC  
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);
	memDc.SelectObject(&splashImage);

	// drawing into the dialog  
	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &memDc, 0, 0, SRCCOPY);  
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Shows the splash screen.
/// 
/// @author	Clark Gaebel
/// @date	8/10/2009
////////////////////////////////////////////////////////////////////////////////////////////////////
void CSplashScreenDlg::Show()
{
	Create(dialogID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Fades the splash screen out.
/// 
/// @author	Clark Gaebel
/// @date	8/10/2009
/// 
/// @param	fadeTimeInMilliseconds	The length of time until the splash screen fully dissapears.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CSplashScreenDlg::Hide(unsigned int fadeTimeInMilliseconds)
{
	AnimateWindow(fadeTimeInMilliseconds, AW_BLEND | AW_HIDE);
	DestroyWindow();
}