#pragma once


// BroadcastSettingsDlg dialog

class BroadcastSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(BroadcastSettingsDlg)

public:
	BroadcastSettingsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~BroadcastSettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BCASTSETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	char* ConvertCStringtoStr(CString);
	unsigned int GetWidth();
	unsigned int GetHeight();
	CEdit txtWidth;
	CEdit txtHeight;
	CString txtWidVal;
	CString txtHeightVal;
	afx_msg void OnEnChangeHeight();
};
