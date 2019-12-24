/* Created by Nguyen Duc Dung on 2019-09-03.
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - HCMUT
 * Description : The data structure library for Assignment 1
 * Course      : Data Structure and Algorithms - Fall 2019
 * =========================================================================================
 */

#include "dbLib.h"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

void ReleaseData(void* & pData){
}
 
 

void LoadData(void* &pData) {
	
	Data *pLoad = new Data();
	pLoad->pCityData->LoadTCityList();
	pLoad->pLineData->LoadTLineList();
	pLoad->pStationData->LoadStationList();
	pLoad->pStation_LineData->LoadStation_LineList(); 
	pLoad->pTrackData->LoadData(); 
	
	pData = pLoad;
}

