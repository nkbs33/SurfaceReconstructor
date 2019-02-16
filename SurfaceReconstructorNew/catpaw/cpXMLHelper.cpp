#include "cpToolBox.h"

using namespace tinyxml2;

void Tinyxml_Reader::Use(XMLElement* base) {
	basenode = base;
	if (!base) {
		printf("WARNING: NULL Node Used.\n");
	}
}

const char* Tinyxml_Reader::GetText(const char* name) {
	float tmp=0;
	if (basenode!=NULL) {
		XMLElement* attr = basenode->FirstChildElement(name);
		if (attr)
			return attr->GetText();
	}
	return NULL;
}

float Tinyxml_Reader::GetFloat(const char* name) {
	if (basenode==NULL)
		return 0;
	float tmp=0;
	XMLElement* attr = basenode->FirstChildElement(name);
	if (attr)
		attr->QueryFloatText(&tmp);
	return tmp;
}

int Tinyxml_Reader::GetInt(const char* name) {
	if (basenode==NULL)
		return 0;
	int tmp=0;
	XMLElement* e = basenode->FirstChildElement(name);
	if(e)
		e->QueryIntText(&tmp);
	return tmp;
}

cfloat3 Tinyxml_Reader::GetFloat3(const char* name) {
	if (basenode==NULL)
		return cfloat3(0,0,0);
	XMLElement* e = basenode->FirstChildElement(name);
	if(!e)
		return cfloat3(0, 0, 0);
	cfloat3 tmp(0, 0, 0);
	const char* text = e->GetText();
	sscanf(text, "%f,%f,%f", &tmp.x, &tmp.y, &tmp.z);
	return tmp;
}

cint3 Tinyxml_Reader::GetInt3(char* name) {
	if (basenode==NULL)
		return cint3(0, 0, 0);
	XMLElement* e = basenode->FirstChildElement(name);
	if (!e)
		return cint3(0, 0, 0);
	cint3 tmp(0, 0, 0);
	const char* text = e->GetText();
	sscanf(text, "%d,%d,%d", &tmp.x, &tmp.y, &tmp.z);
	return tmp;
}



void Tinyxml_Reader::GetFloatN(float* buffer, int size, const char* name) {
	if (basenode==NULL)
		return;
	XMLElement* e = basenode->FirstChildElement(name);
	if (!e) {
		printf("Error: accessing invalid float n.\n");
		return;
	}
		
	const char* str = e->GetText();
	std::string fmstr = "";
	for (int i=0; i<size-1; i++)
		fmstr = fmstr+"%f,";
	fmstr = fmstr+"%f";
	sscanf(str, fmstr.c_str(), &buffer[0], &buffer[1], &buffer[2]);
}