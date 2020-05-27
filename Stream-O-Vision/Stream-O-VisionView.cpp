
// Stream-O-VisionView.cpp : implementation of the CStreamOVisionView class
//


#include <stdio.h>
#include <stdlib.h>


#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Stream-O-Vision.h"
#endif

#include "Stream-O-VisionDoc.h"
#include "Stream-O-VisionView.h"
#include "AddStationDialog.h"
#include "BroadcastSettingsDlg.h"
#include "vlcpp/vlc.hpp"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStreamOVisionView

IMPLEMENT_DYNCREATE(CStreamOVisionView, CFormView)

BEGIN_MESSAGE_MAP(CStreamOVisionView, CFormView)
ON_BN_CLICKED(IDC_PLAY, &CStreamOVisionView::OnBnClickedPlay)
ON_LBN_SELCHANGE(IDC_PLAYLIST, &CStreamOVisionView::OnLbnSelchangePlaylist)
ON_LBN_SELCHANGE(IDC_STATIONLIST, &CStreamOVisionView::OnLbnSelchangeStationlist)
ON_BN_CLICKED(IDC_ADDSTATION, &CStreamOVisionView::OnBnClickedAddstation)
ON_BN_CLICKED(IDC_ADDMEDIA, &CStreamOVisionView::OnBnClickedAddmedia)
ON_BN_CLICKED(IDC_DELETEMEDIA, &CStreamOVisionView::OnBnClickedDeletemedia)
ON_BN_CLICKED(IDC_STOP, &CStreamOVisionView::OnBnClickedStop)
ON_BN_CLICKED(IDC_DELETESTATION, &CStreamOVisionView::OnBnClickedDeletestation)
ON_WM_WINDOWPOSCHANGED()
ON_BN_CLICKED(IDC_BCASTSETT, &CStreamOVisionView::OnBnClickedBcastsett)
END_MESSAGE_MAP()

// CStreamOVisionView construction/destruction

CStreamOVisionView::CStreamOVisionView() noexcept
	: CFormView(IDD_STREAMOVISION_FORM)
{
	int err = this->Database.OpenDatabase();
	TRACE("Database Open return value: ", err);
	this->Database.LoadAll();
	this->Stations = this->Database.GetStationMap();	
	this->ViewerSettings.Height = 480;
	this->ViewerSettings.Width = 640;
}

CStreamOVisionView::~CStreamOVisionView()
{
	this->Database.CloseDatabase();

}

void CStreamOVisionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYLIST, PlaylistContents);
	DDX_Control(pDX, IDC_STATIONLIST, StationList);
}

BOOL CStreamOVisionView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CStreamOVisionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	UpdateStations();

}


// CStreamOVisionView diagnostics

#ifdef _DEBUG
void CStreamOVisionView::AssertValid() const
{
	CFormView::AssertValid();
}

void CStreamOVisionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CStreamOVisionDoc* CStreamOVisionView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStreamOVisionDoc)));
	return (CStreamOVisionDoc*)m_pDocument;
}
#endif //_DEBUG

char* CStreamOVisionView::ConvertCStringtoStr(CString input) {
	const size_t newsizew = (input.GetLength() + 1) * 2;
	char* nstringw = new char[newsizew];
	size_t convertedCharsw = 0;
	wcstombs_s(&convertedCharsw, nstringw, newsizew, input, _TRUNCATE);
	TRACE("Converted CString as Str: ", nstringw);
	return nstringw;
}


// CStreamOVisionView message handlers


void CStreamOVisionView::UpdatePlaylistContents() {
	if (StationList.GetCurSel() != LB_ERR) {
		int index = StationList.GetCurSel();
		PlaylistContents.ResetContent();
		for (auto media = Stations[index].Media.begin(); media != Stations[index].Media.end(); ++media) {
			PlaylistContents.AddString(media->Filename);
		}
	}
}

void CStreamOVisionView::OnBnClickedPlay()
{	
	if (::IsWindow(MainVidCont.m_hWnd)) {
		MainVidCont.DestroyWindow();
	}
	if (StationList.GetCurSel() == LB_ERR) {
		StationList.SetCurSel(0);
		OnLbnSelchangeStationlist(); 
	}
	if (PlaylistContents.GetCurSel() == LB_ERR) {
		PlaylistContents.SetCurSel(0);
		OnLbnSelchangePlaylist();
	}
	int stationIndex = StationList.GetCurSel(); 
	Stations[stationIndex].vlcPlayer = VLC::MediaPlayer(Stations[stationIndex].vlcMedia);	

	MainVidCont.Create(IDD_MAINVID);
	CRect windowRect, thisRect; 
	MainVidCont.GetWindowRect(&windowRect);
	GetWindowRect(&thisRect);
	MainVidCont.MoveWindow(windowRect.left + thisRect.Width() +25, thisRect.top - 75, ViewerSettings.Width, ViewerSettings.Height);
	MainVidCont.ShowWindow(SW_SHOW);
	MainVidCont.SetMediaPlayer(&Stations[stationIndex].vlcPlayer);
	Stations[stationIndex].vlcPlayer.setHwnd(MainVidCont.GetSafeHwnd());
	Stations[stationIndex].vlcPlayer.play();
		
	//std::this_thread::sleep_for(std::chrono::seconds(10));   // how to do a sleep!

}


void CStreamOVisionView::OnLbnSelchangePlaylist()
{
	if (StationList.GetCurSel() != LB_ERR && PlaylistContents.GetCurSel() != LB_ERR) {
		int stationIndex = StationList.GetCurSel();
		CString cStrVid = Stations[stationIndex].Media[PlaylistContents.GetCurSel()].Path;
		char* strVid = ConvertCStringtoStr(cStrVid.GetString());
		Stations[stationIndex].vlcMedia = VLC::Media(Stations[stationIndex].vlcInstance, strVid, VLC::Media::FromPath);
	}
}


