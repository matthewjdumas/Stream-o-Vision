#pragma once


// AddStationDialog dialog

class AddStationDialog : public CDialog
{
	DECLARE_DYNAMIC(AddStationDialog)
	CString m_StationId;
	CString m_StationName;

public:
	AddStationDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddStationDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	CListBox Playlist;

	CString GetStationId();
	CString GetStationName(); 
	CEdit txtStationId;
	CEdit txtStationName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeStationid();
};

