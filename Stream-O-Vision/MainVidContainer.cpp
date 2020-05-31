// MainVidContainer.cpp : implementation file
//

#include "pch.h"
#include "Stream-O-Vision.h"
#include "MainVidContainer.h"
#include "afxdialogex.h"
#include <vlcpp/vlc.hpp>

// MainVidContainer dialog

IMPLEMENT_DYNAMIC(MainVidContainer, CDialog)

MainVidContainer::MainVidContainer(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAINVID, pParent)
{
	
	logFile = fopen("vlc_log.txt", "w");
	VlcInstance = VLC::Instance(0, nullptr);
	VlcInstance.logSetFile(logFile);

}

MainVidContainer::~MainVidContainer()
{
	fclose(logFile);
}

void MainVidContainer::StopPlayer()
{
	VlcPlayer.stop();
}

void MainVidContainer::PlayVideo() {
	VlcPlayer.setHwnd(this->GetSafeHwnd());
	VlcPlayer.play();
}

void MainVidContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void MainVidContainer::ClosePlayer() {
	OnClose();
}

void MainVidContainer::SetIp(std::string ip) {
	ipAddress = ip;
}

void MainVidContainer::SetPort(unsigned int p) {
	port = p;
}

void MainVidContainer::SetStationName(std::string s) {
	stationName = s;
}

void MainVidContainer::SetMediaFile(char* path)
{

	if (VlcPlayer.isValid()) {
		if (VlcPlayer.isPlaying()) {
			VlcPlayer.stop();
			VlcPlayer = VLC::MediaPlayer();
		}
	}
	
	VlcMedia = VLC::Media(VlcInstance, path, VLC::Media::FromPath);
	std::string soutCmd = ":sout=#rtp{dst="+ipAddress+",port=" + std::to_string(port) + ",mux=ts,sap,name="+stationName+"}";

	VlcMedia.addOption(soutCmd);
	VlcPlayer = VLC::MediaPlayer(VlcMedia);
	VlcMediaPlayerEventMgr = &VlcPlayer.eventManager();
	auto vlcEndFunc = [this]() -> void {
		::PostMessage(this->parentHwnd, WM_PLAYNEXT, 0, 0);
	};

	VlcMediaPlayerEventMgr->onEndReached(vlcEndFunc);
	
}

void MainVidContainer::SetParentHwnd(HWND h)
{
	parentHwnd = h;
}

BEGIN_MESSAGE_MAP(MainVidContainer, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// MainVidContainer message handlers


void MainVidContainer::OnClose()
{
	VlcPlayer.stop();
	CDialog::OnClose();
}
