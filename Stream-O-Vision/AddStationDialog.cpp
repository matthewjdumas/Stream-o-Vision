// AddStationDialog.cpp : implementation file
//

#include "pch.h"
#include "Stream-O-Vision.h"
#include "AddStationDialog.h"
#include "afxdialogex.h"


// AddStationDialog dialog

IMPLEMENT_DYNAMIC(AddStationDialog, CDialog)

AddStationDialog::AddStationDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

AddStationDialog::~AddStationDialog()
{

}

void AddStationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIONID, txtStationId);
	DDX_Control(pDX, IDC_STATIONNAME, txtStationName);
}


BEGIN_MESSAGE_MAP(AddStationDialog, CDialog)
	ON_BN_CLICKED(IDOK, &AddStationDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_STATIONID, &AddStationDialog::OnEnChangeStationid)
END_MESSAGE_MAP()


// AddStationDialog message handlers
BOOL AddStationDialog::OnInitDialog() {
	CDialog::OnInitDialog(); 
	SetDlgItemText(IDC_STATIONID, m_StationId);
	SetDlgItemText(IDC_STATIONNAME, m_StationName);
	return TRUE;
}

BOOL AddStationDialog::DestroyWindow() {
	GetDlgItemText(IDC_STATIONID, m_StationId);
	GetDlgItemText(IDC_STATIONNAME, m_StationName);
	return CDialog::DestroyWindow();
}



CString AddStationDialog::GetStationId() {
	return m_StationId;
}

CString AddStationDialog::GetStationName() {
	return m_StationName;
}


void AddStationDialog::OnBnClickedOk()
{
	CDialog::OnOK();
}


void AddStationDialog::OnEnChangeStationid()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
