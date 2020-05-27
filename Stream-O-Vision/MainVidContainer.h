#pragma once
#include <vlcpp/vlc.hpp>

// MainVidContainer dialog

class MainVidContainer : public CDialog
{
	DECLARE_DYNAMIC(MainVidContainer)

public:
	MainVidContainer(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainVidContainer();
	void SetMediaPlayer(VLC::MediaPlayer*);
	VLC::MediaPlayer* MediaPlayer;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINVID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};
