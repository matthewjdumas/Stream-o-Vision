#pragma once

#include <sqlite3.h>
#include <string>

class SqliteHandler
{
	private: 
		sqlite3* m_sqliteDb;
		int m_sqliteReturnCode;
		char* m_lastError;
		std::vector<Station> m_stations;

	public:
		int OpenDatabase();
		void CloseDatabase(); 

		BOOL LoadAll();
		std::vector<Station> GetStationMap();
		BOOL GetPlaylist(int64_t,int);

		int64_t AddStation(std::string, std::string);
		BOOL DeleteStation(int64_t);
		BOOL DeletePlaylistByStationId(int64_t);
		BOOL DeletePlaylistItemById(int64_t);
		int64_t AddPlaylistItem(std::string, std::string, int64_t);

		BOOL CreateTables();

		SqliteHandler() {
			m_sqliteDb = NULL;
			m_sqliteReturnCode = 0;
			m_lastError = "";
		}

};

