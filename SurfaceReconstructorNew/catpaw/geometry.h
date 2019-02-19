#pragma once

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifdef BUILD_CUDA
#include <host_defines.h>
#define HDFUNC __host__ __device__

#else
#define HDFUNC
#define __inline__ inline
#endif

#define EPSILON 1e-10

typedef unsigned int uint;
typedef unsigned char uchar;

HDFUNC inline float cmin(float a, float b) {
	return a < b ? a : b;
}


inline float cmax(float a, float b) {
	return a > b ? a : b;
}



struct cfloat2 {
	float x,y;
	cfloat2(){}
	cfloat2(float _x,float _y):x(_x),y(_y){}
	void Set(float _x,float _y){ x=_x; y=_y; }
	cfloat2 operator*(float s){
		return cfloat2(x*s, y*s);
	}
	cfloat2 operator/(float s){
		return cfloat2(x/s,y/s);
	}
	cfloat2 operator-(cfloat2 b){
		return cfloat2(x-b.x,y-b.y);
	}
	cfloat2 operator+(cfloat2 b){
		return cfloat2(x+b.x, y+b.y);
	}
};


inline float dot(cfloat2& a, cfloat2& b) {
	return a.x*b.x + a.y*b.y;
}





struct cfloat3 {
	float x, y, z;
	HDFUNC cfloat3() {x=y=z=0;}
	HDFUNC cfloat3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
	HDFUNC void Set(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	HDFUNC cfloat3 operator- (cfloat3 b) {
		return cfloat3(x - b.x, y - b.y, z - b.z);
	}
	HDFUNC cfloat3 operator+(cfloat3 b) {
		return cfloat3(x + b.x, y + b.y, z + b.z);
	}
	HDFUNC cfloat3 operator*(cfloat3 b) {
		return cfloat3(x * b.x, y * b.y, z * b.z);
	}
	HDFUNC cfloat3 operator/(cfloat3 b) {
		return cfloat3(x / b.x, y / b.y, z / b.z);
	}
	HDFUNC cfloat3 operator/(float b){
		return cfloat3(x/b,y/b,z/b);
	}

	HDFUNC cfloat3 operator*(float s) {
		return cfloat3(x*s, y*s, z*s);
	}
	HDFUNC cfloat3 operator+(float a) {
		return cfloat3(x + a, y + a, z + a);
	}
	HDFUNC cfloat3 operator-(float a) {
		return cfloat3(x - a, y - a, z - a);
	}
	HDFUNC void operator+= (cfloat3 b) {
		x+=b.x; y+=b.y; z+=b.z;
	}
	HDFUNC void operator-= (cfloat3 b){
		x-=b.x; y-=b.y; z-=b.z;
	}
	HDFUNC void operator*= (float b) {
		x*=b; y*=b; z*=b;
	}
	HDFUNC void operator/= (float b) {
		x/=b; y/=b; z/=b;
	}
	HDFUNC void operator /= (cfloat3 b){
		x/=b.x; y/=b.y; z/=b.z;
	}

	HDFUNC inline float minx() {
		return cmin(x, cmin(y, z));
	}
	HDFUNC inline float Norm() {
		return sqrt(x*x+y*y+z*z);
	}
	HDFUNC inline float square() {
		return x*x + y*y + z*z;
	}
};

HDFUNC inline cfloat3 minfilter(cfloat3 a, cfloat3 b) {
	return cfloat3(cmin(a.x, b.x), cmin(a.y, b.y), cmin(a.z, b.z));
}

inline cfloat3 maxfilter(cfloat3 a, cfloat3 b) {
	return cfloat3(cmax(a.x, b.x), cmax(a.y, b.y), cmax(a.z, b.z));
}

HDFUNC inline cfloat3 cross(cfloat3 a, cfloat3 b) {
	cfloat3 tmp;
	tmp.x = a.y*b.z - a.z*b.y;
	tmp.y = a.z*b.x - a.x*b.z;
	tmp.z = a.x*b.y - a.y*b.x;
	return tmp;
}

HDFUNC inline float dot(cfloat3& a,cfloat3& b){
	return a.x*b.x+a.y*b.y+a.z*b.z;
}


float angle(cfloat3& a,cfloat3& b);





struct cint3 {
	int x, y, z;
	HDFUNC cint3() {}
	HDFUNC cint3(int _x, int _y, int _z) :x(_x), y(_y), z(_z) {}
	HDFUNC void Set(int _x, int _y, int _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	HDFUNC operator cfloat3(){
		return cfloat3(x,y,z);
	}

	HDFUNC int prod() {
		return x*y*z;
	}
	
	HDFUNC cint3 operator + (cint3 b) {
		return cint3(x+b.x, y+b.y, z+b.z);
	}
	HDFUNC cfloat3 operator * (float b) {
		return cfloat3(x*b, y*b, z*b);
	}
};

HDFUNC inline cint3 floor(cfloat3 a) {
	return cint3(floor(a.x), floor(a.y), floor(a.z));
}
HDFUNC inline cint3 ceil(cfloat3 a) {
	return cint3(ceil(a.x), ceil(a.y), ceil(a.z));
}





struct cmat3 
{
	float data[9];
	HDFUNC cmat3() {
		for(int i=0;i<9;i++) data[i]=0;
	}
	HDFUNC cmat3(float mat[]) { //make sure mat longer than 9
		for(int i=0;i<9;i++) data[i]=mat[i];
	}

