#pragma once

#include <sqlite3.h>

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
		BOOL GetPlaylist(int,int);
		//void GetStation();
		//void AddStation(); 
		//void DeleteStation();

		//void GetSchedule(); 
		//void UpdateSchedule(); 
		//void DeleteSchedule();
		//void CreateSchedule(); 

};

