#include "Camera.h"

cmat4  cCamera::CameraRotateMat(){

	cfloat3 tmpright = cross(forceup,dir);
	tmpright /= sqrt(dot(tmpright, tmpright));
	right = tmpright;
	up = cross(dir,right); //update up

	cmat4 tmp = IDENTITY_MAT;
	tmp[0][0] = right.x;
	tmp[1][0] = right.y;
	tmp[2][0] = right.z;
	tmp[3][0] = 0; //new x

	tmp[0][1] = up.x;
	tmp[1][1] = up.y;
	tmp[2][1] = up.z;
	tmp[3][1] = 0; //new y

	tmp[0][2] = dir.x;
	tmp[1][2] = dir.y;
	tmp[2][2] = dir.z;
	tmp[3][2] = 0; //new z

	tmp[0][3] = 0;
	tmp[1][3] = 0;
	tmp[2][3] = 0;
	tmp[3][3] = 1; //new w
	return tmp;
}

void  cCamera::CameraTranslateMat(cmat4& rotation,cfloat3& pos){
	cmat4 tmp = IDENTITY_MAT;
	tmp[3][0]=-pos.x;
	tmp[3][1]=-pos.y;
	tmp[3][2]=-pos.z;

	rotation =  tmp * rotation;
}

void cCamera::AdvanceCamera(float dt){
	if (wasd.lenth==0) //no input
	{
		if (velf > 0.001)
			velf -= velf*10*dt;
	}
	else{
		velf += (velMax - velf)  * dt;
		getVelDir();
	}

	if (velf < 0.001 && dot(rotatexy, rotatexy)<0.001 && dot(transxy,transxy)<0.001) {
		return;
	}
	cfloat3 dx = vel * velf * dt;

	//translate
	pos = pos+dx;
	target = target+dx;

	//rotate
	
	rotatexy  = rotatexy * 0.01;
	target = target - up*rotatexy.y;
	target = target + right*rotatexy.x;

	transxy = transxy * 0.01;
	dx = up * (transxy.y) + right*(-transxy.x);
	dx = dx*100;
	pos = pos+dx;
	target = target+dx;

	lookat(pos, target);
	//printf("%f %f %f, %f %f %f\n",dir.x,dir.y,dir.z, vel.x,vel.y,vel.z);
}

void cCamera::lookat(cfloat3 _pos, cfloat3 _target) {
	dir = _pos - _target;
	dir = dir * (1/ sqrt(dot(dir, dir)));

	pos = _pos;
	target = pos - dir;

	viewmat = CameraRotateMat();
	CameraTranslateMat(viewmat, pos);
}