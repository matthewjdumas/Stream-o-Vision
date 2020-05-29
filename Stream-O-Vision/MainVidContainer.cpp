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
	VlcInstance = VLC::Instance(0, nullptr);

}

MainVidContainer::~MainVidContainer()
{
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

void MainVidContainer::SetMediaFile(char* path)
{

	if (VlcPlayer.isValid()) {
		if (VlcPlayer.isPlaying()) {
			VlcPlayer.stop();
			delete VlcPlayer;
		}
	}
	
	VlcMedia = VLC::Media(VlcInstance, path, VLC::Media::FromPath);
	VlcPlayer = VLC::MediaPlayer(VlcMedia);
	VlcMediaPlayerEventMgr = &VlcPlayer.eventManager();
	auto vlcEndFunc = [this]() -> void {
		//::SendMessage(this->parentHwnd, WM_PLAYNEXT, 0, 0);
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
