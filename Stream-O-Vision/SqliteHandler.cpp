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
		newStation.dbStationId = (int)results[0 + pos];
		newStation.StationId = results[1 + pos];
		newStation.StationName = results[2 + pos]; 

		/*int tempRc; 
		int tempRows, tempCols; 
		char** tempResults; 
		char* terr;
		char* id = results[0 + pos];

		char* q2= "SELECT * FROM playlists WHERE fk_stations_id=";

		char* q{ new char[strlen(id) + 23] };

		q = strcpy(q, q2);
		q = strcat(q, id);
		q = strcat(q, ";");
		tempRc = sqlite3_get_table(this->m_sqliteDb, q, &tempResults, &tempRows, &tempCols, &terr);
		if (tempRc) {
			const char* errMsg = sqlite3_errmsg(this->m_sqliteDb);
			fprintf(stdout, "Can't open database: %s\n", sqlite3_errmsg(this->m_sqliteDb));
			sqlite3_free(err);
			break;
		}

		MediaItem newMI;
		for (int rr = 1; rr <= tempRows; ++rr) {
			int ppos = (rr * tempCols);
			newMI.Path = tempResults[ppos + 3];
			newMI.Filename = tempResults[ppos + 2];
		}

		newStation.Media.push_back(newMI);*/
		this->m_stations.push_back(newStation);
	}

	sqlite3_free_table(results);
}

std::vector<Station> SqliteHandler::GetStationMap() {
	return this->m_stations;
}