void CStreamOVisionView::OnBnClickedAddstation()
{
	AddStationDialog newStationDlg;
	if (newStationDlg.DoModal() == IDOK) {
		Station newStation = Station(); 
		newStation.StationId = newStationDlg.GetStationId();
		newStation.StationName = newStationDlg.GetStationName();
		newStation.vlcInstance = VLC::Instance(0, nullptr);
		int rowId = Database.AddStation(CStringToStdString(newStation.StationId), CStringToStdString(newStation.StationName));
		newStation.dbStationId = rowId;
		Stations.push_back(newStation);
		UpdateStations();
	}

}

void CStreamOVisionView::OnBnClickedDeletestation()
{
	if (StationList.GetCurSel() != LB_ERR) {
		int dbId = Stations[StationList.GetCurSel()].dbStationId; 
		Database.DeleteStation(dbId);
		Database.DeletePlaylistByStationId(dbId);
		Stations.erase(StationList.GetCurSel() + Stations.begin());
		UpdateStations();
		PlaylistContents.ResetContent();
	}
}

void CStreamOVisionView::UpdateStations() {
	StationList.ResetContent();
	for (auto station = Stations.begin(); station != Stations.end(); ++station) {
		StationList.AddString(station->StationName + " (" + station->StationId + ")");
	}
}

void CStreamOVisionView::OnLbnSelchangeStationlist()
{
	UpdatePlaylistContents();
}

void CStreamOVisionView::OnBnClickedAddmedia()
{
	// list taken from VLC
	CString typeFilter;
	typeFilter.Append(_T("Common media formats|*.avi;*.wmv;*.wmp;*.wm;*.asf;*.rm;*.ram;*.rmvb;*.ra;*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;"));
	typeFilter.Append(_T("*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.vob;*.ac3;*.dts;*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2;*.swf;*.ogg;"));
	typeFilter.Append(_T("*.mkv;*.ogm;*.m4b;*.tp;*.ts;*.tpr;*.pva;*.pss;*.wv;*.m2ts;*.evo;|"));
	typeFilter.Append(_T("Windows Media Video(*.avi;*wmv;*wmp;*wm;*asf)|*.avi;*.wmv;*.wmp;*.wm;*.asf|"));
	typeFilter.Append(_T("MPEG Video(*.mpg;*mpeg;*mpe;*m1v;*m2v;*mpv2;*mp2v;*dat;*mp4;*m4v;*m4p;*m4b;*ts;*tp;*tpr;*pva;*pss;*.wv;)|"));
	typeFilter.Append(_T("*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.m4b;*.ts;*.tp;*.tpr;*.pva;*.pss;*.wv;|"));
	typeFilter.Append(_T("DVD(*.vob;*ifo;*ac3;*dts)|*.vob;*.ifo;*.ac3;*.dts|MP3(*.mp3)|*.mp3|CD Tracks(*.cda)|*.cda|"));
	typeFilter.Append(_T("Quicktime(*.mov;*qt;*mr;*3gp;*3gpp;*3g2;*3gp2)|*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2|"));
	typeFilter.Append(_T("Flash Files(*.flv;*swf;*.f4v)|*.flv;*.swf;*.f4v|Playlist(*.smpl;*.asx;*m3u;*pls;*wvx;*wax;*wmx;*mpcpl)|*.smpl;*.asx;*.m3u;*.pls;*.wvx;*.wax;*.wmx;*.mpcpl|"));
	typeFilter.Append(_T("All Files(*.*)|*.*||"));

	CFileDialog browseDlg(TRUE, NULL, NULL, NULL, typeFilter);
	if (browseDlg.DoModal() == IDOK) {
		if (StationList.GetCurSel() == LB_ERR) {
			StationList.SetCurSel(0);
		}
		int index = StationList.GetCurSel();
		int dbPlaylistId = Database.AddPlaylistItem(CStringToStdString(browseDlg.GetFileName()), CStringToStdString(browseDlg.GetPathName()), Stations[index].dbStationId);
		Stations[index].Media.push_back(MediaItem(browseDlg.GetPathName(), browseDlg.GetFileName(),dbPlaylistId));
		UpdatePlaylistContents();
	}
}


void CStreamOVisionView::OnBnClickedDeletemedia()
{
	if (PlaylistContents.GetCurSel() != LB_ERR) {
		int index = PlaylistContents.GetCurSel();
		Database.DeletePlaylistItemById(Stations[StationList.GetCurSel()].Media[index].dbPlaylistId);
		Stations[StationList.GetCurSel()].Media.erase(Stations[StationList.GetCurSel()].Media.begin() + index);
		UpdatePlaylistContents();
	}
	
}


void CStreamOVisionView::OnBnClickedStop()
{
	if (StationList.GetCurSel() != LB_ERR) {
		Stations[StationList.GetCurSel()].vlcPlayer.stop();
		if (::IsWindow(MainVidCont.m_hWnd)) {
			MainVidCont.DestroyWindow();
		}
	}
}

std::string CStreamOVisionView::CStringToStdString(CString input) {
	CString cStr(input);
	CT2CA ctConvStr(cStr);
	std::string output(ctConvStr);
	return output;
}





void CStreamOVisionView::OnBnClickedBcastsett()
{
	BroadcastSettingsDlg bSett;
	if (bSett.DoModal() == IDOK) {
		unsigned int width = bSett.GetWidth();
		unsigned int height = bSett.GetHeight();
		ViewerSettings.Height = height;
		ViewerSettings.Width = width;
	}
}
