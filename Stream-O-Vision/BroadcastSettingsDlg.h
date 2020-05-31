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
	unsigned int GetPort();

	void SetWidth(unsigned int);
	void SetHeight(unsigned int);
	void SetPort(unsigned int);
	void SetIpAddress(std::string);

	std::string GetIpAddress();
	CEdit txtWidth;
	CEdit txtHeight;
	CString txtWidVal;
	CString txtHeightVal;
	afx_msg void OnEnChangeHeight();
	CString ipAddress;
	CString port;
	afx_msg void OnBnClickedOk();
};
