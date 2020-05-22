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
		BOOL GetPlaylist(int,int);

		int AddStation(std::string, std::string);
		BOOL DeleteStation(int);
		//void GetStation();
		//void AddStation(); 
		//void DeleteStation();

		//void GetSchedule(); 
		//void UpdateSchedule(); 
		//void DeleteSchedule();
		//void CreateSchedule(); 


		SqliteHandler() {
			m_sqliteDb = NULL;
			m_sqliteReturnCode = 0;
			m_lastError = "";
		}

};