	HDFUNC void Set(float mat[]) {
		for (int i=0; i<9; i++) data[i] = mat[i];
	}
	HDFUNC void Set(float c) {
		for(int i=0;i<9;i++) data[i]=c;
	}
	HDFUNC void Print() {
		for (int i=0; i<3; i++) {
			printf("%f %f %f\n", data[i*3], data[i*3+1], data[i*3+2]);
		}
	}
	HDFUNC float* operator[](int i){
		return &data[i*3];
	}
	HDFUNC cmat3 operator+(cmat3& b) {
		cmat3 res;
		for (int k=0; k<9; k++) {
			res.data[k]=data[k]+b.data[k];
		}
		return res;
	}
	HDFUNC cmat3 operator*(float b) {
		cmat3 res;
		for (int k=0; k<9; k++) {
			res.data[k] = data[k] * b;
		}
		return res;
	}

	HDFUNC float Det() {
		return data[0]*(data[4]*data[8]-data[5]*data[7]) + data[1]*(data[5]*data[6]-data[3]*data[8]) + data[2]*(data[3]*data[7]-data[4]*data[6]);
	}
	HDFUNC cmat3 Inv() {
		cmat3 inv;
		float det = Det();
		if (fabs(det)<1e-10) {
			return inv;
		}
		inv[0][0] = (data[4]*data[8]-data[5]*data[7])/det;
		inv[0][1] = (data[2]*data[7]-data[1]*data[8])/det;
		inv[0][2] = (data[1]*data[5]-data[2]*data[4])/det;
		inv[1][0] = (data[5]*data[6]-data[3]*data[8])/det;
		inv[1][1] = (data[0]*data[8]-data[2]*data[6])/det;
		inv[1][2] = (data[2]*data[3]-data[0]*data[5])/det;
		inv[2][0] = (data[3]*data[7]-data[4]*data[6])/det;
		inv[2][1] = (data[1]*data[6]-data[0]*data[7])/det;
		inv[2][2] = (data[0]*data[4]-data[1]*data[3])/det;
		return inv;
	}

	HDFUNC float Norm() {
		float norm=0;
		for(int k=0;k<9;k++)
			norm += data[k]*data[k];
		return sqrt(norm);
	}

	HDFUNC void Add(cmat3& b) {
		for(int k=0; k<9; k++)
			data[k] = data[k] + b.data[k];
	}
	HDFUNC void Multiply(float b) {
		for(int k=0;k<9;k++)
			data[k] = data[k]*b;
	}
	HDFUNC void Minus(cmat3& b) {
		for(int k=0; k<9; k++)
			data[k] -= b.data[k];
	}

	HDFUNC cfloat3 Col(int n) {
		if (n<0 || n>2) {
			printf("Illegal Matrix Column Visit.\n");
			return cfloat3(0,0,0);
		}
		return cfloat3(data[n], data[3+n], data[6+n]);
	}
};


HDFUNC __inline__ cmat3 TensorProduct(cfloat3& a, cfloat3& b) {
	cmat3 res;
	res[0][0] = a.x*b.x; res[0][1] = a.x*b.y; res[0][2] = a.x*b.z;
	res[1][0] = a.y*b.x; res[1][1] = a.y*b.y; res[1][2] = a.y*b.z;
	res[2][0] = a.z*b.x; res[2][1] = a.z*b.y; res[2][2] = a.z*b.z;
	return res;
}

HDFUNC __inline__ void  mat3add(cmat3& a, cmat3& b, cmat3& c) {
	for (int i=0; i<9; i++)
		c.data[i] = a.data[i]+b.data[i];
}

HDFUNC __inline__ void  mat3sub(cmat3& a, cmat3& b, cmat3& c) {
	for (int i=0; i<9; i++)
		c.data[i] = a.data[i]-b.data[i];
}

HDFUNC __inline__ void  mat3prod(cmat3& a, cmat3& b, cmat3& c) {
	c[0][0] = a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0];
	c[0][1] = a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1];
	c[0][2] = a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2];

	c[1][0] = a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0];
	c[1][1] = a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1];
	c[1][2] = a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2];

	c[2][0] = a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0];
	c[2][1] = a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1];
	c[2][2] = a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2];
}

