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
		TRACE( "Error in LoadALL(): ", errMsg );
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

	return true;
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
		TRACE("Error in GetPlaylist(): ", errMsg);
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

	return true;
}

int SqliteHandler::AddStation(std::string StationId, std::string StationName) {
	std::string query = "INSERT INTO stations (StationId, StationName) VALUES ('" + StationId + "','" + StationName + "');";
	char* err;
	
	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in AddStation(): ", errMsg);
		sqlite3_free(err);
		return -1;
	}

	return sqlite3_last_insert_rowid(this->m_sqliteDb);
}

BOOL SqliteHandler::DeleteStation(int dbStationId) {
	std::string query = "DELETE FROM stations WHERE id=" + std::to_string(dbStationId) + ";";
	char* err;

	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in DeleteStation(): ", errMsg);
		sqlite3_free(err);
		return false;
	}

	return true;
}

BOOL SqliteHandler::DeletePlaylistByStationId(int dbStationId) {
	std::string query = "DELETE FROM playlists WHERE fk_stations_id=" + std::to_string(dbStationId) + ";";
	char* err;

	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in DeletePlaylistByStationId(): ", errMsg);
		sqlite3_free(err);
		return false;
	}

	return true;
}

BOOL SqliteHandler::DeletePlaylistItemById(int dbPlaylistId) {
	std::string query = "DELETE FROM playlists WHERE id=" + std::to_string(dbPlaylistId) + ";";
	char* err;

	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in DeletePlaylistItemById(): ", errMsg);
		sqlite3_free(err);
		return false;
	}

	return true;
}

int SqliteHandler::AddPlaylistItem(std::string filename, std::string path, int dbStationId) {
	std::string query = "INSERT INTO playlists (Filename, Path, fk_stations_id) VALUES ('" + filename + "','" + path + "',"+ std::to_string(dbStationId) +");";
	char* err;

	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in AddPlaylistItem(): ", errMsg);
		sqlite3_free(err);
		return -1;
	}

	return sqlite3_last_insert_rowid(this->m_sqliteDb);
}

BOOL SqliteHandler::CreateTables() {
	std::string query = "CREATE TABLE \"playlists\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,\"fk_stations_id\" INTEGER,\"Filename\" TEXT,\"Path\" TEXT)";
	char* err;

	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in CreateTables() Table 1: ", errMsg);
		sqlite3_free(err);
		return FALSE;
	}

	query = "CREATE TABLE \"stations\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,\"StationId\" TEXT NOT NULL UNIQUE,\"StationName\" INTEGER)";
	this->m_sqliteReturnCode = sqlite3_exec(this->m_sqliteDb, query.c_str(), NULL, NULL, &err);
	if (this->m_sqliteReturnCode) {
		const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
		TRACE("Error in CreateTables() Table 1: ", errMsg);
		sqlite3_free(err);
		return FALSE;
	}

	return TRUE;
}
