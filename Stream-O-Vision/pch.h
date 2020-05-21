// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define sleep(x) Sleep(1000 * (x))

// add headers that you want to pre-compile here
#include "framework.h"
#include <vlc/vlc.h>
#include <vector>

struct MediaItem {
	CString Path; 
	CString Filename;

	MediaItem(CString path, CString filename) {
		this->Path = path; 
		this->Filename = filename;
	}

	MediaItem() {
		this->Filename = "";
		this->Path = "";
	}
};

struct Station {
	std::vector<MediaItem> Media; 
	CString StationName; 
	CString StationId; 
	int dbStationId;
	unsigned int MediaCurrentIndex; 
	libvlc_instance_t* vlcInstance;
	libvlc_media_player_t* vlcPlayer;
	libvlc_media_t* vlcMedia;

	Station() {
		this->dbStationId = -1;
		this->StationId = "";
		this->StationName = "";
		this->MediaCurrentIndex = 0;
		this->vlcInstance = libvlc_new(0, NULL);       // create vlc instance
	}
};


#endif //PCH_H
