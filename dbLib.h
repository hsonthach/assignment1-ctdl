/* Created by Nguyen Duc Dung on 2019-09-03.
 * =========================================================================================
 * Name        : dbLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - HCMUT
 * Description : The data structure library for Assignment 1
 * Course      : Data Structure and Algorithms - Fall 2019
 * =========================================================================================
 */

#ifndef DSA191_A1_DBLIB_H
#define DSA191_A1_DBLIB_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include <math.h>

#include "dsaLib.h"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */

struct LineString {
	double a;
	double b;
	LineString() {};

	LineString(double _a, double _b) {
		a = _a;
		b = _b;
	}
	LineString(const LineString &x) {
		a = x.a;
		b = x.b;
	}
	bool  operator==(const LineString &x) {
		return ((a == x.a) && (b == x.b));  
	}

	double calcDistance(const LineString &x) {
		return sqrt(pow(a - x.a, 2) + pow(b - x.b, 2)); 
	}

};

struct TLine {
	int id;
	int city_id;
	TLine() {};
	TLine(const TLine & x) {
		city_id = x.city_id;
		id = x.id;
	}
};

struct TCity {
	int id;
	string city_name;
	TCity() {};
	TCity(const TCity & x) {
		city_name = x.city_name;
		id = x.id;
	}

};

struct TStation {
	int id;
	int city_id;
	string name; 
	LineString *pLineString;  
	TStation() {
		this->id = -1;
		this->city_id = -1;
		this->name = "";
		this->pLineString = nullptr;
	};
	TStation(int id, int city_id, string name, LineString *pLineString) {
		this->id = id; 
		this->city_id = city_id; 
		this->name = name;  
		this->pLineString = pLineString; 
	}
	TStation(const TStation & x) {
		city_id = x.city_id;
		id = x.id;
		name = x.name; 
		pLineString = x.pLineString; 
	}
};

struct TStation_Line {

	int station_id; 
	int line_id; 
	TStation_Line() {};
	TStation_Line(int station_id, int line_id) {
		
		this->station_id = station_id; 
		this->line_id = line_id; 
	}
	TStation_Line(const TStation_Line & x) {
		station_id = x.station_id;
		line_id = x.line_id;
	}
	
};


struct TTrack {
	L1List<LineString> *plineStringList; 
	int id; 

	TTrack(int _id, L1List<LineString> *_plineStringList) {
		this->id = _id; 
		this->plineStringList = _plineStringList; 
	}

	TTrack() {
		this->plineStringList = new L1List<LineString>();
		this->id = -1; 
	}
	
	TTrack(const TTrack & x) {
		this->plineStringList = x.plineStringList;
		this->id = x.id;
	}

	int findStationPositionByPoint(LineString *pLineString) {
		int count = 0;
		L1Item<LineString> *temp = plineStringList->pHead;  
		while (temp != nullptr) {
			if (temp->data == *pLineString) return count; 
			count++; 
			temp = temp->pNext; 
		}
		return -1; 
	}

};

struct Error {
	string *pData; 
	int size; 
	Error() {
		pData = nullptr;
		size = 0; 
	}
};

////////////////////////////////////////// DATA

struct TrackData {
	L1List<TTrack> *pTrackList;
	TrackData() {
		pTrackList = new L1List<TTrack>();
	}

	bool getLine(string &file, string &line) {
		if (file.size() == 0) return false;
		int end = file.find('\n');
		line = file.substr(0, end);
		file.erase(0, end + 1);

		return true;
	
	}

	TTrack* getTTrackFromCSVLine(string line) {
		string delimitation = ",";

		int id; 
		L1List<LineString> *pLineStringList = new L1List<LineString>(); 

		int count = 0;  
		while ( count < 2) {
			count++;
			if (count == 1) { // id  
				string pushingelement = line.substr(0, line.find(delimitation));
				line = line.erase(0, pushingelement.size() + delimitation.size());
				id = stoi(pushingelement);
			}

			else if (count == 2) { // LINESTRINGList
				int start = line.find("(");
				int end = line.find(")");
				line = line.substr(start + 1, end - start);

				double a, b;
				LineString *pNew = nullptr; 

				// Add into LineStringList
				while (true) { // line.find(")") < line.size()
					
					if (line.find(" ") < line.size()) {
						end = line.find(" ");
						a = stod(line.substr(0, end));
						line.erase(0, end + 1);
					}
					if (line.find(",") < line.size()) {
						end = line.find(",");
						b = stod(line.substr(0, end));
						line.erase(0, end + 1);
					}
					else if (line.find(")") < line.size()) {
						end = line.find(")");
						b = stod(line.substr(0, end));

						line.erase(0, end + 2);
					}
					else break; 

					// Pushing into LineStringList of TTrack item 
					LineString *pNew = new LineString(a, b);
					pLineStringList->push(*pNew);
					
				}
			}

		}
		
		return new TTrack(id, pLineStringList); 
	}

