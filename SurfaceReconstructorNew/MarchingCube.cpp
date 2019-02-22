#include <cmath>
#include <utility>
#include "MarchingCube.h"
#include "MarchingCubeConstants.h"


IdPoint CalculateIntersection(cint3 coord, int edgeNumber);
void CalculateNormals(Mesh& mesh);
Mesh Reindex(PointIdMapping& vertexMapping, vector<Triangle>& triangles);

void MarchingCube::Marching() {
	cint3 cellDim = surfaceGrid->resolution;
	int xWidth = surfaceGrid->vertexResolution.x;
	int xyWidth = surfaceGrid->vertexResolution.x * surfaceGrid->vertexResolution.y;

	PointIdMapping vertexMapping;
	vector<Triangle> triangles;
	float value[8];

	vector<SurfaceVertex>& sVertices = surfaceGrid->surfaceVertices;

	for(int i=0; i< sVertices.size(); i++){

		int tableIndex = 0;
		
		cint3 coord = sVertices[i].coord;
		
		value[0] = GetValue(coord);
		value[1] = GetValue(coord + cint3(0, 1, 0));
		value[2] = GetValue(coord + cint3(1, 1, 0));
		value[3] = GetValue(coord + cint3(1, 0, 0));
		value[4] = GetValue(coord + cint3(0, 0, 1));
		value[5] = GetValue(coord + cint3(0, 1, 1));
		value[6] = GetValue(coord + cint3(1, 1, 1));
		value[7] = GetValue(coord + cint3(1, 0, 1));

		/*if (GetValue(coord) < isoLevel)
			tableIndex |= 1;
		if (GetValue(coord + cint3(0, 1, 0)) < isoLevel)
			tableIndex |= 2;
		if (GetValue(coord + cint3(1, 1, 0)) < isoLevel)
			tableIndex |= 4;
		if (GetValue(coord + cint3(1, 0, 0)) < isoLevel)
			tableIndex |= 8;
		if (GetValue(coord + cint3(0, 0, 1)) < isoLevel)
			tableIndex |= 16;
		if (GetValue(coord + cint3(0, 1, 1)) < isoLevel)
			tableIndex |= 32;
		if (GetValue(coord + cint3(1, 1, 1)) < isoLevel)
			tableIndex |= 64;
		if (GetValue(coord + cint3(1, 0, 1)) < isoLevel)
			tableIndex |= 128;*/

		if (value[0] < isoLevel)
			tableIndex |= 1;
		if (value[1] < isoLevel)
			tableIndex |= 2;
		if (value[2] < isoLevel)
			tableIndex |= 4;
		if (value[3] < isoLevel)
			tableIndex |= 8;
		if (value[4] < isoLevel)
			tableIndex |= 16;
		if (value[5] < isoLevel)
			tableIndex |= 32;
		if (value[6] < isoLevel)
			tableIndex |= 64;
		if (value[7] < isoLevel)
			tableIndex |= 128;

		if (EDGE_TABLE[tableIndex] != 0) {
			if (EDGE_TABLE[tableIndex] & 8) {
				IdPoint pt = CalculateIntersection(coord, 3);
				int id = GetEdgeIndex(coord, 3);
				vertexMapping.insert(PointIdMapping::value_type(id, pt));
			}
			if (EDGE_TABLE[tableIndex] & 1) {
				IdPoint pt = CalculateIntersection(coord, 0);
				int id = GetEdgeIndex(coord, 0);
				vertexMapping.insert(PointIdMapping::value_type(id, pt));
			}
			if (EDGE_TABLE[tableIndex] & 256) {
				IdPoint pt = CalculateIntersection(coord, 8);
				int id = GetEdgeIndex(coord, 8);
				vertexMapping.insert(PointIdMapping::value_type(id, pt));
			}

			if (coord.x == cellDim.x - 1) {
				if (EDGE_TABLE[tableIndex] & 4) {
					IdPoint pt = CalculateIntersection(coord, 2);
					int id = GetEdgeIndex(coord, 2);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
				if (EDGE_TABLE[tableIndex] & 2048) {
					IdPoint pt = CalculateIntersection(coord, 11);
					int id = GetEdgeIndex(coord, 11);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
			}
			if (coord.y == cellDim.y - 1) {
				if (EDGE_TABLE[tableIndex] & 2) {
					IdPoint pt = CalculateIntersection(coord, 1);
					int id = GetEdgeIndex(coord, 1);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
				if (EDGE_TABLE[tableIndex] & 512) {
					IdPoint pt = CalculateIntersection(coord, 9);
					int id = GetEdgeIndex(coord, 9);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
			}
			if (coord.z == cellDim.z - 1) {
				if (EDGE_TABLE[tableIndex] & 16) {
					IdPoint pt = CalculateIntersection(coord, 4);
					int id = GetEdgeIndex(coord, 4);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
				if (EDGE_TABLE[tableIndex] & 128) {
					IdPoint pt = CalculateIntersection(coord, 7);
					int id = GetEdgeIndex(coord, 7);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
			}
			if ((coord.x == cellDim.x - 1) && (coord.y == cellDim.y - 1))
				if (EDGE_TABLE[tableIndex] & 1024) {
					IdPoint pt = CalculateIntersection(coord, 10);
					int id = GetEdgeIndex(coord, 10);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
			if ((coord.x == cellDim.x - 1) && (coord.z == cellDim.z - 1))
				if (EDGE_TABLE[tableIndex] & 64) {
					IdPoint pt = CalculateIntersection(coord, 6);
					int id = GetEdgeIndex(coord, 6);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}
			if ((coord.y == cellDim.y - 1) && (coord.z == cellDim.z - 1))
				if (EDGE_TABLE[tableIndex] & 32) {
					IdPoint pt = CalculateIntersection(coord, 5);
					int id = GetEdgeIndex(coord, 5);
					vertexMapping.insert(PointIdMapping::value_type(id, pt));
				}

			for (int i = 0; TRIANGLE_TABLE[tableIndex][i] != -1; i += 3) {
				Triangle triangle;
				int pointId0, pointId1, pointId2;
				pointId0 = GetEdgeIndex(coord, TRIANGLE_TABLE[tableIndex][i]);
				pointId1 = GetEdgeIndex(coord, TRIANGLE_TABLE[tableIndex][i + 1]);
				pointId2 = GetEdgeIndex(coord, TRIANGLE_TABLE[tableIndex][i + 2]);
				triangle.pointId[0] = pointId0;
				triangle.pointId[1] = pointId1;
				triangle.pointId[2] = pointId2;
				triangles.push_back(triangle);
			}
		}
	}
	Mesh mesh = Reindex(vertexMapping, triangles);

	CalculateNormals(mesh);
	return;
}

int MarchingCube::GetEdgeIndex(cint3 coord, int edgeNumber) {
	switch (edgeNumber) {
	case 0:
		return GetVertexIndex(coord)*3 + 1;
	case 1:
		return GetVertexIndex(coord+cint3(0,1,0))*3;
	case 2:
		return GetVertexIndex(coord+cint3(1,0,0))*3 + 1;
	case 3:
		return GetVertexIndex(coord)*3;
	case 4:
		return GetVertexIndex(coord+cint3(0,0,1))*3 + 1;
	case 5:
		return GetVertexIndex(coord+cint3(0,1,1))*3;
	case 6:
		return GetVertexIndex(coord+cint3(1,0,1))*3 + 1;
	case 7:
		return GetVertexIndex(coord+cint3(0,0,1))*3;
	case 8:
		return GetVertexIndex(coord)*3 + 2;
	case 9:
		return GetVertexIndex(coord+cint3(0,1,0))*3 + 2;
	case 10:
		return GetVertexIndex(coord+cint3(1,1,0))*3 + 2;
	case 11:
		return GetVertexIndex(coord+cint3(1,0,0))*3 + 2;
	}
}

IdPoint MarchingCube::CalculateIntersection(
	cint3 coord, 
	int edgeNumber,
	float* values
) {
	cfloat3 x1,x2;
	int vId[2];
	float value[2];
	
	vId[0] = EDGE_VERTEX[edgeNumber][0];
	vId[1] = EDGE_VERTEX[edgeNumber][1];

	x1 = surfaceGrid->GetVertexPosition(coord);
	x2 = x1;
	x1 += cube.vertexOffsets[vId[0]];
	x2 += cube.vertexOffsets[vId[1]];
	value[0] = values[vId[0]];
	value[1] = values[vId[1]];

	float w = (isoLevel - value[0]) / (value[1] - value[0]);
	if (w<0 || w>1) {
		printf("error: invalid intersection point value\n");
	}
	IdPoint intersection;
	intersection.pos = x1 + (x2-x1)*w;
	return intersection;
}