HDFUNC __inline__ void mat3transpose(cmat3& a, cmat3& b) {
	cmat3 tmp;
	tmp[0][0]=a[0][0]; tmp[0][1]=a[1][0]; tmp[0][2]=a[2][0];
	tmp[1][0]=a[0][1]; tmp[1][1]=a[1][1]; tmp[1][2]=a[2][1];
	tmp[2][0]=a[0][2]; tmp[2][1]=a[1][2]; tmp[2][2]=a[2][2];
	b=tmp;
}

/*
Compute the product of cmat3 m and cfloat3 v,
storing the result into cfloat3 c.
c and v can be the same vector.
*/
HDFUNC __inline__ void  mvprod(cmat3& m, cfloat3& v, cfloat3& c) {
	cfloat3 tmp;
	tmp.x = m.data[0]*v.x + m.data[1]*v.y + m.data[2]*v.z;
	tmp.y = m.data[3]*v.x + m.data[4]*v.y + m.data[5]*v.z;
	tmp.z = m.data[6]*v.x + m.data[7]*v.y + m.data[8]*v.z;
	c=tmp;
}



void RotateX(cfloat3& a, float b);
void RotateY(cfloat3& a, float b);
void RotateZ(cfloat3& a, float b);
void RotateXYZ(cfloat3& a, cfloat3& xyz);


HDFUNC __inline__ void AxisAngle2Matrix(cfloat3 axis, float angle, cmat3& mat)
{
	//Do the normalization if needed.
	float l = axis.Norm();
	if (fabs(l-1)>EPSILON)
		axis /= l;
	float s = sin(angle);
	float c = cos(angle);
	float t = 1 - c;
	mat[0][0] = t*axis.x*axis.x + c;
	mat[1][1] = t*axis.y*axis.y + c;
	mat[2][2] = t*axis.z*axis.z + c;

	float tmp1 = axis.x*axis.y*t;
	float tmp2 = axis.z*s;
	mat[1][0] = tmp1+tmp2;
	mat[0][1] = tmp1-tmp2;
	tmp1 = axis.x*axis.z*t;
	tmp2 = axis.y*s;
	mat[2][0] = tmp1 - tmp2;
	mat[0][2] = tmp1 + tmp2;
	tmp1 = axis.y*axis.z*t;
	tmp2 = axis.x*s;
	mat[2][1] = tmp1+tmp2;
	mat[1][2] = tmp1-tmp2;
}



/* -----------------------------------


	            OpenGL Utility


-------------------------------------*/

const float cPI = 3.1415926536;

struct cvertex{
	float position[4];
	float color[4];
};

struct cfloat4{
	float x,y,z,w;
	HDFUNC void Set(float _x,float _y,float _z,float _w){
		x=_x; y=_y; z=_z; w=_w;
	}
	HDFUNC cfloat4():
		x(0),y(0),z(0),w(0){}
	HDFUNC cfloat4(float _x, float _y, float _z, float _w):
		x(_x),y(_y),z(_z),w(_w){}
	HDFUNC cfloat4(cfloat3 x3, float _w):
		x(x3.x), y(x3.y), z(x3.z), w(_w){}
};

//watch out, column major
struct cmat4{
	float data[16];
	float* operator[](int i){
		return &data[i*4];
	}
	cmat4 operator*(cmat4& b){
		cmat4 tmp;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				tmp[i][j] = data[0+i*4]*b[0][j] + data[1+i*4]*b[1][j]
					+ data[2+i*4]*b[2][j] + data[3+i*4]*b[3][j];
			}
		}
		return tmp;
	}

	cfloat4 operator*(cfloat4& b) {
		cfloat4 tmp;
		tmp.x = data[0*4+0]*b.x + data[1*4+0]*b.y + data[2*4+0]*b.z + data[3*4+0]*b.w;
		tmp.y = data[0*4+1]*b.x + data[1*4+1]*b.y + data[2*4+1]*b.z + data[3*4+1]*b.w;
		tmp.z = data[0*4+2]*b.x + data[1*4+2]*b.y + data[2*4+2]*b.z + data[3*4+2]*b.w;
		tmp.w = data[0*4+3]*b.x + data[1*4+3]*b.y + data[2*4+3]*b.z + data[3*4+3]*b.w;
		return tmp;
	}
	
};


extern const cmat4 IDENTITY_MAT;

float cotangent(float angle);
float deg2rad(float deg);
float rad2deg(float rad);

void RotateAboutX(cmat4& m, float ang);
void RotateAboutY(cmat4& m, float ang);
void RotateAboutZ(cmat4& m, float ang);
void ScaleMatrix(cmat4& m, cfloat3 x);
void TranslateMatrix(cmat4& m, cfloat3 x);

cmat4 CreateProjectionMatrix(float fovy, float aspect_ratio,
	float near_plane, float far_plane);

struct vertex {
	cfloat3 pos;
	cfloat4 color;
};