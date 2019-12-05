//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "pch.h"
#include "pch.h"
#include "../TestPluginTCale/Common.h"
#include "../TestPluginTCale/TCalcFuncSets.h"
#include "../TestPluginTCale/SearchApp.h"
#include "../TestPluginTCale/FindApp.h"
#include "../TestPluginTCale/BI.h"
#include "../TestPluginTCale/FX.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

TestDataBag* LoadData(std::string file_path) {
	TestDataBag* bag = new TestDataBag;
	fstream file;
	file.open(file_path, ios::in);
	vector<float> pfINa_high;
	vector<float> pfINb_low;
	vector<float> pfINc_date;
	char buf[1024];
	while (file.getline(buf, sizeof(buf))) {
	std:string str(buf);
		vector<string> strarr = mySplit(str, " ");
		float high = atof(strarr[0].c_str());
		pfINa_high.push_back(high);
		float low = atof(strarr[1].c_str());
		pfINb_low.push_back(low);
		float date1 = atof(strarr[2].c_str());
		pfINc_date.push_back(date1);
	}
	file.close();
	int DataLength = pfINa_high.size();
	float* out = new float[DataLength];
	float* pfINa = new float[DataLength];
	float* pfINb = new float[DataLength];
	float* pfINc = new float[DataLength];
	memcpy(pfINa, &pfINa_high[0], pfINa_high.size() * sizeof(float));
	memcpy(pfINb, &pfINb_low[0], pfINb_low.size() * sizeof(float));
	memcpy(pfINc, &pfINc_date[0], pfINc_date.size() * sizeof(float));
	bag->DataLength = DataLength;
	bag->out = out;
	bag->pfINa = pfINa;
	bag->pfINb = pfINb;
	bag->pfINc = pfINc;
	return bag;
}
