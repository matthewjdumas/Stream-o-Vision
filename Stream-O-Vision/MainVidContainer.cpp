// MainVidContainer.cpp : implementation file
//

#include "pch.h"
#include "Stream-O-Vision.h"
#include "MainVidContainer.h"
#include "afxdialogex.h"


// MainVidContainer dialog

IMPLEMENT_DYNAMIC(MainVidContainer, CDialog)

MainVidContainer::MainVidContainer(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAINVID, pParent)
{
	MediaPlayer = nullptr;
}

MainVidContainer::~MainVidContainer()
{
	MediaPlayer->stop();
}

void MainVidContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void MainVidContainer::SetMediaPlayer(VLC::MediaPlayer* mp) {
	MediaPlayer = mp;
}

BEGIN_MESSAGE_MAP(MainVidContainer, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// MainVidContainer message handlers


void MainVidContainer::OnClose()
{
	MediaPlayer->stop();

	CDialog::OnClose();
}