	void LoadData() {
		string s;
		string line;
		
		FILE* f = fopen("tracks.csv", "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
		char* file2 = (char*)malloc(fsize + 1);
		fread(file2, 1, fsize, f);
		fclose(f);
		file2[fsize] = 0;
		stringstream file(file2);
	
		// Read file 
		L1List<TTrack> *_pTrackList = new L1List<TTrack>();
		bool isFirstLine = true;
		
		while (getline(file, line)) {
			
			if (isFirstLine) {
				isFirstLine = false;
				continue;
			}

			TTrack *pLine = this->getTTrackFromCSVLine(line);
			//cout << pLine->id <<endl ; 
			_pTrackList->push(*pLine);

		}

		this->pTrackList = _pTrackList;
	}


	TTrack * getTrackByID(int trackID) {
		L1Item<TTrack> * temp = pTrackList->pHead; 
		while (temp != nullptr) {
			if (temp->data.id == trackID) return &temp->data; 
			temp = temp->pNext; 
		}
		
		return nullptr; 
	}

	void removePointByPoint(LineString point) {
		L1Item<TTrack> * temp = pTrackList->pHead; 
		while (temp !=  nullptr) {
			L1Item<LineString> *lineStringTemp = temp->data.plineStringList->pHead; 
			int count = 0; 
			while (lineStringTemp != nullptr) {
				
				if (point == lineStringTemp->data) {
					temp->data.plineStringList->remove(count);
				}
				count++; 
				lineStringTemp = lineStringTemp->pNext; 
			}

			temp = temp->pNext; 
		}
	}

};

struct Station_LineData {
	L1List<TStation_Line> *pStation_LineList; 
	Station_LineData() {
		pStation_LineList = new L1List<TStation_Line>;
	}
	void LoadStation_LineList() {
		ifstream file("station_lines.csv");
		if (!file.is_open()) cout << "The file wasn't opened" << endl;


		// Read file 
		L1List<TStation_Line> *_pStation_LineList = new L1List<TStation_Line>();

		string line = "";
		string delimitation = ",";
		bool isFirstLine = true;
		while (getline(file, line)) {
			if (isFirstLine) {
				isFirstLine = false;
				continue;
			}

			TStation_Line *pLine = new TStation_Line();

			int count = 0;  // is used to minimize the propreties to add in data  

			while (count < 3) {

				count++;

				// Get the element from beginning to the comma
				string pushingelement = line.substr(0, line.find(delimitation));

				// Delete the beginning of line 
				line = line.erase(0, pushingelement.size() + delimitation.size());

				if (count == 2) { // station_id 
					pLine->station_id = stoi(pushingelement);
				}
				else if (count == 3) { // line_id
					pLine->line_id = stoi(pushingelement); 
				}

			}
			//pLine->station_id << "\t" << pLine->line_id << endl; 
			_pStation_LineList->push(*pLine);

		}

		this->pStation_LineList = _pStation_LineList;
	}

	int *getListOfStationIDByLineID(int lineID,int & N) {
		int *pList = new int[50000];
		N = 0; 
		L1Item<TStation_Line> *temp = this->pStation_LineList->pHead; 

		while (temp != nullptr) {
			if (temp->data.line_id == lineID) {
				pList[N] = temp->data.station_id; 
				N++; 
			}
			temp = temp->pNext; 
		}
		return pList; 

	}

	int getPositionByStationIDAndLineID(int stationID, int lineID) {
		int count = 0; 
		L1Item<TStation_Line> *temp = this->pStation_LineList->pHead;

		while (temp != nullptr) {
			
			if (temp->data.line_id == lineID && temp->data.station_id == stationID) {
				return count; 
			}
			temp = temp->pNext;
			count++; 
		}
		return -1;
	}

	void removeAllLineByStationID(int stationID) {
		L1Item<TStation_Line> *temp = this->pStation_LineList->pHead;
		int *pRemoveIndex = new int[100];
		int size = 0, count = 0;
		while (temp != nullptr) {
			if (temp->data.station_id == stationID) {
				pRemoveIndex[size] = count; 
				size++; 
			}
			count++; 
			temp = temp->pNext; 
		}

		for (int i = size -1 ; i > -1 ; i--) {
			this->pStation_LineList->remove(pRemoveIndex[i]);
		}
	}

};

struct CityData {
	L1List<TCity> * pCityList;
	CityData() {
		pCityList = new L1List<TCity>;
	}

