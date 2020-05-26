// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

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
		this->Path = path; 
		this->Filename = filename;
		this->dbPlaylistId = dbId;
	}

	MediaItem() {
		this->Filename = "";
		this->Path = "";
		this->dbPlaylistId = -1;
	}
};

struct Station {
	std::vector<MediaItem> Media; 
	CString StationName; 
	CString StationId; 
	int dbStationId;
	unsigned int MediaCurrentIndex; 
	VLC::Instance vlcInstance;
	VLC::MediaPlayer vlcPlayer;
	VLC::Media vlcMedia;

	Station() {
		this->dbStationId = -1;
		this->StationId = "";
		this->StationName = "";
		this->MediaCurrentIndex = 0;
		this->vlcInstance = VLC::Instance(0, nullptr);       
	}
};


#endif //PCH_H
