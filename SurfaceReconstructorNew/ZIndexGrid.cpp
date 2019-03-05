#include "ZIndexGrid.hpp"
#include "IndexMap.hpp"

void ZIndexGrid::BindParticleData(ParticleData& particleData){
    particles = &particleData.GetParticleArray();
    numParticles = particles->size();
    particleHandlers.resize(numParticles);
    particleHashes.resize(numParticles);
    reorderBuffer.resize(numParticles);
}

cint3 ZIndexGrid::ComputeCoordinate(cfloat3& pos){
    cint3 pCoord;
    pCoord.x = floor(pos.x - xmin.x)/cellWidth;
    pCoord.y = floor(pos.y - xmin.y)/cellWidth;
    pCoord.z = floor(pos.z - xmin.z)/cellWidth;
    return pCoord;
}

int ZIndexGrid::ComputeCellHash(cint3& pCoord){

    if(pCoord.x<0 || pCoord.x>=resolution ||
        pCoord.y<0 || pCoord.y>=resolution ||
        pCoord.z<0 || pCoord.z>=resolution)
        return INVALID_CELL;

    int mappedIndex = indexMap[pCoord.x] |
        indexMap[pCoord.y]<<1 |
        indexMap[pCoord.z]<<2;
    return mappedIndex;
}

void ZIndexGrid::ComputeParticleHash(){

    for(int i=0; i<numParticles; i++){
        cfloat3& pos = (*particles)[i].pos;

        cint3 pCoord = ComputeCoordinate(pos);
        int pHash = ComputeCellHash(pCoord);

        particleHashes[i] = pHash;
        particleHandlers[i].particleId = i;
        particleHandlers[i].particleHash = pHash;
    }
}

bool compare(ParticleHandler& a, ParticleHandler& b){
    return a.particleHash < b.particleHash;
}
#include <algorithm>
void ZIndexGrid::SortParticleHash(){
    sort(particleHandlers.begin(), particleHandlers.end(), compare);
}

void ZIndexGrid::ComputeStartIndices(){
    for(int i=0; i<startIndices.size(); i++){
        startIndices[i] = CELL_EMPTY;
        endIndices[i] = CELL_EMPTY;
    }

    if(particleHandlers[0].particleHash == INVALID_CELL)
		return;
	else
        startIndices[particleHandlers[0].particleHash] = 0;
   
	
	int i;
    for(i=1; i<particleHandlers.size(); i++){
		if (particleHandlers[i].particleHash == INVALID_CELL) {
			endIndices[particleHandlers[i-1].particleHash] = i;
			break;
		}
		if(particleHandlers[i].particleHash != 
            particleHandlers[i-1].particleHash) {
            startIndices[particleHandlers[i].particleHash] =  i;
            endIndices[particleHandlers[i-1].particleHash] = i;
        }
    }
	endIndices[particleHandlers[i-1].particleHash] = particleHandlers.size();
}

void ZIndexGrid::ReorderParticleData(){
    for(int i=0; i<numParticles; i++){
        reorderBuffer[i] = (*particles)[i];
    }
    for(int i=0; i<numParticles; i++){
        int oldIndex = particleHandlers[i].particleId;
        (*particles)[i] = reorderBuffer[oldIndex];
    }
}



