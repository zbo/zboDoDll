#include "pch.h"
#include "../TestPluginTCale/Common.h"
#include "../TestPluginTCale/TCalcFuncSets.h"
#include "../TestPluginTCale/SearchApp.h"
#include "../TestPluginTCale/FindApp.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


TEST(Sample, HelloWorld) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
  float arr[10];
  int a = 1;
  FillinPOutDefault(arr, a);
}

TEST(E2E, DAY420) {
	fstream file;
	file.open("C:\\zd_pazq_hy\\debug2.txt", ios::in);
	vector<float> pfINa_high;
	vector<float> pfINb_low;
	char buf[1024];
	while (file.getline(buf, sizeof(buf))) {
	std:string str(buf);
		vector<string> strarr = mySplit(str, " ");
		float high = atof(strarr[0].c_str());
		pfINa_high.push_back(high);
		float low = atof(strarr[1].c_str());
		pfINb_low.push_back(low);
		cout << str << endl;
	}
	file.close();
	int DataLength = pfINa_high.size();
	float* out = new float[DataLength];
	float* pfINa = new float[DataLength];
	float* pfINb = new float[DataLength];
	float* pfINc = new float[DataLength];
}