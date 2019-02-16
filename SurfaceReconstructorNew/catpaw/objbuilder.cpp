#include "objbuilder.h"
#include <stdio.h>

void ObjContainer::writeToFile(const char* filepath) {
	FILE* fp = fopen(filepath,"w+");
	for (int i=0; i<pointlist.size(); i++) {
		fprintf(fp,"v %f %f %f\n",pointlist[i].pos.x, pointlist[i].pos.y, pointlist[i].pos.z);
	}
	for (int i=0; i<trianglelist.size(); i++) {
		fprintf(fp,"f %d %d %d\n", trianglelist[i].plist[0]+1,trianglelist[i].plist[1]+1, trianglelist[i].plist[2]+1);
	}
	fclose(fp);
}


//create a rectangular, rownum and colnum indicate the number of vertices
//in each direction respectively
ObjContainer* ObjBuilder::build_rectangular(int rownum,int colnum, float spacing) {

	ObjContainer* newobj = new ObjContainer;
	//vertex list
	for(int i=0; i<rownum; i++)
		for (int j=0; j<colnum; j++) {
			float x = j*spacing - colnum/2;
			float y = i*spacing - rownum/2;
			float z = (float)rand()/(5.0f)/ RAND_MAX;
			objvertex v;
			v.pos.Set(x,y,z);
			newobj->addvertex(v);
		}
	newobj->vertex_triangle.resize(newobj->pointlist.size());
	
	//triangle list
	for (int i=0; i<rownum-1; i++) {
		for (int j=0; j<colnum-1; j++) {
			objtriangle t;
			int tid;
			int p1,p2,p3;
			
			p1 = i*colnum + j;
			p2 = p1+1;
			p3 = p1+colnum;
			newobj->addtriangle(p1,p2,p3);

			p1 = (i+1)*colnum + j;
			p2 = i*colnum+j+1;
			p3 = p1+1;
			newobj->addtriangle(p1,p2,p3);
		}
	}
	return newobj;
}

void switchInt(int& a, int &b) {
	int c = a;
	a = b;
	b = c;
}

void sort(int* plist) {
	int temp;
	if (plist[0]<plist[1])
		switchInt(plist[0],plist[1]);
	if(plist[1]<plist[2])
		switchInt(plist[1],plist[2]);
	if (plist[0]<plist[1])
		switchInt(plist[0], plist[1]);
}


void ObjContainer::buildEdgeList() {
	//handle default case
	if(pointlist.size()==0)
		return;

	for (int tid=0; tid!=trianglelist.size(); tid++) {
		int plist[3];
		for(int i=0; i<3; i++)//copy
			plist[i] = trianglelist[tid].plist[i];

		//max,medium,min
		sort(plist);
		//printf("%d %d %d\n",plist[0],plist[1],plist[2]);
		edge_triangles[make_pair(plist[0],plist[1])].push_back(tid);
		edge_triangles[make_pair(plist[0],plist[2])].push_back(tid);
		edge_triangles[make_pair(plist[1],plist[2])].push_back(tid);
	}

	map<pair<int,int>, vector<int> >::iterator miter;
	for (miter=edge_triangles.begin(); miter!=edge_triangles.end(); miter++) {
		
		assert(miter->second.size()==1 || miter->second.size()==2);

		edgeConstraint ec;
		int p1=miter->first.first;
		int p2=miter->first.second;
		//along edge
		ec.p1 = p1;
		ec.p2 = p2;
		//cross edge
		int p3=-1,p4=-1;
		ec.t1 = miter->second[0];
		ec.t2 = -1;

		//if(ec.p1<10 || ec.p2<10)
		//	continue;

		if (miter->second.size()==2){
			int t1=miter->second[0];
			int t2=miter->second[1];
			ec.t2 = t2;

			for (int j=0; j<3; j++) {
				if (trianglelist[t1].plist[j]!=p1 && trianglelist[t1].plist[j]!=p2) {
					p3=trianglelist[t1].plist[j];
					break;
				}
			}
			for(int j=0;j<3;j++){
				if (trianglelist[t2].plist[j]!=p1 && trianglelist[t2].plist[j]!=p2) {
					p4=trianglelist[t2].plist[j];
				}
			}
		}
		ec.p3=p3;
		ec.p4=p4;
		edgeConstraints.push_back(ec);
	}

	//DEBUG
#ifdef DEBUG
	for (int i=0; i<pointlist.size(); i++) {
		edgeConstraint& ec = edgeConstraints[i];
		printf("%d %d %d %d\n",ec.p1, ec.p2, ec.p3, ec.p4);
	}
#endif
}


