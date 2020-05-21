
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
END_MESSAGE_MAP()

// CStreamOVisionView construction/destruction

CStreamOVisionView::CStreamOVisionView() noexcept
	: CFormView(IDD_STREAMOVISION_FORM)
{
	this->m_database.OpenDatabase();
	this->m_database.LoadAll();
	Stations = this->m_database.GetStationMap();
	
}

CStreamOVisionView::~CStreamOVisionView()
{
	this->m_database.CloseDatabase();
	for (auto station = Stations.begin(); station != Stations.end(); ++station) {
		libvlc_release(station->vlcInstance);
	}
	
}

void CStreamOVisionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYLIST, PlaylistContents);
	DDX_Control(pDX, IDC_STATIONLIST, StationList);
}

BOOL CStreamOVisionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CStreamOVisionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

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

	return nstringw;
}


// CStreamOVisionView message handlers


void CStreamOVisionView::UpdatePlaylistContents() {
	PlaylistContents.ResetContent();
	int index = StationList.GetCurSel();
	for (auto media = Stations[index].Media.begin(); media != this->Stations[index].Media.end(); ++media) {
		PlaylistContents.AddString(media->Filename);
	}
}

void CStreamOVisionView::OnBnClickedPlay()
{
	if (StationList.GetCurSel() == LB_ERR) {
		StationList.SetCurSel(0);
		OnLbnSelchangeStationlist(); 
	}
	if (PlaylistContents.GetCurSel() == LB_ERR) {
		PlaylistContents.SetCurSel(0);
		OnLbnSelchangePlaylist();
	}
	int stationIndex = StationList.GetCurSel(); 

	Stations[stationIndex].vlcPlayer = libvlc_media_player_new_from_media(Stations[stationIndex].vlcMedia);

	libvlc_media_release(Stations[stationIndex].vlcMedia);
	libvlc_media_player_play(Stations[stationIndex].vlcPlayer);
}


void CStreamOVisionView::OnLbnSelchangePlaylist()
{
	int stationIndex = StationList.GetCurSel();
	CString cStrVid = Stations[stationIndex].Media[PlaylistContents.GetCurSel()].Path;
	char* strVid = ConvertCStringtoStr(cStrVid.GetString());
	Stations[stationIndex].vlcMedia = libvlc_media_new_path(Stations[stationIndex].vlcInstance, strVid);
}


void CStreamOVisionView::OnBnClickedAddstation()
{
	AddStationDialog newStationDlg;
	if (newStationDlg.DoModal() == IDOK) {
		Station newStation = Station(); 
		newStation.StationId = newStationDlg.GetStationId();
		newStation.StationName = newStationDlg.GetStationName();
		newStation.vlcInstance = libvlc_new(0, NULL);
		Stations.push_back(newStation);
		UpdateStations();
	}

}

void CStreamOVisionView::OnBnClickedDeletestation()
{
	if (StationList.GetCurSel() != LB_ERR) {
		Stations.erase(StationList.GetCurSel() + Stations.begin());
		UpdateStations();
	}
}

void CStreamOVisionView::UpdateStations() {
	StationList.ResetContent();
	for (auto i = this->Stations.begin(); i != this->Stations.end(); ++i) {
		StationList.AddString(i->StationName + " (" + i->StationId + ")");
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
		Stations[index].Media.push_back(MediaItem(browseDlg.GetPathName(), browseDlg.GetFileName()));
		UpdatePlaylistContents();
	}
}


void CStreamOVisionView::OnBnClickedDeletemedia()
{
	int index = PlaylistContents.GetCurSel();
	Stations[StationList.GetCurSel()].Media.erase(Stations[StationList.GetCurSel()].Media.begin() + index);
	UpdatePlaylistContents();
}


void CStreamOVisionView::OnBnClickedStop()
{
	libvlc_media_player_stop(Stations[StationList.GetCurSel()].vlcPlayer);
	libvlc_media_player_release(Stations[StationList.GetCurSel()].vlcPlayer);
}





void CStreamOVisionView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
}


void CStreamOVisionView::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CFormView::OnWindowPosChanged(lpwndpos);

}
