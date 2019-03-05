
#include "SurfaceReconstructor.hpp"



int main_(){
    printf("Welcome to Surface Reconstruction.\n");
    SurfaceReconstructor surfaceReconstructor;
    
    surfaceReconstructor.LoadParticle("test.txt");
    surfaceReconstructor.SetupGrids();
    
    surfaceReconstructor.ExtractSurface();
    //surfaceReconstructor.OutputMesh("test.obj");
    return 0;
}



void Test() {
	SurfaceReconstructor sr;
	SurfaceGrid& surfaceGrid = sr.surfaceGrid;
	surfaceGrid.xmin = cfloat3(-0.15,-0.15,-0.15);
	surfaceGrid.xmax = cfloat3(0.16,0.16,0.16);
	surfaceGrid.cellWidth = 0.01;
	surfaceGrid.Init();
	
	int numSurfaceVertices = 0;
	for(int xx=0; xx<surfaceGrid.vertexResolution.x; xx++)
		for (int yy=0; yy<surfaceGrid.vertexResolution.y; yy++)
			for (int zz=0; zz<surfaceGrid.vertexResolution.z; zz++) {
				surfaceGrid.InsertSurfaceVertex(cint3(xx,yy,zz));
			}
	printf("surface vertices: %d\n", surfaceGrid.surfaceVertices.size());
	for (auto& sv : surfaceGrid.surfaceVertices) {
		cint3 coord = sv.coord;
		cfloat3 pos = surfaceGrid.GetVertexPosition(coord);
		//sv.value = 0;
		sv.value = pos.Norm();
	}

	MarchingCube marchingCube;
	marchingCube.surfaceGrid = & surfaceGrid;
	marchingCube.SetCubeWidth(surfaceGrid.cellWidth);
	marchingCube.isoLevel = 0.1;

	marchingCube.Marching();
	marchingCube.mesh.Output("test.obj");
}

int main() {
	main_();
	return 0;
}