inline bool operator<(cfloat3 a, cfloat3 b) { // for use in map
	if (a.x < b.x) return true;
	if (a.x > b.x) return false;
	if (a.y < b.y) return true;
	if (a.y > b.y) return false;
	if (a.z < b.z) return true;
	if (a.z > b.z) return false;
	return false;
};

ObjContainer* ObjReader::readraw(char* filepath) {
	FILE* fp = fopen(filepath, "r");
	if(fp==NULL)
		return NULL;

	ObjContainer* oc = new ObjContainer();
	map<cfloat3,int> vertmap;
	int vid=0;
	cfloat3 a,b,c;

	while (true) {
		fscanf(fp, "%f %f %f", &a.x, &a.y, &a.z);
		fscanf(fp, "%f %f %f", &b.x, &b.y, &b.z);
		int tmp = fscanf(fp, "%f %f %f\n", &c.x, &c.y, &c.z);
		if(tmp==EOF)
			break;

		if (vertmap.count(a)==0)
			vertmap[a]=vid++;
		if (vertmap.count(b)==0)
			vertmap[b]=vid++;
		if (vertmap.count(c)==0)
			vertmap[c]=vid++;

		objtriangle t;
		t.plist[0]=vertmap[a];
		t.plist[1]=vertmap[b];
		t.plist[2]=vertmap[c];
		oc->trianglelist.push_back(t);

		oc->pointlist.resize(vid);
		for (const auto& p: vertmap) {
			oc->pointlist[p.second].pos = p.first;
		}
	}
	return oc;
	
}

ObjContainer* ObjReader::readobj(char* filepath) {
	FILE* fp = fopen(filepath, "r");
	if (fp==NULL)
		return NULL;

	ObjContainer* oc = new ObjContainer();
	map<cfloat3, int> vertmap;
	int vid=0;
	cfloat3 a, b, c;
	int tmpc;
	int p1,p2,p3;

	while (true) {
		tmpc = fgetc(fp);
		if (tmpc=='v') {
			fscanf(fp,"%f %f %f\n",&a.x, &a.y, &a.z);
			objvertex v;
			v.pos = a;
			oc->pointlist.push_back(v);
		}
		else if (tmpc=='f') {
			fscanf(fp,"%d %d %d\n",&p1, &p2, &p3);
			objtriangle tri;
			tri.plist[0]=p1-1;
			tri.plist[1]=p2-1;
			tri.plist[2]=p3-1;
			oc->trianglelist.push_back(tri);
		}
		else if(tmpc==EOF)
			break;
		else {
			while (true) {
				if(tmpc=='\n')
					break;
				tmpc = fgetc(fp);
			}
		}
	}

	printf("triangleNum: %d\n", oc->trianglelist.size());
	return oc;
}

ObjContainer* ObjReader::readobj_n(char* filepath) {
	FILE* fp = fopen(filepath, "r");
	if (fp==NULL)
		return NULL;

	ObjContainer* oc = new ObjContainer();
	map<cfloat3, int> vertmap;
	int vid=0;
	cfloat3 a, b, c;
	int tmpc;
	int p1, p2, p3;
	int n1,n2,n3;

	while (true) {
		tmpc = fgetc(fp);
		if (tmpc=='v') {
			if(fgetc(fp)=='n')
				goto skip;
			fscanf(fp, "%f %f %f\n", &a.x, &a.y, &a.z);
			objvertex v;
			v.pos = a;
			oc->pointlist.push_back(v);
		}
		else if (tmpc=='f') {
			fscanf(fp, "%d//%d %d//%d %d//%d\n", &p1,&n1, &p2,&n2, &p3,&n3);
			objtriangle tri;
			tri.plist[0]=p1-1;
			tri.plist[1]=p2-1;
			tri.plist[2]=p3-1;
			oc->trianglelist.push_back(tri);
		}
		else if (tmpc==EOF)
			break;
		else {
skip:
			while (true) {
				tmpc = fgetc(fp);
				if (tmpc=='\n')
					break;
			}
		}
	}
	return oc;
}