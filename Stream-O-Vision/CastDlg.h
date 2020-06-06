#pragma once


// CastDlg dialog

class CastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CastDlg)

public:
	CastDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CastDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CASTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox ctrlRenderItems;
	CString valRenderItem;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnscan();

	char* ConvertCStringtoStr(CString);
	void SetStream(char*);

	LPCTSTR ConvertStrtoLPCTSTR(std::string);

	VLC::Instance VlcInstance{ 0, nullptr };
	VLC::RendererDiscoverer VlcRendererDiscoverer{ VlcInstance, "microdns" };

	std::vector<VLC::RendererDiscoverer::Item> RendererItems;
	VLC::MediaPlayer VlcPlayer;

	char* streamLocation;
};
