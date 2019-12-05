//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

class TestDataBag {
public:
	int DataLength;
	float* out;
	float* pfINa;
	float* pfINb;
	float* pfINc;

};

TestDataBag* LoadData(std::string file_path);