	void LoadTCityList() {
		ifstream file("cities.csv");
		if (!file.is_open()) cout << "The file wasn't opened" << endl;


		// Read file 
		L1List<TCity> *_pCityList = new L1List<TCity>();

		string line = "";
		string delimitation = ",";
		bool isFirstLine = true;
		while (getline(file, line)) {
			if (isFirstLine) {
				isFirstLine = false;
				continue;
			}

			TCity *pLine = new TCity();

			int count = 0;  // is used to minimize the propreties to add in data  

			while (line.find(delimitation) < line.size() && count < 2) {

				count++;

				// Get the element from beginning to the comma
				string pushingelement = line.substr(0, line.find(delimitation));

				// Delete the beginning of line 
				line = line.erase(0, pushingelement.size() + delimitation.size());

				if (count == 1) { // id 
					pLine->id = stoi(pushingelement);
				}
				else if (count == 2) { // city_id 
					pLine->city_name = pushingelement;
				}

			}
			
			_pCityList->push(*pLine);

		}

		this->pCityList = _pCityList;
	}

	int getCityIDByCityName(string cityname) {
		int count = 0;
		L1Item<TCity> *temp = this->pCityList->pHead;
		while (temp != nullptr)
		{
			if (temp->data.city_name == cityname) return temp->data.id;
			temp = temp->pNext;
			
		}
		return -1;
	}
	
};

struct LineData {
	L1List<TLine>* pLineList;
	LineData() {
		pLineList = new L1List<TLine>;
	}
	void LoadTLineList() {
		ifstream file("lines.csv");
		if (!file.is_open()) cout << "The file wasn't opened" << endl;


		// Read file 
		//L1List<TLine> line ; 
		L1List<TLine> *pLineList = new L1List<TLine>();

		string line = "";
		string delimitation = ",";
		bool isFirstLine = true;
		while (getline(file, line)) {
			if (isFirstLine) {
				isFirstLine = false;
				continue;
			}

			TLine *pLine = new TLine();

			int count = 0;  // is used to minimize the propreties to add in data  

			while (count < 2) {
				count++;

				// Get the element from beginning to the comma
				string pushingelement = line.substr(0, line.find(delimitation));

				// Delete the beginning of line 
				line = line.erase(0, pushingelement.size() + delimitation.size());

				if (count == 1) { // id 
					pLine->id = stoi(pushingelement);
				}
				else if (count == 2) { // city_id 
					pLine->city_id = stoi(pushingelement);
				}
			}
			pLineList->push(*pLine);
		}

		this->pLineList = pLineList;
	}


	int calcLineByCityID(int city_id) {
		if (city_id == -1) return -1;
		L1Item<TLine> *temp = this->pLineList->pHead;
		int count = 0;
		while (temp != nullptr) {
			if (temp->data.city_id == city_id) count++;
			temp = temp->pNext;
		}
		return count;
	}


	int * getListOfLineByCity(int city_id, int&N) {
		int *list = new int[10000];
		N = 0;
		if (city_id == -1) return nullptr; 
		L1Item<TLine> *temp = this->pLineList->pHead;
		while (temp != nullptr) {
			if (temp->data.city_id == city_id) {
				list[N] = temp->data.id;

				N++;
			}
			temp = temp->pNext;
		}

		return list;
	}

	int getCityIDByLineID(int line_id) {
		L1Item<TLine> *temp = this->pLineList->pHead;
		while (temp != nullptr) {
			if (temp->data.id == line_id) {
				return temp->data.city_id;
			}
			temp = temp->pNext;
		}
		return -1;
	}
};

struct StationData {
	L1List<TStation>* pStationList;
	int maxID;
	StationData() {
		pStationList = new L1List<TStation>;
		maxID = 0;
	}


	bool includeSubString(string str, string substr) {

		if (str.find(substr) != string::npos) return true;

		return false;
	}


