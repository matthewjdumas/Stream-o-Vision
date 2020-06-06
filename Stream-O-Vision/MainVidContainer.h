#pragma once
#include <vlcpp/vlc.hpp>


// MainVidContainer dialog

class MainVidContainer : public CDialog
{
	DECLARE_DYNAMIC(MainVidContainer)

public:
	MainVidContainer(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainVidContainer();
	void StopPlayer();
	void PlayVideo();
	void ClosePlayer();
	void SetMediaFile(char*);
	void SetParentHwnd(HWND);
	void SetIp(std::string);
	void SetPort(unsigned int);
	void SetStationName(std::string);
	void SetLocalOutput(BOOL);
	void SetRenderer(const VLC::RendererDiscoverer::Item&&);


	VLC::Instance VlcInstance;
	VLC::MediaPlayer VlcPlayer;
	VLC::Media VlcMedia;
	VLC::MediaPlayerEventManager* VlcMediaPlayerEventMgr;
	HWND parentHwnd;
	FILE* logFile;
	std::string stationName; 
	std::string ipAddress; 
	unsigned int port;
	BOOL showLocal{ FALSE };


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
