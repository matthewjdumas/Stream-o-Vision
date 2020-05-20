#pragma once


// NewStationDlg dialog

class NewStationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewStationDlg)

public:
	NewStationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NewStationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSTATIONDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddmedia();
	afx_msg void OnBnClickedRemovemedia();
	CListBox Playlist;
	void UpdatePlaylist();
	std::vector<MediaItem> Media;
};
