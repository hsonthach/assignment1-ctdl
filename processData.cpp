/* Created by Nguyen Duc Dung on 2019-09-03.
 * =========================================================================================
 * Name        : processData.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - HCMUT
 * Description : Implementation of main features in the assignment
 * Course      : Data Structure and Algorithms - Fall 2019
 * =========================================================================================
 */

#include "processData.h"

/* TODO: You can implement methods, functions that support your data structures here.
 * */
using namespace std;

void Initialization() {
    // If you use global variables, please initialize them explicitly in this function.
}

void Finalization() {
    // Release your data before exiting
}

string *getBlockScopeArr(string a) {
	string *blockScopeArr = new string[3];
	blockScopeArr[0] = "";
	blockScopeArr[1] = "";
	blockScopeArr[2] = "";
	int count = 0;
	string *exceptionalRequest = new string[7]{ "CL ", "LSC ","LLC ","FC ","FS ", "IS ", "US" };
	bool isException = false; 
	for (int i = 0; i < 7; i++){
		if (includeSubString(a, exceptionalRequest[i])) {
			isException = true;
			break;
		}
	}
	if (isException ) {
		int updateIndex = 0; 
		if (includeSubString(a,"US ")) {
			int start = a.find(" ");
			a = a.erase(0, start + 1);
			int end;
			if (!includeSubString(a, " "))  end = a.size();
			else  end = a.find(" ");

			string pushingelement = a.substr(0, end);

				// Delete the beginning of line 
			a = a.erase(0, pushingelement.size());

			blockScopeArr[0] = pushingelement;
			updateIndex = 1; 
		}
		
		int start = a.find(" ");
		a = a.erase(0, start + 1);
		int  end = a.size(); 
		string pushingelement = a.substr(0, end);

		// Delete the beginning of line 
		a = a.erase(0, pushingelement.size());

		blockScopeArr[updateIndex] = pushingelement;
		
	}
	else {
		if (includeSubString(a," "))
		while (includeSubString(a, " ")) {
			int start = a.find(" ");
			a =a.erase(0,start+1);
			int end;
			if (!includeSubString(a, " "))  end = a.size();
			else  end = a.find(" ");

			string pushingelement = a.substr(0, end);

			// Delete the beginning of line 
			a = a.erase(0, pushingelement.size());

			blockScopeArr[count] = pushingelement;
			count++;
		}

	}

	return blockScopeArr;
}

/*
string *getBlockScopeArr(string a) {
	string *blockScopeArr = new string[3];
	blockScopeArr[0] = "";
	blockScopeArr[1] = "";
	blockScopeArr[2] = "";
	int count = 0;
	while (includeSubString(a, "<")) {
		int start = a.find("<");
		int end = a.find(">");

		string pushingelement = a.substr(start + 1, end - start - 1);

		// Delete the beginning of line 
		a = a.erase(start, pushingelement.size() + 2);

		blockScopeArr[count] = pushingelement;
		count++;
	}

	return blockScopeArr;
}
*/


bool includeSubString(string str, string substr) {
	if (str.find(substr) != string::npos) return true;
	return false;
}


