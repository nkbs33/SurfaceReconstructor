#pragma once

#include "catpaw/vec_define.h"
#include "SurfaceGrids.h"
#include <map>

#define OUTSIDE 999

struct IdPoint {
	int id;
	cfloat3 pos;
};

typedef map<int, IdPoint> PointIdMapping;

struct Triangle {
	int pointId[3];
};

struct Mesh {
	int numVertices;
	vecf3 vertices;
	vecf3 normals;
	int numFaces;
	veci faces;

	Mesh(){}
};

struct EdgeVertex {
	int v1, v2;
};

struct Cube {
	cfloat3 vertexOffsets[8];
	void SetWidth(float width) {
		for (int i=0; i<8; i++) {
			vertexOffsets[i].x = VERTEX_OFFSETS[i][0];
			vertexOffsets[i].y = VERTEX_OFFSETS[i][1];
			vertexOffsets[i].z = VERTEX_OFFSETS[i][2];
			vertexOffsets[i] *= width;
		}
	}
};

class MarchingCube {
public:

	float isoLevel;
	veci* surfaceIndexMap;
	SurfaceGrid* surfaceGrid;
	Cube cube;
	Mesh mesh;

	int GetVertexIndex(cint3 coord) {
		surfaceGrid->GetVertexIndex(coord);
	}

	void SetIsoLevel(float isoLevel_) {
		isoLevel = isoLevel;
	}

	void SetWidth(float width) {
		cube.SetWidth(width);
	}

	int GetEdgeIndex(cint3 coord, int edge);

	float GetValue(cint3 coord) {
		int index = GetVertexIndex(coord);
		int surfaceIndex = surfaceGrid->surfaceIndices[index];
		if (surfaceIndex == -1) {
			printf("Error: accessing non-surface vertex\n");
			return OUTSIDE;
		}
		else {
			return surfaceGrid->surfaceVertices[surfaceIndex].value;
		}
	}

	void Marching();

	IdPoint CalculateIntersection(cint3 coord, int edgeNumber, float* values);

};