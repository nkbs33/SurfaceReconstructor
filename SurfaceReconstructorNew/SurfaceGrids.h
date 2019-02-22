#pragma once

#include "Grids.h"

class SurfaceVertex {
public:
	int gridIndex;
	cint3 coord;
	float value;
};


class SurfaceGrid {
public:
	cfloat3 xmin;
	cfloat3 xmax;
	float cellWidth;
	cint3 resolution;
	cint3 vertexResolution;
	veci surfaceIndices;
	vector<SurfaceVertex> surfaceVertices;

	int GetVertexIndex(cint3 coord) {
		if (coord.x<0 || coord.x>=vertexResolution.x ||
			coord.y<0 || coord.y>=vertexResolution.y ||
			coord.z<0 || coord.z>=vertexResolution.z)
			return INVALID_CELL;

		return coord.z*(vertexResolution.y*vertexResolution.x)
			+ coord.y*vertexResolution.x + coord.x;
	}
	cint3 GetVertexCoord(int index) {
		cint3 coord;
		coord.z = index / (vertexResolution.x * vertexResolution.y);
		coord.y = index % (vertexResolution.x * vertexResolution.y) / vertexResolution.x;
		coord.x = index % vertexResolution.x;
		return coord;
	}
	cfloat3 GetVertexPosition(int index) {
		return xmin + GetVertexCoord(index)*cellWidth;
	}
	cfloat3 GetVertexPosition(cint3 coord) {
		return xmin + coord*cellWidth;
	}
	cint3 GetVertexCoord(cfloat3 p) {
		cint3 coord;
		coord = floor((p - xmin)/cellWidth);
		return coord;
	}

	void Init() {
		resolution = ceil((xmax-xmin)/cellWidth);
		xmax = xmin + resolution * cellWidth;
		vertexResolution = resolution+cint3(1, 1, 1);
		surfaceIndices.resize(vertexResolution.prod());
		surfaceVertices.clear();
		for (int i=0; i<surfaceIndices.size(); i++)
			surfaceIndices[i] = -1;
	}
};