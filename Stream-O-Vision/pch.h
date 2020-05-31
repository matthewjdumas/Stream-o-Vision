// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define WM_PLAYNEXT WM_USER+1
#define WM_STOP WM_USER+2

#define sleep(x) Sleep(1000 * (x))

typedef long int ssize_t;   // Need this for libvlcpp bindings to work right for some reason

// add headers that you want to pre-compile here
#include "framework.h"
#include <vlcpp/vlc.hpp>
#include <vector>

struct MediaItem {
	CString Path; 
	CString Filename;
	int dbPlaylistId;

	MediaItem(CString path, CString filename, int dbId) {
		Path = path; 
		Filename = filename;
		dbPlaylistId = dbId;
	}

	MediaItem() {
		Filename = "";
		Path = "";
		dbPlaylistId = -1;
	}
};

struct Station {
	std::vector<MediaItem> Media; 
	CString StationName; 
	CString StationId; 
	int dbStationId;
	unsigned int MediaCurrentIndex; 

	Station() {
		dbStationId = -1;
		StationId = "";
		StationName = "";
		MediaCurrentIndex = 0;
	}
};

struct BroadcastViewerSettings {
	unsigned int Height, Width, Port;
	std::string IpAddress;
	BOOL LocalPlay;

	BroadcastViewerSettings() {
		Height = 480;
		Width = 640;
		Port = 5004;
		IpAddress = "";
		LocalPlay = FALSE;
	}
	BroadcastViewerSettings(unsigned int h, unsigned int w, unsigned int p, std::string ip, BOOL l) {
		Height = h;
		Width = w;
		Port = p;
		IpAddress = ip;
		LocalPlay = l;
	}

};

#endif //PCH_H