void ProcessRequest(const char* pRequest, void* pData, void* &pOutput, int &N) {
    // TODO: Implement this function for processing a request
    // NOTE: You can add other functions to support this main process.
    //       pData is a pointer to a data structure that manages the dataset
    //       pOutput is a pointer reference. It is set to nullptr and student must allocate data for it in order to save the required output
    //       N is the size of output, must be a non-negative number
    Data *pLoad = static_cast<Data*>(pData);
	int *pReturn = nullptr;
	pReturn = new int[10000]; 
	pReturn = new int[100]; 
	N = 1; 
	// TODO: modify includeSubString with input is const char*

	string *blockScopeArr = getBlockScopeArr(pRequest);
	

	if (includeSubString(pRequest, "CL") && blockScopeArr[0] == "") {
				
		pReturn[0] = pLoad->pLineData->pLineList->size;
	}
	else if (includeSubString(pRequest, "CL") && blockScopeArr[0] != "") {
		int cityID = pLoad->pCityData->getCityIDByCityName(blockScopeArr[0]);
		if (cityID == -1) {
			pReturn[0] = cityID; 
		}
		else {
			int numOfLine = pLoad->pLineData->calcLineByCityID(cityID);
			pReturn[0] = numOfLine;
		}
	}
	else if (includeSubString(pRequest, "LSC") && blockScopeArr[0] != "") {
		int cityID = pLoad->pCityData->getCityIDByCityName(blockScopeArr[0]);
		pReturn = pLoad->pStationData->getListStationByCityID(cityID, N);
	}
	else if (includeSubString(pRequest, "LLC") && blockScopeArr[0] != "") {
		int cityID = pLoad->pCityData->getCityIDByCityName(blockScopeArr[0]);
		pReturn = pLoad->pLineData->getListOfLineByCity(cityID, N);
	}
	else if (includeSubString(pRequest, "LSL") && blockScopeArr[0] != "") {
		int lineID = stoi(blockScopeArr[0]);
		
		pReturn = pLoad->pStation_LineData->getListOfStationIDByLineID(lineID,N); 
	}
	else if (includeSubString(pRequest, "FC") && blockScopeArr[0] != "") {
		int cityID = pLoad->pCityData->getCityIDByCityName(blockScopeArr[0]);
		pReturn[0] = cityID;
	}
	else if (includeSubString(pRequest, "FS") && blockScopeArr[0] != "") {
		int stationID = pLoad->pStationData->getStationIDByStationName(blockScopeArr[0]);
		pReturn[0] = stationID;
	}
	else if (includeSubString(pRequest, "SLP") && blockScopeArr[0] != "" &&  blockScopeArr[1] != "") {
		int stationID = stoi(blockScopeArr[0]);
		int trackID = stoi(blockScopeArr[1]);
		int position = 0; 
		
		LineString *pLineString = pLoad->pStationData->getLineStringByStationID(stationID); 
		TTrack *pTrack =  pLoad->pTrackData->getTrackByID(trackID); 
		
		if (pLineString == nullptr || pTrack == nullptr) {
			position = -1; 
		}
		else {
			position =  pTrack->findStationPositionByPoint(pLineString); 
		}

		pReturn[0] = position; 
	}
	else if (includeSubString(pRequest, "IS") && blockScopeArr[0] != "" && blockScopeArr[1] == "" && blockScopeArr[2] == "") {
		string csv_description = blockScopeArr[0]; // NOT include city_id and id 
		pLoad->pStationData->insertNewStation(csv_description); 
		int res = pLoad->pStationData->maxID;
		pReturn[0] = res; 
	}
	else if (includeSubString(pRequest, "RS") && blockScopeArr[0] != "") {
		int stationID = stoi(blockScopeArr[0]);
		int res = pLoad->pStationData->findIndexByStationID(stationID);
		if (res != -1) {
			// Remove in Station Line
			pLoad->pStation_LineData->removeAllLineByStationID(stationID); 

			// Remove in Track (TODO:Load city_id to decrease search time)
			LineString *pLineString =  pLoad->pStationData->getLineStringByStationID(stationID);
			if (pLineString != nullptr){
				pLoad->pTrackData->removePointByPoint(*pLineString);  
			}

			// Remove in Station 
			res = pLoad->pStationData->removeStationByID(stationID); 
		}
		if (res != -1) res = 0; 
		pReturn[0] = res; 
	}
	else if (includeSubString(pRequest, "US") && blockScopeArr[0] != "" && blockScopeArr[1] != "") {
		//TODO: Data can be changed station_name,station_point (maybe change track data)
		int stationID = stoi(blockScopeArr[0]);
		int index = pLoad->pStationData->findIndexByStationID(stationID);
		int res = 0;
		if (index == -1) res = -1;
		else {
			string csv_description = blockScopeArr[1]; 
		
			int res = pLoad->pStationData->updateStationByID(stationID, csv_description); 
		}
		pReturn[0] = res;
	}
	else if (includeSubString(pRequest, "ISL") && blockScopeArr[0] != "" && blockScopeArr[1] != ""  && blockScopeArr[2] != "") {
		int stationID = stoi(blockScopeArr[0]);
		int lineID = stoi(blockScopeArr[1]);
		int index = stoi(blockScopeArr[2]);
		bool isNExist = (pLoad->pStation_LineData->getPositionByStationIDAndLineID(stationID, lineID) == -1) ;
		
		if (isNExist) {
			TStation_Line *pNewStation_Line = new TStation_Line(stationID, lineID);

			int res;
			if (pLoad->pStation_LineData->pStation_LineList->insert(index, *pNewStation_Line)) {
				res = 0;
			}
			else res = -1;
			pReturn[0] = res;
		}
		else {
			int res = -1;
			pReturn[0] = res;
		}
	}
	else if (includeSubString(pRequest, "RSL") && blockScopeArr[0] != "" && blockScopeArr[1] != "" ) {
		int stationID = stoi(blockScopeArr[0]);
		int lineID = stoi(blockScopeArr[1]);
		
		int index = pLoad->pStation_LineData->getPositionByStationIDAndLineID(stationID, lineID); 

		int res;
		if (index == -1) { res = -1; }
		else {
			res = 0; 
			pLoad->pStation_LineData->pStation_LineList->remove(index); 
		}

		pReturn[0] = res;
	}

	pOutput = pReturn;
	pData = pLoad;
}


