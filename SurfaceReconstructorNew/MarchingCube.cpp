#include <cmath>
#include <utility>
#include "MarchingCube.h"
#include "MarchingCubeConstants.h"

void MarchingCube::SetCubeWidth(float width) {
	for (int i=0; i<8; i++) {
		vertexOffsets[i].x = VERTEX_OFFSETS[i][0];
		vertexOffsets[i].y = VERTEX_OFFSETS[i][1];
		vertexOffsets[i].z = VERTEX_OFFSETS[i][2];
		vertexOffsets[i] *= width;
	}
}

void MarchingCube::Marching() {
	cint3 cellDim = surfaceGrid->resolution;
	int xWidth = surfaceGrid->vertexResolution.x;
	int xyWidth = surfaceGrid->vertexResolution.x * surfaceGrid->vertexResolution.y;

	float value[8];
	vector<SurfaceVertex>& sVertices = surfaceGrid->surfaceVertices;

	for(auto const& vertex : sVertices){

		int tableIndex = 0;
		cint3 coord = vertex.coord;
		
		value[0] = GetValue(coord);
		value[1] = GetValue(coord + cint3(0, 1, 0));
		value[2] = GetValue(coord + cint3(1, 1, 0));
		value[3] = GetValue(coord + cint3(1, 0, 0));
		value[4] = GetValue(coord + cint3(0, 0, 1));
		value[5] = GetValue(coord + cint3(0, 1, 1));
		value[6] = GetValue(coord + cint3(1, 1, 1));
		value[7] = GetValue(coord + cint3(1, 0, 1));
		
		bool doTriangle = true;
		for (int viter=0; viter<8; viter++) {
			if (value[viter] > NON_SURFACE - 0.1f) {
				doTriangle = false;
				break;
			}
		}

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
				InsertVertex(coord, 3, value);
			}
			if (EDGE_TABLE[tableIndex] & 1) {
				InsertVertex(coord, 0, value);
			}
			if (EDGE_TABLE[tableIndex] & 256) {
				InsertVertex(coord, 8, value);
			}

			// x+1 
			//if (coord.x == cellDim.x - 1) {
			if(surfaceGrid->GetSurfaceIndex(coord+cint3(1,0,0))==-1){
				if (EDGE_TABLE[tableIndex] & 4) {
					InsertVertex(coord, 2, value);
				}
				if (EDGE_TABLE[tableIndex] & 2048) {
					InsertVertex(coord, 11, value);
				}
			}

			// y+1 
			//if (coord.y == cellDim.y - 1) {
			if (surfaceGrid->GetSurfaceIndex(coord+cint3(0, 1, 0))==-1){
				if (EDGE_TABLE[tableIndex] & 2) {
					InsertVertex(coord, 1, value);
				}
				if (EDGE_TABLE[tableIndex] & 512) {
					InsertVertex(coord, 9, value);
				}
			}

			// z+1 
			//if (coord.z == cellDim.z - 1) {
			if (surfaceGrid->GetSurfaceIndex(coord+cint3(0, 0, 1))==-1){
				if (EDGE_TABLE[tableIndex] & 16) {
					InsertVertex(coord, 4, value);
				}
				if (EDGE_TABLE[tableIndex] & 128) {
					InsertVertex(coord, 7, value);
				}
			}

			// corner case
			//if ((coord.x == cellDim.x - 1) && (coord.y == cellDim.y - 1))
			if (surfaceGrid->GetSurfaceIndex(coord+cint3(1, 1, 0))==-1)
				if (EDGE_TABLE[tableIndex] & 1024) {
					InsertVertex(coord, 10, value);
				}
			//if ((coord.x == cellDim.x - 1) && (coord.z == cellDim.z - 1))
			if (surfaceGrid->GetSurfaceIndex(coord+cint3(1, 0, 1))==-1)
				if (EDGE_TABLE[tableIndex] & 64) {
					InsertVertex(coord, 6, value);
				}
			//if ((coord.y == cellDim.y - 1) && (coord.z == cellDim.z - 1))
			if (surfaceGrid->GetSurfaceIndex(coord+cint3(0, 1, 1))==-1)
				if (EDGE_TABLE[tableIndex] & 32) {
					InsertVertex(coord, 5, value);
				}


			if(!doTriangle)
				continue;
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

				/*if (pointId0==1569||pointId1==1569||pointId2==1569) {
					printf("?\n");
				}*/
			}
		}
	}
	
	Reindex(vertexMapping, triangles);
	mesh.CalculateNormals();
	return;
}

void MarchingCube::InsertVertex(cint3 coord, int edgeNumber, float* value) {
	IdPoint pt = CalculateIntersection(coord, edgeNumber, value);
	if(pt.pos.x < -998)
		return;
	int id = GetEdgeIndex(coord, edgeNumber);
	vertexMapping.insert(PointIdMapping::value_type(id, pt));
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
	IdPoint intersection;

	vId[0] = EDGE_VERTEX[edgeNumber][0];
	vId[1] = EDGE_VERTEX[edgeNumber][1];

	x1 = surfaceGrid->GetVertexPosition(coord);
	x2 = x1;
	x1 += vertexOffsets[vId[0]];
	x2 += vertexOffsets[vId[1]];
	value[0] = values[vId[0]];
	value[1] = values[vId[1]];

	if (value[0]==OUTSIDE || value[1]==OUTSIDE){
		intersection.pos = cfloat3(-999,-999,-999);
		return intersection;
	}

	float w = (isoLevel - value[0]) / (value[1] - value[0]);
	if (w<0 || w>1) {
		printf("error: invalid intersection point value\n");
	}
	intersection.pos = x1 + (x2-x1)*w;
	return intersection;
}



void MarchingCube::Reindex(PointIdMapping& vertexMapping, vector<Triangle>& triangles) {
	
	// reindex
	int nextId = 1;
	for (auto& pair : vertexMapping) {
		printf("%d\n",pair.first);
		pair.second.id = nextId ++;
	}
	for (auto& triangle : triangles) {
		for (int j=0; j<3; j++) {
			int tmp = triangle.pointId[j];
			triangle.pointId[j] = vertexMapping[tmp].id;
			if (triangle.pointId[j]==0) {
				printf("!?\n");
			}
		}
	}

	// copy to mesh
	mesh.numVertices = vertexMapping.size();
	mesh.vertices.resize(mesh.numVertices);
	int index = 0;
	for (const auto& pair : vertexMapping) {
		mesh.vertices[index] = pair.second.pos;
		index ++;
	}

	mesh.numFaces = triangles.size();
	mesh.faces.resize(mesh.numFaces*3);
	index = 0;
	for (const auto& triangle : triangles) {
		mesh.faces[index*3] = triangle.pointId[0];
		mesh.faces[index*3+1] = triangle.pointId[1];
		mesh.faces[index*3+2] = triangle.pointId[2];
		index ++;
	}
}

void Mesh::CalculateNormals() {
}

void Mesh::Output(string filePath) {
	FILE* fp = fopen(filePath.c_str(), "w");
	for (const auto& vertex : vertices) {
		fprintf(fp, "v %f %f %f\n", vertex.x, vertex.y, vertex.z);
	}
	for (int i=0; i<numFaces; i++) {
		fprintf(fp, "f %d %d %d\n", faces[i*3], faces[i*3+1], faces[i*3+2]);
	}
	fclose(fp);
}