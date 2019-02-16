#pragma once


#include "geometry.h"
#include <vector>
#include <utility>
#include <map>
#include <cassert>

using namespace std;


//vertex, position and norm, edge list, triangle list
struct objvertex {
	cfloat3 pos;
	cfloat3 norm;//if has any or zero
};

//edge, point on edge, triangle near edge
struct objedge {
	int p1,p2;
	float length;
	int trianglelist[2];//may has more than two in fact
};

struct objtriangle {
	int plist[3];
	int edgelist[3];
	cfloat3 norm; //if has any or zero
	int objectId;
};


#define CONSTRAINT_STRETCH 0
#define CONSTRAINT_BEND 1

struct edgeConstraint {
	int p1,p2,p3,p4;
	int t1,t2;
	float L0;
	float Phi0;
};

class ObjContainer {
public:
	vector<objvertex> pointlist;
	vector<objedge> edgelist;
	vector<objtriangle> trianglelist;
	vector<vector<int> > vertex_triangle; //triangle list
	map<pair<int,int>, vector<int> > edge_triangles;
	vector<edgeConstraint> edgeConstraints;

	//geometry build up
	void addvertex(objvertex& v) {
		pointlist.push_back(v);
	}
	int addtriangle(int p1,int p2,int p3) {
		objtriangle t;
		t.plist[0] = p1;
		t.plist[1] = p2;
		t.plist[2] = p3;
		trianglelist.push_back(t);

		//vertex triangle
		int tid = trianglelist.size()-1;
		addvertex_triangle(t.plist[0], tid);
		addvertex_triangle(t.plist[1], tid);
		addvertex_triangle(t.plist[2], tid);

		//edge triangle


		return 0;
	}
	void addvertex_triangle(int vid, int tid) {
		vertex_triangle[vid].push_back(tid);
	}
	
	
	//IO
	void writeToFile(const char* filepath);


	//PBD
	void buildEdgeList();
};

class ObjBuilder {
public:
	ObjContainer* build_rectangular(int rownum,int colnum,float spacing);
};

class ObjReader {
public:
	ObjContainer* readraw(char* filepath);
	ObjContainer* readobj(char* filepath);
	ObjContainer* readobj_n(char* filepath);
};