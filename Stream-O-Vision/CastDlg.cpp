// CastDlg.cpp : implementation file
//

#include "pch.h"
#include "Stream-O-Vision.h"
#include "CastDlg.h"
#include "afxdialogex.h"
#include <codecvt>


// CastDlg dialog

IMPLEMENT_DYNAMIC(CastDlg, CDialogEx)

CastDlg::CastDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CASTDLG, pParent)
	, valRenderItem(_T(""))
{

}

CastDlg::~CastDlg()
{
}

void CastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RENDERITEMS, ctrlRenderItems);
	DDX_LBString(pDX, IDC_RENDERITEMS, valRenderItem);
}


BEGIN_MESSAGE_MAP(CastDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CastDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CastDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTNSCAN, &CastDlg::OnBnClickedBtnscan)
END_MESSAGE_MAP()


// CastDlg message handlers


void CastDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	 VlcInstance = VLC::Instance(0, nullptr);

	int i = ctrlRenderItems.GetCurSel();
	VLC::Media VlcMedia = VLC::Media(VlcInstance, streamLocation, VLC::Media::FromLocation);
	 VlcPlayer = VLC::MediaPlayer(VlcMedia);

	VlcPlayer.setRenderer(RendererItems[i]);  
	// TODO: Issue here. It only caches what has currently been sent so you won't get the full stream. Somehow need to change this into a loop such
	//        that it refreshes the cache every time the cache is close to empty. Not sure how to do that right now since that doesn't seem to be
	//        exposed via api
	VlcPlayer.play();
}


void CastDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CastDlg::OnBnClickedBtnscan()
{
	auto& em = VlcRendererDiscoverer.eventManager();
	em.onItemAdded([this](const VLC::RendererDiscoverer::Item& item) {
		if (item.canRenderVideo() && item.canRenderAudio()) {
			this->RendererItems.push_back(item);
		}

		});
	if (VlcRendererDiscoverer.start() == false)
		abort();

	std::this_thread::sleep_for(std::chrono::seconds{ 10 });

	for (VLC::RendererDiscoverer::Item i : RendererItems) {
		ctrlRenderItems.AddString(CA2T(i.name().c_str()));
	}
	
}

char* CastDlg::ConvertCStringtoStr(CString input) {
	const size_t newsizew = (input.GetLength() + 1) * 2;
	char* nstringw = new char[newsizew];
	size_t convertedCharsw = 0;
	wcstombs_s(&convertedCharsw, nstringw, newsizew, input, _TRUNCATE);
	TRACE("Converted CString as Str: ", nstringw);
	return nstringw;
}

void CastDlg::SetStream(char* loc)
{
	streamLocation = loc;
}

LPCTSTR CastDlg::ConvertStrtoLPCTSTR(std::string input) {
	return CA2T(input.c_str());
}