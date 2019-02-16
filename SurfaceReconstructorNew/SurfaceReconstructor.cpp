#include "SurfaceReconstructor.hpp"

void SurfaceReconstructor::SetupZGrid(){
    zGrid.SetCellWidth(infectRadius);
    zGrid.SetXMin(particleData.GetXMin() - padding*2);
    zGrid.SetXMax(particleData.GetXMax() + padding*2);
    zGrid.AdjustSize();
}

void SurfaceReconstructor::ExtractSurface(){
    ExtractSurfaceParticles();

    ExtractSurfaceVertices();

    ComputeScalarValues();

    Triangulate();
}

void SurfaceReconstructor::ExtractSurfaceParticles(){

	printf("extracting surface particles..\n");

    SortParticles();

    ComputeColorFieldAndMarkParticles();
}

void SurfaceReconstructor::SortParticles(){
    zGrid.SortParticles(particleData);
}

void SurfaceReconstructor::ComputeColorFieldAndMarkParticles(){
    for(int i=0; i<particleData.Size(); i++){
        cfloat3& xi = particleData.GetParticle(i).pos;
        cint3 coord = zGrid.ComputeCoordinate(xi);
        int pHash = zGrid.particleHandlers[i].particleHash;
        if(pHash == INVALID_CELL)
            continue;
        
        int numNeighbors = 0;
        cfloat3 normal(0,0,0);
        float pVol = particleSpacing * particleSpacing * particleSpacing;

        // for each neighbor cell
        for(int xx=-1; xx<=1; xx++)
        for(int yy=-1; yy<=1; yy++)
        for(int zz=-1; zz<=1; zz++){
            cint3 coord1 = coord + cint3(xx,yy,zz);
            int pHash1 = zGrid.ComputeCellHash(coord1);
            if(pHash1 == INVALID_CELL)
                continue;
            
            int startIndice = zGrid.startIndices[pHash1];
            int endIndice = zGrid.endIndices[pHash1];

            // for each neighboring particle
            for(int j=startIndice; j<endIndice; j++){
                if(j == i)
                    continue;
                cfloat3& xj = particleData.GetParticle(j).pos;
                cfloat3 xij = xi - xj;
                float d = xij.Norm();
                if(d >= infectRadius)
                    continue;
                cfloat3 nablaW = sphHelper.CubicGradient(xij);
                normal += nablaW;
                numNeighbors ++;
            }
        }
        normal = normal * pVol;
        float n_norm = normal.Norm();
        if(n_norm > normThres || numNeighbors < neighborThres)
            surfaceParticleMark[i] = 1;
        else 
            surfaceParticleMark[i] = 0;
        
        printf("%d %f %d\n",i, n_norm, numNeighbors);
    }
}


void SurfaceReconstructor::ExtractSurfaceVertices(){

	printf("extracting surface vertices..\n");

    cfloat3 aabbLen = cfloat3(infectRadius, infectRadius, infectRadius);
    int numSurfaceVertices = 0;

    //scatter approach
    for(int i=0; i<particleData.Size();i++){
        if(surfaceParticleMark[i]==0)
            continue;
        cfloat3& x = particleData.GetParticle(i).pos;
        //AABB bounding box
        cfloat3 boxMin = x - aabbLen;
        cfloat3 boxMax = x + aabbLen;
        cint3 coordMin = surfaceGrid.GetVertexCoord(boxMin) + cint3(1,1,1);
        cint3 coordMax = surfaceGrid.GetVertexCoord(boxMax);
        
        for(int xx=coordMin.x; xx<=coordMax.x; xx++)
        for(int yy=coordMin.y; yy<=coordMax.y; yy++)
        for(int zz=coordMin.z; zz<=coordMax.z; zz++){
            int index = surfaceGrid.GetVertexIndex(cint3(xx,yy,zz));
            if( surfaceGrid.vertices[index].surfaceIndex == -1 ){
                surfaceGrid.vertices[index].surfaceIndex = numSurfaceVertices ++;
                SurfaceVertex sVertex;
                sVertex.gridIndex = index;
                surfaceGrid.surfaceVertices.push_back(sVertex);
            }
        }
    }
}

void SurfaceReconstructor::ComputeScalarValues(){
    //gather approach
    for(int i=0; i<surfaceGrid.surfaceVertices.size(); i++){
        int gridIndex = surfaceGrid.surfaceVertices[i].gridIndex;
		cfloat3 xi = surfaceGrid.GetVertexPosition(gridIndex);	
		
		cint3 coord = zGrid.ComputeCoordinate(xi);
		int pHash = zGrid.particleHandlers[i].particleHash;
		if (pHash == INVALID_CELL)
			continue;

		int numNeighbors = 0;
		cfloat3 normal(0, 0, 0);
		float pVol = particleSpacing * particleSpacing * particleSpacing;

		// for each neighbor cell
		for (int xx=-1; xx<=1; xx++)
			for (int yy=-1; yy<=1; yy++)
				for (int zz=-1; zz<=1; zz++) {
					cint3 coord1 = coord + cint3(xx, yy, zz);
					int pHash1 = zGrid.ComputeCellHash(coord1);
					if (pHash1 == INVALID_CELL)
						continue;

					int startIndice = zGrid.startIndices[pHash1];
					int endIndice = zGrid.endIndices[pHash1];

					// for each neighboring particle
					for (int j=startIndice; j<endIndice; j++) {
						if (j == i)
							continue;
						cfloat3& xj = particleData.GetParticle(j).pos;
						cfloat3 xij = xi - xj;
						float d = xij.Norm();
						if (d >= infectRadius)
							continue;
						cfloat3 nablaW = sphHelper.CubicGradient(xij);
						normal += nablaW;
						numNeighbors ++;
					}
				}
    }
}

void SurfaceReconstructor::Triangulate(){

}