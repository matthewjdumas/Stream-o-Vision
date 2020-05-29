
// Stream-O-VisionView.h : interface of the CStreamOVisionView class
//

#pragma once
#include "SqliteHandler.h"
#include "MainVidContainer.h"

#define WM_PLAYNEXT WM_USER+1
#define WM_STOP WM_USER+2

class CStreamOVisionView : public CFormView
{
protected: // create from serialization only
	CStreamOVisionView() noexcept;
	DECLARE_DYNCREATE(CStreamOVisionView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_STREAMOVISION_FORM };
#endif

// Attributes
public:
	CStreamOVisionDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct


// Implementation
public:
	virtual ~CStreamOVisionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	std::vector<Station> Stations;
	SqliteHandler Database;
	char* ConvertCStringtoStr(CString input);
	void UpdatePlaylistContents();
	void UpdateStations();
	std::string CStringToStdString(CString);
	void HandlePlay();

	afx_msg void OnBnClickedPlay();
	afx_msg void OnLbnSelchangePlaylist();
	afx_msg void OnLbnSelchangeStationlist();
	afx_msg void OnBnClickedAddstation();
	afx_msg void OnBnClickedAddmedia();
	afx_msg void OnBnClickedDeletemedia();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedDeletestation();
	afx_msg void OnBnClickedBcastsett();

	CListBox StationList;
	CListBox PlaylistContents; // variable to hold the contents of the playlist
	MainVidContainer MainVidCont;
	BroadcastViewerSettings ViewerSettings; 
	

protected:
	afx_msg LRESULT OnPlaynext(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStop(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // debug version in Stream-O-VisionView.cpp
inline CStreamOVisionDoc* CStreamOVisionView::GetDocument() const
   { return reinterpret_cast<CStreamOVisionDoc*>(m_pDocument); }
#endif

