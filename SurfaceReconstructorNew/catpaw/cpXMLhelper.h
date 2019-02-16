#pragma once
#include "tinyxml2.h"
#include <string>
#include "geometry.h"

using namespace tinyxml2;

class Tinyxml_Reader {
private:
	XMLElement* basenode = NULL;
public:
	void Use(XMLElement* base);
	const char* GetText(const char* name);
	float GetFloat(const char* name);
	int GetInt(const char* name);
	cfloat3 GetFloat3(const char* name);
	cint3 GetInt3(char* name);
	void GetFloatN(float* buffer, int size, const char* name);
};
