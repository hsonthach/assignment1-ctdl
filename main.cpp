#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sstream>
#include <math.h>

#include "dsaLib.h"
#include "dbLib.h"
#include "processData.h"

using namespace std;

void LoadTestCase(string *req, int &size ) {
	string s;
	string line;

	FILE* f = fopen("testcases.txt", "rb");
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
		if (line[line.size()-1] == '\r')
		line.erase(line.size() - 1, 1); 
		req[size] = line; 
		size++; 
	}
}

int main() {
   // Initialization();

    void*   pData = nullptr;
    void*   pOutput = nullptr;
    int     N;
    //cout << "Run " ; 
    LoadData(pData);
    assert(pData != nullptr);
    cout << fixed << setprecision(8);
    string *req = new string[100];
	int size = 0; 
	LoadTestCase(req, size); 
	int DEBUGAT = 2; 
	for (int i = 0; i < size; i++) {
		ProcessRequest(req[i].data(), pData, pOutput, N);
		/*if (i == DEBUGAT) {
			cout << "Request: \t" << req[i].data() << endl; 
		}
		cout << "Number : \t" << i << "\n"; */
		PrintReqOutput<int>(req[i].data(), (int*)pOutput, N);
		delete[](int*)pOutput;
		pOutput = nullptr;
	}
  //  while (true) {
  //      //req = "";
		////getline(cin, req);
  //      if (cin.bad()) {
  //          cin.clear();
  //          cin.ignore(1024, '\n');
  //          continue;
  //      }
  //      if (req == "Exit") {
  //          break;
  //      }
  //      ProcessRequest(req.data(), pData, pOutput, N);
  //      PrintReqOutput<int>(req.data(), (int*)pOutput, N);
  //      delete [] (int*)pOutput;
  //      pOutput = nullptr;
  //  }

  //  ReleaseData(pData);
  //  Finalization();
    return 0;
}


/*

Data{
	LineData{
		pLineList[
			TLine{
				int id
				int cityID
			};
		];
	},
	CityData {
		pCityList[
			TCity: {
				cityName:
				id:
			}
		]
	}
	StationData {
		int maxID
		pStationList [
			TStation {
				id:
				city_id:
				name:
				pLineString  {a,b}
			}
		]
	}
	StationLineData{
		pStationLineList [
			TStationLine {
				station_id
				line_id
			}
		]
	}
	TrackData {
		pTrackList [
			TTrack {
				id
				pLineStringList [
					pLineString {a,b}
				]
			}
		]
	}

}

*/