	// line contain id,name,point,city_id
	TStation * getTStationFromCSVLine(string line) {
		string delimitation = ",";

		int cityID, id;
		string name;
		double a, b;
		LineString *pLineString = nullptr;

		int count = 0;

		// Load CSV 
		while (count < 4) {
			count++;
			if (count == 3) { // city id 
				int last = line.find_last_of(",");
				string pushingElement = line.substr(last + 1, line.size() - last);
				cityID = stoi(pushingElement);
			}
			else if (count == 4) { // Point 
				int start = line.find("(");
				int end = line.find(")");
				line = line.substr(start + 1, end - start);

				while (line.find(" ") < line.size() || line.find(",") < line.size()) { // line.find(")") < line.size()
					if (line.find(" ") < line.size()) {
						end = line.find(" ");
						a = stod(line.substr(0, end));
						line.erase(0, end + 1);
					}
					if (line.find(",") < line.size()) {
						end = line.find(",");
						b = stod(line.substr(0, end));
						line.erase(0, end + 1);

					}
					else if (line.find(")") < line.size()) {
						end = line.find(")");
						b = stod(line.substr(0, end));
						line.erase(0, end + 2);
					}
					pLineString = new LineString(a, b);

				}
			}
			else if (count == 1) { //id
				string pushingelement = line.substr(0, line.find(delimitation));

				// Delete the beginning of line 
				line = line.erase(0, pushingelement.size() + delimitation.size());

				id = stoi(pushingelement);

			}
			else if (count == 2) { // Name
				string pushingelement = line.substr(0, line.find(delimitation));

				if (includeSubString(pushingelement, "\"")) {
					// Speacial name Case : (Ex:"Kilómetro 11,10") 
					line.erase(0, 1);
					pushingelement = line.substr(0, line.find("\""));

					// Delete the beginning of line 
					line = line.erase(0, pushingelement.size() + delimitation.size() + 1);
				}
				else {
					line = line.erase(0, pushingelement.size() + delimitation.size());
				}
				name = pushingelement;

			}
		}
		//cout << id << "\t" << cityID << "\t" << name << "\t" << pLineString->a << "\t" << pLineString->b << "\n"; 
		return new TStation(id, cityID, name, pLineString);
	}

	bool getLine(string &file, string &line) {
		if (file.size() == 0) return false;
		int end = file.find('\n');
		line = file.substr(0, end);
		file.erase(0, end + 1);

		return true;

	}

