#include "pch.h"
#include "SqliteHandler.h"
#include <stdlib.h>
#include <string>

#include <direct.h>
#define GetCurrentDir _getcwd



int SqliteHandler::OpenDatabase() {
	const char* cCurrentDir = "..\\Debug\\streamovision.db";  // TODO: Need to figure out how to do this without hardcoding paths

	return sqlite3_open(cCurrentDir,&this->m_sqliteDb);
}

void SqliteHandler::CloseDatabase() {
	sqlite3_close(this->m_sqliteDb);
}

BOOL SqliteHandler::LoadAll() {
	const char* query = "SELECT * FROM stations";
	int rows, cols;
	char** results; 
	char* err;
	this->m_sqliteReturnCode = sqlite3_get_table(this->m_sqliteDb, query, &results, &rows, &cols, &err);

	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		fprintf(stdout, "Can't open database: %s\n", sqlite3_errmsg(this->m_sqliteDb));
		sqlite3_free(err);
		return false;
	}

	for (int r = 1; r <= rows; ++r) {
		int pos = (r * cols);
		Station newStation = Station();
		char* dbTempId;
		newStation.dbStationId = strtol(results[0 + pos], &dbTempId,10);
		newStation.StationId = results[1 + pos];
		newStation.StationName = results[2 + pos]; 
		this->m_stations.push_back(newStation);
	}

	sqlite3_free_table(results);

	int index = 0;
	for (auto station = this->m_stations.begin(); station != this->m_stations.end(); ++station) {
		GetPlaylist(station->dbStationId,index);
		index++;
	}
}

std::vector<Station> SqliteHandler::GetStationMap() {
	return this->m_stations;
}

BOOL SqliteHandler::GetPlaylist(int id, int index) {
	std::string strId = std::to_string(id).c_str();
	std::string strQuery = "SELECT * FROM playlists WHERE fk_stations_id='" + strId + "';";
	const char* query = strQuery.c_str();

	int rows, cols;
	char** results;
	char* err;
	this->m_sqliteReturnCode = sqlite3_get_table(this->m_sqliteDb, query, &results, &rows, &cols, &err);

	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		fprintf(stdout, "Can't open database: %s\n", sqlite3_errmsg(this->m_sqliteDb));
		sqlite3_free(err);
		return false;
	}

	for (int r = 1; r <= rows; ++r) {
		int pos = (r * cols);
		MediaItem newMI = MediaItem();
		newMI.Filename = results[2 + pos];
		newMI.Path = results[3 + pos];

		this->m_stations[index].Media.push_back(newMI);
	}

	sqlite3_free_table(results);
}