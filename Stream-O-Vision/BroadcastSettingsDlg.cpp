// BroadcastSettingsDlg.cpp : implementation file
//

#include "pch.h"
#include "Stream-O-Vision.h"
#include "BroadcastSettingsDlg.h"
#include "afxdialogex.h"



// BroadcastSettingsDlg dialog

IMPLEMENT_DYNAMIC(BroadcastSettingsDlg, CDialog)

BroadcastSettingsDlg::BroadcastSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BCASTSETTINGS, pParent)
	, txtWidVal(_T(""))
	, txtHeightVal(_T(""))
{

}

BroadcastSettingsDlg::~BroadcastSettingsDlg()
{
}

void BroadcastSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIDTH, txtWidth);
	DDX_Control(pDX, IDC_HEIGHT, txtHeight);
	DDX_Text(pDX, IDC_WIDTH, txtWidVal);
	DDX_Text(pDX, IDC_HEIGHT, txtHeightVal);
}


BEGIN_MESSAGE_MAP(BroadcastSettingsDlg, CDialog)
	ON_EN_CHANGE(IDC_HEIGHT, &BroadcastSettingsDlg::OnEnChangeHeight)
END_MESSAGE_MAP()


// BroadcastSettingsDlg message handlers


BOOL BroadcastSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	txtWidth.SetWindowTextW(_T("640"));
	txtHeight.SetWindowTextW(_T("480"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



unsigned int BroadcastSettingsDlg::GetWidth() {
	std::string strWid = ConvertCStringtoStr(txtWidVal);
	unsigned int ul = std::strtoul(strWid.c_str(), NULL, 0);
	return ul;
}

unsigned int BroadcastSettingsDlg::GetHeight() {
	std::string strHeight = ConvertCStringtoStr(txtHeightVal);
	unsigned int ul = std::strtoul(strHeight.c_str(), NULL, 0);
	return ul;
}
char* BroadcastSettingsDlg::ConvertCStringtoStr(CString input) {
	const size_t newsizew = (input.GetLength() + 1) * 2;
	char* nstringw = new char[newsizew];
	size_t convertedCharsw = 0;
	wcstombs_s(&convertedCharsw, nstringw, newsizew, input, _TRUNCATE);
	TRACE("Converted CString as Str: ", nstringw);
	return nstringw;
}

void BroadcastSettingsDlg::OnEnChangeHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}