	void LoadStationList() {
		string s;
		string line;

		FILE* f = fopen("stations.csv", "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
		char* file2 = (char*)malloc(fsize + 1);
		fread(file2, 1, fsize, f);
		fclose(f);
		file2[fsize] = 0;
		stringstream file(file2);

		// Read file 

		L1List<TStation> *_pStationList = new L1List<TStation>();



		bool isFirstLine = true;
		while (getline(file, line)) {
			if (isFirstLine) {
				isFirstLine = false;
				continue;
			}

			TStation *pLine = this->getTStationFromCSVLine(line);

			// Update the ID 
			this->updateMaxID(pLine->id);



			_pStationList->push(*pLine);

		}

		this->pStationList = _pStationList;
	}

	void updateMaxID(int id) {
		if (this->maxID < id) this->maxID = id;
	}

	int *getListStationByCityID(int city_id, int &N) {
		int * pList = new int[10000];
		N = 0;
		if (city_id == -1) return pList;
		L1Item<TStation> *temp = pStationList->pHead;
		while (temp != nullptr)
		{
			if (temp->data.city_id == city_id) {

				pList[N] = temp->data.id;

				N++;
			}

			temp = temp->pNext;
		}

		return pList;
	}

	int getStationIDByStationName(string stationName) {
		L1Item<TStation> *temp = pStationList->pHead;

		while (temp != nullptr) {
			if (temp->data.name == stationName) return temp->data.id;
			temp = temp->pNext;
		}
		return -1;
	}

	int getCityIDByStationName(string stationName){
		L1Item<TStation> *temp = pStationList->pHead;

		while (temp != nullptr) {
			if (temp->data.name == stationName) return temp->data.city_id; 
			temp = temp->pNext; 
		}

		return -1; 
	}

	// id = idmax +1 , city_id = -1 , name , pLineString 
	TStation *getNewStationFromCSVString(string csv_description) {
		string name = "";
		int start = csv_description.find("\n") + 1;
		string line = csv_description.substr(start, csv_description.size() - start);
		int newID = this->maxID + 1;
		LineString *pLineString = nullptr; 

		int count = 0;
		while (includeSubString(csv_description,",")) {
			if (count == 0) { // name
				name = line.substr(0, line.find(","));
				string pushingelement = line.substr(0, line.find(","));
				
				if (includeSubString(pushingelement, "\"")) {
					// Speacial name Case : (Ex:"Kilómetro 11,10") 
					line.erase(0, 1);
					pushingelement = line.substr(0, line.find("\""));
					
					// Delete the beginning of line 
					line = line.erase(0, pushingelement.size() + 2);
				}
				else {
					line = line.erase(0, pushingelement.size() + 1);
				}
				name = pushingelement;
			}
			else if (count == 1) { // point 
				int start = line.find("(");
				int end = line.find(")");
				line = line.substr(start + 1, end - start);
				double a, b;

				while (line.find(" ") < line.size() || line.find(",") < line.size()) { // line.find(")") < line.size()
					if (line.find(" ") < line.size()) {
						end = line.find(" ");
						a = stod(line.substr(0, end));
						line.erase(0, end + 1);
					}
					if (line.find(",") < line.size()) {
						end = line.find(",");
						b = stod(line.substr(0, end));
						line.erase(0, end + 1);

					}
					else if (line.find(")") < line.size()) {
						end = line.find(")");
						b = stod(line.substr(0, end));
						line.erase(0, end + 2);
					}
					pLineString = new LineString(a, b);
					break;
				}
				break; 
			}
			count++;
		}

		TStation *newStation = new TStation(newID, -1, name,pLineString);
		//cout << newStation->id << "\t" << newStation->name << "\n" << newStation->pLineString->a << "\t" << newStation->pLineString->b << "\t" << newStation->city_id << endl; 
		
		return newStation; 
	}

	LineString* getPointFromCSVString(string csv_description) {
		
		int start = csv_description.find("\n") + 1;
		string line = csv_description.substr(start, csv_description.size() - start);

		int count = 0;
		while (line.find("(") < line.size()) {
			// point 
				int start = line.find("(");
				int end = line.find(")");
				line = line.substr(start + 1, end - start);
				
				LineString* pNew = new LineString(); 
				while ((line.find(" ") < line.size()) || line.find(",") < line.size()) { // line.find(")") < line.size()
					double a, b;
					if (line.find(" ") < line.size()) {
						end = line.find(" ");
						a = stod(line.substr(0, end));
						
						line.erase(0, end + 1);
					}
					if (line.find(",") < line.size()) {
						end = line.find(",");
						b = stod(line.substr(0, end));
						
						line.erase(0, end + 1);

					}
					else if (line.find(")") < line.size()) {
						end = line.find(")");
						b = stod(line.substr(0, end));
						line.erase(0, end + 2);
					}

					break;
				}
			count++;
		}
	}

	void insertNewStation(string csv_description) {
		
		TStation *newStation = this->getNewStationFromCSVString(csv_description); 
		
		this->updateMaxID(newStation->id);

		// Push into StationList
		this->pStationList->push(*newStation);
	}

	int findIndexByStationID(int stationID) {
		L1Item<TStation> *temp = this->pStationList->pHead;
		int count = 0;
		while (temp != nullptr) {
			if (temp->data.id == stationID) break;
			temp = temp->pNext;
			count++;
		}
		if (count == this->pStationList->size) return -1;
		else return count; 
	}

	int removeStationByID(int stationID) {
		// Get index of station
		int index = findIndexByStationID(stationID); 
		if (index == -1) return -1; 

		// Remove by index 
		this->pStationList->remove(index);

		this->updateMaxID();
		
		return 0;
	}
	void updateMaxID() {
		L1Item<TStation> *temp = this->pStationList->pHead; 
		if (temp != nullptr) {
			int maxID = temp->data.id; 
			while (temp != nullptr) {
				if (temp->data.id > maxID) maxID = temp->data.id; 
				temp = temp->pNext; 
			}
			this->maxID = maxID;  
		}
	}

	int updateStationByID(int stationID, string csv_description) {
		// Find index 
		int index = findIndexByStationID(stationID);
		if (index == -1) return -1;
		
		TStation *newStation = this->getNewStationFromCSVString(csv_description);
		
		L1Item<TStation> *pUpdateItem = this->pStationList->get(index); 

		// Update item 
		pUpdateItem->data.name = newStation->name; 
		pUpdateItem->data.pLineString = newStation->pLineString;
		//pUpdateItem->data = *newStation ; 
		
		return  0; 
	}

	LineString * getLineStringByStationID(int stationID) {
		L1Item<TStation> *temp = this->pStationList->pHead;
		
		while (temp != nullptr) {
			if (temp->data.id == stationID) return temp->data.pLineString; 
			temp = temp->pNext;
		
		}
		return nullptr;  
	}

};

class Data {
public:
	LineData * pLineData;
	CityData  * pCityData;
	StationData * pStationData;
	Station_LineData *pStation_LineData; 
	TrackData *pTrackData; 
	Error *pError; 
	Data() {
		pLineData = new  LineData(); 
		pCityData = new CityData();
		pStationData = new StationData();
		pStation_LineData = new Station_LineData(); 
		pTrackData = new TrackData(); 
		pError = new Error(); 
	}
};


// Please add more or modify as needed

void LoadData(void* &);
void ReleaseData(void* &);

#endif //DSA191_A1_DBLIB_H

