#pragma once



// AddStationDlg form view

class AddStationDlg : public CFormView
{
	DECLARE_DYNCREATE(AddStationDlg)

public:
	AddStationDlg();           
	virtual ~AddStationDlg();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDSTATIONDLG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	std::vector<MediaItem> Media;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddmedia();
	afx_msg void OnBnClickedRemovemedia();

	void UpdatePlaylist();
	CListBox Playlist;
};


