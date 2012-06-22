#ifndef __glox__
#define __glox__

#include<iostream>
using namespace std;

#include<sstream>

namespace glox{
	namespace clk{
		int dtms=10;
		float dt=dtms/1000.f;
		clock_t t0=clock();
		clock_t t1=t0;
		int tk=0;
		inline void timerrestart(){t1=clock();}
		inline clock_t timerdclk(){return clock()-t1;}
		inline float timerdt(){return (float)(clock()-t1)/CLOCKS_PER_SEC;}
	}
	namespace metrics{
		int globs;
		int coldetsph;
		int frames;
		float dtupd;
		float dtrend;
		int p3s;
		int m3s;
		int bvols;
		int collisions;
		int globos;
		int f3s;
		int mwrefresh;
		int mpmul;
		int mmmul;
		int ngrids;
		float dtgrd;
		float dtcoldetbrute;
	}
	inline float dt(const float f=1){return f*clk::dt;}
	inline float rnd(const float from,const float tonotincluding){
		return from+(tonotincluding-from)*rand()/RAND_MAX;
	}
	const float pi=3.1415926f;
	const float degtoradk=pi/180;
	inline float degtorad(const float deg=1){return deg*degtoradk;}
	ostringstream sts,inp;
}
using namespace glox;

#include<cmath>

class p3{
	float x,y,z;
public:
	inline p3():x(0),y(0),z(0){metrics::p3s++;}
	inline p3(const p3&p){x=p.x;y=p.y;z=p.z;metrics::p3s++;}
	inline p3(const float x,const float y,const float z):x(x),y(y),z(z){metrics::p3s++;}
	inline p3(const p3&from,const p3&to):x(to.x-from.x),y(to.y-from.y),z(to.z-from.z){metrics::p3s++;}
	inline ~p3(){metrics::p3s--;}

	inline float getx()const{return x;}
	inline float gety()const{return y;}
	inline float getz()const{return z;}
	inline p3&transl(const float dx,const float dy,const float dz){x+=dx;y+=dy;z+=dz;return*this;}
	inline p3&transl(const p3&dp){x+=dp.x;y+=dp.y;z+=dp.z;return*this;}
	inline p3&transl(const p3&dp,const float dt){x+=dp.x*dt;y+=dp.y*dt;z+=dp.z*dt;return*this;}
	inline float magn()const{return sqrt(x*x+y*y+z*z);}
	inline p3&set(const p3&p){x=p.x;y=p.y;z=p.z;return*this;}
	inline p3&set(const float x,const float y,const float z){this->x=x;this->y=y;this->z=z;return*this;}
	inline p3&neg(){x=-x;y=-y;z=-z;return*this;}
	inline p3&scale(const float s){x*=s;y*=s;z*=s;return*this;}
	inline p3&scale(const float sx,const float sy,const float sz){x*=sx;y*=sy;z*=sz;return*this;}
	inline bool operator==(const p3&p)const{return x==p.x&&y==p.y&&z==p.z;}
	friend ostream&operator<<(ostream&,const p3&);
	friend istream&operator>>(istream&,p3&);
};
inline ostream&operator<<(ostream&os,const p3&p){os<<p.x<<" "<<p.y<<" "<<p.z;return os;}
inline istream&operator>>(istream&is,p3&p){is>>p.x;is.ignore();is>>p.y;is.ignore();is>>p.z;return is;}

#include<execinfo.h>

class signl{
	const int i;
	const char*s;
public:
	signl(const int i,const char*s):i(i),s(s){
		cout<<" ••• signl "<<i<<" · "<<s<<endl;
        const int nva=10;
		void*va[nva];
		int n=backtrace(va,nva);
		backtrace_symbols_fd(va,n,1);
	}
	inline int num()const{return i;}
	inline const char* str()const{return s;}
};
//
//template<class T>class arai{
//private:
//	T*ar;
//	int of,ln;
//	inline void asrt(const int offset,const int len)const{if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofboundsinrw");}
//public:
//	inline arai(T a[],const int offset,const int len):ar(a),of(offset),ln(len){}
//	inline T&operator[](const int i)const{asrt(i,1);return ar[of+i];}
//	inline int ofs()const{return of;}
//	inline int len()const{return ln;}
//	inline void ro(const int offset,const int len,void(*f)(const T&e))const{if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofboundsinro");T*p=ar+of+offset;int i=len;while(i--)(*f)(*p++);}
//	inline void rw(const int offset,const int len,void(*f)(T&e)){asrt(offset,len);T*p=ar+of+offset;int i=len;while(i--)(*f)(*p++);}
//};
//
#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#include <glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define flf()l("  ",__FILE__,__LINE__,__FUNCTION__);
static inline ostream&l(const char*s="",const char*file="",int lineno=0,const char*func=""){cerr<<file;if(lineno){cerr<<":"<<lineno;}cerr<<" "<<func<<"  "<<s;return cerr;}






bool gluInvertMatrix(const float m[16],float invOut[16]){
	double inv[16],det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

	det=m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];
	if(det==0)
		return false;

	det=1.0/det;
	for(i=0;i<16;i++)
		invOut[i]=(float)(inv[i]*det);
	return true;
}










class m3{
	float xx,yx,zx,ox;
	float xy,yy,zy,oy;
	float xz,yz,zz,oz;
	float xo,yo,zo,oo;
	m3&rotx(const float a){
		float c=cos(a),s=sin(a);
		float nyx=yx*c+zx*s,nyy=yy*c+zy*s,nyz=yz*c+zz*s,nyo=yo*c+zo*s;
		float nzx=zx*c-yx*s,nzy=zy*c-yy*s,nzz=zz*c-yz*s,nzo=zo*c-yo*s;
		yx=nyx;yy=nyy;yz=nyz;yo=nyo;
		zx=nzx;zy=nzy;zz=nzz;zo=nzo;
		return*this;
	}
	m3&roty(const float a){
		float c=cos(a),s=sin(a);
		float nxx=xx*c+zx*s,nxy=xy*c+zy*s,nxz=xz*c+zz*s,nxo=xo*c+zo*s;
		float nzx=zx*c-xx*s,nzy=zy*c-xy*s,nzz=zz*c-xz*s,nzo=zo*c-xo*s;
		xx=nxx;xy=nxy;xz=nxz;xo=nxo;
		zx=nzx;zy=nzy;zz=nzz;zo=nzo;
		return*this;
	}
	m3&rotz(const float a){
		float c=cos(a),s=sin(a);
		float nyx=yx*c+xx*s,nyy=yy*c+xy*s,nyz=yz*c+xz*s,nyo=yo*c+xo*s;
		float nxx=xx*c-yx*s,nxy=xy*c-yy*s,nxz=xz*c-yz*s,nxo=xo*c-yo*s;
		xx=nxx;xy=nxy;xz=nxz;xo=nxo;
		yx=nyx;yy=nyy;yz=nyz;yo=nyo;
		return*this;
	}
	m3&transl(const p3&p){
		const float x=p.getx();
		const float y=p.gety();
		const float z=p.getz();
		xo=xx*x+xy*y+xz*z+xo;
		yo=yx*x+yy*y+yz*z+yo;
		zo=zx*x+zy*y+zz*z+zo;
		return*this;
	}

public:
	inline m3(){metrics::m3s++;ident();}
	inline ~m3(){metrics::m3s--;}
	m3&ident(){xx=1;xy=0;xz=0;xo=0; yx=0;yy=1;yz=0;yo=0; zx=0;zy=0;zz=1;zo=0; ox=oy=oz=0; oo=1;return*this;}
	p3 xaxis()const{return p3(xx,xy,xz);}
	p3 yaxis()const{return p3(yx,yy,yz);}
	p3 zaxis()const{return p3(zx,zy,zz);}
	m3&mw(const p3&p,const p3&a){
		ident();
		rotz(degtorad(a.getz()));
		roty(degtorad(a.gety()));
		rotx(degtorad(a.getx()));
		xo=p.getx();
		yo=p.gety();
		zo=p.getz();
		return*this;
	}
	const m3&mult(const p3&src,p3&dst)const{
		metrics::mpmul++;
		const float x=src.getx();
		const float y=src.gety();
		const float z=src.getz();
		float rx=x*xx+y*xy+z*xz+xo;
		float ry=x*yx+y*yy+z*yz+yo;
		float rz=x*zx+y*zy+z*zz+zo;
		dst.set(rx,ry,rz);
		return*this;
	}
	m3&set(const GLfloat m[16]){
		xx=m[ 0];yx=m[ 1];zx=m[ 2];ox=m[ 3];
		xy=m[ 4];yy=m[ 5];zy=m[ 6];oy=m[ 7];
		xz=m[ 8];yz=m[ 9];zz=m[10];oz=m[11];
		xo=m[12];yo=m[13];zo=m[14];oo=m[15];
		return*this;
	}
//	m3&rottrnsl(const p3&p,const p3&a){
//		return*this;
//	}
	m3 inv()const{
		GLfloat m[16];
		m[0 ]=xx;m[ 1]=yx;m[ 2]=zx;m[3]=ox;
		m[4 ]=xy;m[ 5]=yy;m[ 6]=zy;m[7]=oy;
		m[8 ]=xz;m[ 9]=yz;m[10]=zz;m[11]=oz;
		m[12]=xo;m[13]=yo;m[14]=zo;m[15]=oo;

		GLfloat mout[16];
		gluInvertMatrix(m,mout);

		m3 mx;
		mx.set(mout);
		return mx;
	}
	m3&mult(const m3&m){
		metrics::mmmul++;
		float nxx=m.xx*xx+m.yx*xy+m.zx*xz+m.ox*xo;
		float nyx=m.xx*yx+m.yx*yy+m.zx*yz+m.ox*yo;
		float nzx=m.xx*zx+m.yx*zy+m.zx*zz+m.ox*zo;
		float nox=m.xx*ox+m.yx*oy+m.zx*oz+m.ox*oo;

		float nxy=m.xy*xx+m.yy*xy+m.zy*xz+m.oy*xo;
		float nyy=m.xy*yx+m.yy*yy+m.zy*yz+m.oy*yo;
		float nzy=m.xy*zx+m.yy*zy+m.zy*zz+m.oy*zo;
		float noy=m.xy*ox+m.yy*oy+m.zy*oz+m.oy*oo;

		float nxz=m.xz*xx+m.yz*xy+m.zz*xz+m.oz*xo;
		float nyz=m.xz*yx+m.yz*yy+m.zz*yz+m.oz*yo;
		float nzz=m.xz*zx+m.yz*zy+m.zz*zz+m.oz*zo;
		float noz=m.xz*ox+m.yz*oy+m.zz*oz+m.oz*oo;

		float nxo=m.xo*xx+m.yo*xy+m.zo*xz+m.oo*xo;
		float nyo=m.xo*yx+m.yo*yy+m.zo*yz+m.oo*yo;
		float nzo=m.xo*zx+m.yo*zy+m.zo*zz+m.oo*zo;
		float noo=m.xo*ox+m.yo*oy+m.zo*oz+m.oo*oo;

		xx=nxx;yx=nyx;zx=nzx;ox=nox;
		xy=nxy;yy=nyy;zy=nzy;oy=noy;
		xz=nxz;yz=nyz;zz=nzz;oz=noz;
		xo=nxo;yo=nyo;zo=nzo;oo=noo;

		return*this;
	}
//	m3&mult(const m3&m){
//		metrics::mmmul++;
//		float nxx=xx*m.xx+yx*m.xy+zx*m.xz+ox*m.xo;
//		float nyx=xx*m.yx+yx*m.yy+zx*m.yz+ox*m.yo;
//		float nzx=xx*m.zx+yx*m.zy+zx*m.zz+ox*m.zo;
//		float nox=xx*m.ox+yx*m.oy+zx*m.oz+ox*m.oo;
//
//		float nxy=xy*m.xx+yy*m.xy+zy*m.xz+oy*m.xo;
//		float nyy=xy*m.yx+yy*m.yy+zy*m.yz+oy*m.yo;
//		float nzy=xy*m.zx+yy*m.zy+zy*m.zz+oy*m.zo;
//		float noy=xy*m.ox+yy*m.oy+zy*m.oz+oy*m.oo;
//
//		float nxz=xz*m.xx+yz*m.xy+zz*m.xz+oz*m.xo;
//		float nyz=xz*m.yx+yz*m.yy+zz*m.yz+oz*m.yo;
//		float nzz=xz*m.zx+yz*m.zy+zz*m.zz+oz*m.zo;
//		float noz=xz*m.ox+yz*m.oy+zz*m.oz+oz*m.oo;
//
//		float nxo=xo*m.xx+yo*m.xy+zo*m.xz+oo*m.xo;
//		float nyo=xo*m.yx+yo*m.yy+zo*m.yz+oo*m.yo;
//		float nzo=xo*m.zx+yo*m.zy+zo*m.zz+oo*m.zo;
//		float noo=xo*m.ox+yo*m.oy+zo*m.oz+oo*m.oo;
//
//		xx=nxx;yx=nyx;zx=nzx;ox=nox;
//		xy=nxy;yy=nyy;zy=nzy;oy=noy;
//		xz=nxz;yz=nyz;zz=nzz;oz=noz;
//		xo=nxo;yo=nyo;zo=nzo;oo=noo;
//
//		return*this;
//	}
	friend ostream&operator<<(ostream&,const m3&);
	friend istream&operator>>(istream&,m3&);
};
ostream&operator<<(ostream&os,const m3&m){
	cout<<"["<<p3(m.xx,m.yx,m.zx)<<" "<<m.ox<<"] [";
	cout<<p3(m.xy,m.yy,m.zy)<<" "<<m.oy<<"] [";
	cout<<p3(m.xz,m.yz,m.zz)<<" "<<m.oz<<"] [";
	cout<<p3(m.xo,m.yo,m.zo)<<" "<<m.oo<<"]";
	return os;
}
istream&operator>>(istream&is,m3&m){
	p3 p;
	is.ignore();
	is>>p;
	m.xx=p.getx();m.yx=p.gety();m.zx=p.getz();
	is.ignore();
	is>>m.ox;
	is.ignore(3);
	is>>p;
	m.xy=p.getx();m.yy=p.gety();m.zy=p.getz();
	is.ignore();
	is>>m.oy;
	is.ignore(3);
	is>>p;
	m.xz=p.getx();m.yz=p.gety();m.zz=p.getz();
	is.ignore();
	is>>m.oz;
	is.ignore(3);
	is>>p;
	m.xo=p.getx();m.yo=p.gety();m.zo=p.getz();
	is.ignore();
	is>>m.oo;
	is.ignore();
	return is;
}

class glob;
class coll{
public:
	const glob&obja;
	const p3&vtxincol;
	const glob&objb;
	const p3&planevtx;
	const p3&planenml;
	const float dotpd;
	const float t;
};
class bvol{
	float r;
	p3 v;
public:
	bvol(const float sphereradius,const p3&boxcorner):r(sphereradius),v(boxcorner){metrics::bvols++;}
	~bvol(){metrics::bvols--;}
	inline float getradius()const{return r;}
	bvol&setradius(const float r){this->r=r;return*this;}
	bool spheresoverlap(const p3&p,const p3&pb,const bvol&b){
		metrics::coldetsph++;
		const p3 vec=p3(p,pb);
		const float dst=vec.magn();
		if(dst>(r+b.r))
			return false;
		return true;
	}
	bool anyboxdotinboxof(const p3&p,const m3&m,const bvol&bv){
		flf();l()<<p<<m<<bv<<endl;
		return false;
	}
	friend ostream&operator<<(ostream&,const bvol&);
	friend istream&operator>>(istream&,bvol&);
};
ostream&operator<<(ostream&os,const bvol&b){os<<b.r<<",("<<b.v<<")";return os;}
istream&operator>>(istream&is,bvol&bv){is>>bv.r;is.ignore(2);is>>bv.v;is.ignore();return is;}

//#include<vector>
#include <list>

class glob:public p3{
protected:
	const int id;
	glob&g;
	p3 a;
	list<glob*>chs;
	list<glob*>chsrm;
	list<glob*>chsadd;
	int bits;

	m3 mxmw;
	p3 mxmwpos;
	p3 mxmwagl;

	m3 mmw;
	bvol bv;
public:
	static bool drawboundingspheres;
	static int drawboundingspheresdetail;

	glob(glob&g,const p3&p=p3(),const p3&a=p3(),const float r=0,const p3&pbox=p3()):p3(p),id(metrics::globs++),g(g),a(a),bits(1),bv(r,pbox),rmed(false){
		if(&g==0)
			return;
		g.chsadd.push_back(this);
	}
	virtual~glob(){
//		cout<<"~glob("<<id<<")";
		metrics::globs--;
//		auto f=[]{cout<<"hello lambda"<<endl;};f();
		for(auto g:chs)delete g;
		chs.clear();
	}
	inline const bvol&getbvol()const{return bv;}
	inline bool issolid(){return bits&1;}
	inline bool isblt(){return bits&2;}
	inline bool iscoldetrec()const{return bits&4;}
	inline bool isfood()const{return bits&8;}

	inline int getid()const{return id;}
	bool refreshmxmw(){
		if(!&g)
			return false;
		bool refrsh=g.refreshmxmw();
		if(!refrsh){
			if(mxmwpos==*this&&mxmwagl==a){
				return false;
			}
		}
		metrics::mwrefresh++;

		mxmwagl=a;
		mxmwpos=*this;

		m3 m;
		m.ident();
		m.mw(mxmwpos,mxmwagl);

		mxmw=g.mxmw;
		mxmw.mult(m);

//		glTranslatef(getx(),gety(),getz());
//		glRotatef(a.getx(),1,0,0);
//		glRotatef(a.gety(),0,1,0);
//		glRotatef(a.getz(),0,0,1);

		return true;
	}
	p3 posinwcs(const p3&p){
		refreshmxmw();
		p3 d;
		mxmw.mult(p,d);
		return d;
	}

	virtual bool oncol(glob&o){metrics::collisions++;return &o!=0;}
	void coldet(glob&o){
		const p3 wpthis=g.posinwcs(*this);
		const p3 wpo=o.g.posinwcs(o);
		const p3 v(wpthis,wpo);
		const float d=v.magn();
		metrics::coldetsph++;
//		flf();l()<<typeid(*this).name()<<"("<<wpthis<<")  "<<typeid(o).name()<<"("<<wpo<<")  "<<d<<"  "<<bv.r<<"   "<<o.bv.r<<endl;
		if(d>(bv.getradius()+o.bv.getradius())){
			if(o.iscoldetrec()){
				for(auto gg:o.chs)
					coldet(*gg);
				return;
			}
			return;
		}
		if(issolid()&&o.issolid()){
			oncol(o);
			o.oncol(*this);
			return;
		}
		if(issolid()&&!o.issolid()){
			for(auto gg:o.chs)
				coldet(*gg);
			return;
		}
		if(!issolid()&&o.issolid()){
			for(auto gg:chs)
				o.coldet(*gg);
			return;
		}
	}
	inline p3&agl(){return a;}
	void draw(){
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1,0,0);
		glRotatef(a.gety(),0,1,0);
		glRotatef(a.getz(),0,0,1);
		gldraw();
		if(drawboundingspheres){
//			const GLbyte i=(GLbyte)rnd(0,32);
//			glColor3b(i,0,0);
//			glDisable(GL_LIGHTING);
//			glutWireSphere(bv.r,drawboundingspheresdetail,drawboundingspheresdetail);
//			glEnable(GL_LIGHTING);

//			const GLbyte i=32+(GLbyte)rnd(0,32);
			const GLbyte i=127;
			glColor3b(i,i,i);
			int detail=(int)(1.f*bv.getradius()*drawboundingspheresdetail);
			if(detail<drawboundingspheresdetail)
				detail=drawboundingspheresdetail;
			glutSolidSphere(bv.getradius(),detail,detail);
		}
		for(auto g:chs){glPushMatrix();g->draw();glPopMatrix();}
	}
	virtual void gldraw()=0;
	virtual void tick(){
		chs.splice(chs.end(),chsadd);
		for(auto g:chs)g->tick();
		for(auto g:chsrm){chs.remove(g);delete g;}
		chsrm.clear();
	}
	inline glob&getglob()const{return g;}
	void rm(){if(rmed){flf();return;}rmed=true;g.chsrm.push_back(this);}
private:
	bool rmed;
};
bool glob::drawboundingspheres=true;
int glob::drawboundingspheresdetail=6;

class obcorpqb:public glob{
	float r;
	static int n;
	float a,drscl,dr;
public:
	obcorpqb(glob&pt,const float r=1):glob(pt),r(r),a(.25f*n++),drscl(.5){}
	void gldraw(){glutSolidSphere(r+dr,4,3);}
	virtual void tick(){
		const float s=.1f;
		const float dx=rnd(-s,s);
		const float dy=rnd(-s,s);
		const float dz=0;
		transl(dt(dx),dt(dy),dt(dz));
		dr=drscl*sin(a);
		bv.setradius(r+dr);
		glob::tick();
	}
};
int obcorpqb::n=0;


class obcorp:public glob{
	static const float s;
	GLfloat f,ff;
public:
	obcorp(glob&pt,const p3&p=p3(),const p3&a=p3()):glob(pt,p,a){
		bv.setradius(s+1.57f);
		bits=0;
		const float ds=.1f*s;
		const float dz=.5f*s;
		for(float zz=-s;zz<=s;zz+=dz)
			for(float xx=-s;xx<=s;xx+=ds)
				for(float yy=-s;yy<=s;yy+=ds){
					if(sqrt(xx*xx+yy*yy+zz*zz)>s)
						continue;
					glob&o=*new obcorpqb(*this);
					o.transl(xx,yy,zz);
					o.agl().transl(90,0,0);
				}
	}
	void gldraw(){
		glShadeModel(GL_FLAT);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glColor4f(.5,.5,.5,1);
//		GLfloat matspec[]={ff,ff,ff,1};
//		ff+=dt(1/10);if(ff>1)ff=0;
//		glMaterialfv(GL_FRONT,GL_SPECULAR,matspec);
//		GLfloat matshin[]={f};
//		f++;if(f>128)f=0;
//		glMaterialfv(GL_FRONT,GL_SHININESS,matshin);
//		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE) ;
//		glEnable(GL_COLOR_MATERIAL) ;
//		glColor3b(127,127,127);

	}
};
const float obcorp::s=7;


class obwom:public glob{
	int links;
public:
	obwom(glob&pt,const int links=4,const p3&p=p3()):glob(pt,p,p3(),.4f,p3()),links(links){
		if(links==0)
			return;
		bits|=(2+4);
		new obwom(*this,links-1,p3(0,.4f,.4f));
	}
	void gldraw(){
//		glPushAttrib(GL_ENABLE_BIT);
//		glShadeModel(GL_SMOOTH);
//		glShadeModel(GL_FLAT);
////		glEnable(GL_CULL_FACE);
////		glFrontFace(GL_CCW);
//		glEnable(GL_LIGHTING);
//		glEnable(GL_LIGHT0);
//		const float dr=rnd(0,.1f);
		const float dr=0;
		glColor3b(127,127,127);
		glutSolidSphere(bv.getradius()+dr,6,6);
//		glutSolidCube(r+dr);
//		glPopAttrib();
	}
	virtual void tick(){
		glob::tick();
		if(links>0)
			agl().transl(0,dt(60),0);
//		agl().transl(dt(60),0,dt(60));
//		a.transl(0,d(60),d(60));
//		a.transl(d(60),0,0);
//		a.transl(d(60),d(60),0);
	}
};


class obiglo:public glob{
	float s;
	p3 dp;
	p3 pprv;
public:
	obiglo(glob&g,const p3&p,const float size=.05f):glob(g,p,p3(90,0,0),size),s(size),dp(0,-1,0){}
	virtual void gldraw(){}
	virtual void tick(){
		pprv.set(*this);
		transl(dp,dt());
		if(gety()<bv.getradius())
			dp.set(0,0,0);
	}
	virtual bool oncol(glob&o){
		set(pprv);
		dp.set(0,0,0);
		return &o!=0;
	}
};

#include<fstream>

class f3{
	GLuint glpt;
	GLuint glix;
	int vtxbufsizebytes,vtxdim,ixbufsizebytes,ntri;
	p3 scl;
public:
	f3(const char*filepath="ufo.f3",const p3&scale=p3(1,1,1)):glpt(0),glix(0),vtxbufsizebytes(0),vtxdim(0),ixbufsizebytes(0),ntri(0),scl(scale){
		metrics::f3s++;
		ifstream is(filepath);
		if(!is.good())throw signl(-1,filepath);
		load(is);
		if(!is.good())throw signl(-1,filepath);
		is.close();
	}
	virtual~f3(){
		if(glpt)glDeleteBuffers(1,&glpt);
		if(glix)glDeleteBuffers(1,&glix);
		metrics::f3s--;
	}
	inline const p3&scale()const{return scl;}
	virtual void gldraw(){
		if(glpt){
			glBindBuffer(GL_ARRAY_BUFFER,glpt);
			glVertexAttribPointer(0,vtxdim,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(0);
		}
		if(!glix){
			glPointSize(3);
			glColor3b(0,0,0);
			glDrawArrays(GL_POINTS,0,vtxbufsizebytes/vtxdim);
			return;
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glix);
		gldrawelems();
	}
protected:
	virtual void gldrawelems(){
		glColor3b(0,0,0);
		glDrawElements(GL_TRIANGLES,ixbufsizebytes,GL_UNSIGNED_BYTE,0);
	}
private:

	f3&load(ifstream&is){
		is>>skipws;
		int n;
		is>>n;
		vtxdim=3;
//		unique_ptr<GLfloat>pt=new GLfloat[(unsigned long)(n*vtxdim)];
		GLfloat*pt=new GLfloat[(unsigned long)(n*vtxdim)];//? bug leak
		vtxbufsizebytes=n*vtxdim*(int)sizeof(GLfloat);
		GLfloat*pp=pt;
		const float sx=scl.getx();
		const float sy=scl.gety();
		const float sz=scl.getz();
		while(n--){
			is>>*pp;*pp++*=sx;
			is>>*pp;*pp++*=sy;
			is>>*pp;*pp++*=sz;
		}
		glGenBuffers(1,&glpt);
		glBindBuffer(GL_ARRAY_BUFFER,glpt);
		glBufferData(GL_ARRAY_BUFFER,vtxbufsizebytes,pt,GL_STATIC_DRAW);
		delete pt;

		is>>n;
		ntri=n;
		GLubyte*ix=new GLubyte[(unsigned int)ntri*3];//? leak
		ixbufsizebytes=(int)sizeof(GLubyte)*ntri*3;
		GLubyte*p1=ix;
		while(n--){
			int i0,i1,i2;
			int nv;is>>nv;
			if(nv!=3)throw signl(-3,"surfacenottriangle");
			is>>i0;*p1++=(GLubyte)i0;
			is>>i1;*p1++=(GLubyte)i1;
			is>>i2;*p1++=(GLubyte)i2;
			GLshort rgb[3];
			is>>rgb[0]>>rgb[1]>>rgb[2];
		}
//		cout<<filepath<<"(nvtx("<<vtxbufsizebytes/vtxdim/(int)sizeof(GLfloat)<<") nix("<<ntri<<"))"<<endl;

		glGenBuffers(1,&glix);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glix);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,ixbufsizebytes,ix,GL_STATIC_DRAW);
		delete ix;//? uqptr
		return*this;
	}
};

class globo:public glob{
	f3&f;
public:
	globo(glob&g,f3&f,const p3&pos=p3()):glob(g,pos,p3(),f.scale().magn(),f.scale()),f(f){metrics::globos++;}
	~globo(){metrics::globos--;}
	void gldraw(){f.gldraw();}
};

static f3*fufo=0;
class obufocluster:glob{
public:
	obufocluster(glob&g,const p3&p=p3()):glob(g,p){
		const float s=20;
		const float ds=s/9;
		for(float zz=-s;zz<=s;zz+=ds)
			for(float yy=-s;yy<=s;yy+=ds)
				for(float xx=-s;xx<=s;xx+=ds)
					if(sqrtf(xx*xx+yy*yy+zz*zz)>s)
						continue;
					else
						new globo(*this,*fufo,p3(p.getx()+xx,p.gety()+yy,p.getz()+zz));
	}
};

class obtex:public glob{
	GLuint gltx;
protected:
	int wihi;
	GLubyte*rgba;
	float s;
public:
	obtex(glob&g,const int wihi=4*32,const float s=1,const p3&p=p3(),const p3&a=p3(),const float r=1,const p3&v=p3()):glob(g,p,a,r,v),gltx(0),wihi(wihi),s(s){
		rgba=new GLubyte[wihi*wihi*4];
		zap();
		glGenTextures(1,&gltx);
		glBindTexture(GL_TEXTURE_2D,gltx);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
		glBindTexture(GL_TEXTURE_2D,gltx);
		if(glGetError())throw signl(-3,"texture");
		updtx();
	}
	~obtex(){delete rgba;}
	void updtx(){
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,wihi,wihi,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba);
	}
	void zap(){
		int n=wihi*wihi;
		GLubyte*pp=rgba;
		while(n--){
			GLubyte b=(GLubyte)rnd(0,50);
			if(b<49)
				b=0;
			*pp++=b;
			*pp++=b;
			*pp++=b;
			*pp++=255;
		}
	}
	void gldraw(){
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,gltx);
		glBegin(GL_QUADS);
		glColor4b(0,0,0,0);
		glTexCoord2f(0,0);
		glVertex3f(-s,-s,0);
		glTexCoord2f(1,0);
		glVertex3f(s,-s,0);
		glTexCoord2f(1,-1);
		glVertex3f(s,s,0);
		glTexCoord2f(0,-1);
		glVertex3f(-s,s,0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
};

class obcon:public obtex{
	static unsigned short fnt_az[];
	static unsigned short fnt_09[];
	const static int fnt_w=4;
	const static int fnt_h=4;
	const static int bp=4;
	GLubyte*p;
	GLubyte*pnl;
	const char*title;
public:
	obcon(glob&g,const p3&p=p3(),const p3&a=p3(),const char*title="gnox ministry of consoles"):obtex(g,32*bp,1,p,a),p(rgba+wihi*bp+bp),title(title){}
	inline obcon&phom(){p=rgba+wihi*bp+bp;return*this;}
	inline obcon&prnt(const char*s){return prnt(strlen(s),s);}
	obcon&prnt(const size_t len,const char*s){
//		const unsigned short fnt_az[]={0x0552,0x0771,0x0212,0x0774,0x0737,0x0137,0x0651,0x0571,0x0220,0x0122,0x0531,0x0610,0x0770,0x0530,0x0252,0x1770,0x4770,0x0160,0x0324,0x0270,0x0650,0x0250,0x0775,0x0525,0x0225,0x0630};
//		const unsigned short fnt_09[]={0x0252,0x0220,0x0621,0x0642,0x0451,0x0324,0x0612,0x0247,0x2702,0x2452};

		pnl=p;
		for(size_t i=0;i<len;i++){
			const char ch=s[i];
			if(ch==0)
				break;
			unsigned short sch;
			if(ch>='a'&&ch<='z')
				sch=fnt_az[ch-'a'];
			else if(ch>='0'&&ch<='9'){
				sch=fnt_09[ch-'0'];
			}else if(ch=='\n'){
				p=pnl+fnt_h*wihi*bp;
				pnl=p;
				continue;
			}else if(ch==127){
				p-=fnt_w*bp;
				continue;
			}else if(iswspace(ch)){
				sch=0x0020;
			}
			int h=fnt_h;
			while(h--){
				for(int w=fnt_w;w>0;w--){
					if(sch&1){
						*p++=255;
						*p++=255;
						*p++=255;
						*p++=255;
					}else{
						p+=4;
//						*p++=0;
//						*p++=0;
//						*p++=0;
//						*p++=0;
					}
					sch>>=1;
				}
				p=p+wihi*bp-fnt_w*bp;
			}
			p=p-wihi*bp*fnt_h+fnt_w*bp;
		}
		pnl=p=pnl+fnt_h*wihi*bp;
		return *this;
	}
	inline obcon&nl(){p=pnl+=wihi*bp;return*this;}
	virtual void tick(){
		obtex::tick();
		int n=wihi*wihi*bp/4;
		GLubyte*pp=rgba;
		while(n--){
			GLubyte b=(GLubyte)rnd(0,255);
			if(b==0){
				b=*pp>0?0:100;
				*pp++=b;
				*pp++=b;
				*pp++=b;
				*pp++=255;
			}else
				pp+=4;
		}

		pp=rgba;
		n=wihi*wihi*bp;
		n>>=4;
		while(n--)*pp++=0;
		phom().prnt(title).nl().nl().nl().prnt(sts.str().c_str()).prnt(inp.str().c_str()).nl();
		updtx();
	}
};
unsigned short obcon::fnt_az[]={0x0552,0x0771,0x0212,0x0774,0x0737,0x0137,0x0651,0x0571,0x0220,0x0122,0x0531,0x0610,0x0770,0x0530,0x0252,0x1770,0x4770,0x0160,0x0324,0x0270,0x0650,0x0250,0x0775,0x0525,0x0225,0x0630};
unsigned short obcon::fnt_09[]={0x0252,0x0220,0x0621,0x0642,0x0451,0x0324,0x0612,0x0247,0x2702,0x2452};

#include<typeinfo>
class grid{
	grid*subgrids[4];
	list<glob*>globs;
	p3 ptl;
	float s;
	static const size_t nspl;
public:
	grid(const p3&topleft,const float size):subgrids({0,0,0,0}),ptl(topleft),s(size){
//		flf();l()<<"grid(p("<<topleft<<")size("<<size<<")"<<endl;
		metrics::ngrids++;
	}
	~grid(){metrics::ngrids--;clear();}
	void gldraw(){
		glColor3b(0,0,0x40);
		const float yoff=0;//.1f;
		glBegin(GL_LINE_STRIP);
		glVertex3f(ptl.getx()-s,yoff,ptl.getz()-s);
		glVertex3f(ptl.getx()+s,yoff,ptl.getz()-s);
		glVertex3f(ptl.getx()+s,yoff,ptl.getz()+s);
		glVertex3f(ptl.getx()-s,yoff,ptl.getz()+s);
		glVertex3f(ptl.getx()-s,yoff,ptl.getz()-s);
		glEnd();
		for(auto gr:subgrids){
			if(!gr)
				continue;
			gr->gldraw();
		}
	}
	void addall(list<glob*>&ls){
		for(auto g:ls)
			putif(g,*g,g->getbvol().getradius());
		splitif(6);
	}
	void coldet(){
		if(!globs.empty()){
			auto i1=globs.begin();
			while(true){
				auto i2=globs.rbegin();
				if(*i1==*i2)
					break;
				glob&g1=*(*i1);
				do{
					glob&g2=*(*i2);
					g1.coldet(g2);
					i2++;
				}while(*i1!=*i2);
				i1++;
			}
		}
		for(auto g:subgrids)
			if(g)
				g->coldet();
	}
	void clear(){
		globs.clear();
		for(auto&g:subgrids)
			if(g){
				g->clear();
				delete g;
				g=0;
			}
	}
private:
	bool putif(glob*g,const p3&p,const float r){
//		flf();l()<<typeid(*g).name()<<"(p("<<p<<"))r("<<r<<")  grid(p("<<ptl<<")s("<<s<<")"<<endl;
		if((p.getx()+s+r)<ptl.getx())return false;
		if((p.getx()-s-r)>ptl.getx())return false;
		if((p.getz()+s+r)<ptl.getz())return false;
		if((p.getz()-s-r)>ptl.getz())return false;
//		if((p.getz()+r)<ptl.getz())return;
//		if((p.getz()-r)>(ptl.getz()+s))return;
//		cout<<" added"<<endl;
		globs.push_back(g);
//		flf();l()<<"grid(p("<<ptl<<")s("<<s<<"))add "<<typeid(*g).name()<<"(p("<<p<<")r("<<r<<"))"<<endl;
		return true;
	}
	bool splitif(const int nrec){
		if(globs.size()<nspl)
			return false;
		const float ns=s/2;
//		flf();l()<<"split  "<<globs.size()<<"   size("<<ns<<")"<<endl;
		subgrids[0]=new grid(p3(ptl).transl(-ns,0,-ns),ns);
		subgrids[1]=new grid(p3(ptl).transl( ns,0,-ns),ns);
		subgrids[2]=new grid(p3(ptl).transl(-ns,0, ns),ns);
		subgrids[3]=new grid(p3(ptl).transl( ns,0, ns),ns);
		bool done=false;
		for(auto gr:subgrids){
			int i=0;
			int nglobs=0;
			for(auto g:globs){
				nglobs++;
				if(gr->putif(g,*g,g->getbvol().getradius()))
					i++;
			}
			if(nrec==0)
				break;
//			if(i==nglobs){
//				done=true;
//				break;
//			}
			gr->splitif(nrec-1);
		}
		if(done){
//			flf();l()<<" allsubgridscontainallobjects, recursiondone  at "<<s<<"   contains "<<globs.size()<<" globs"<<endl;
			for(auto&g:subgrids){
				delete g;
				g=0;
			}
			return true;
		}
		globs.clear();
		return true;
	}
};
const size_t grid::nspl=20;

class wold:public glob{
	static wold wd;
	float t;
	grid grd;
	wold():glob(*(glob*)0,p3(),p3(),15),t(0),grd(p3(),15),drawgrid(true),hidezplane(false),coldet(false),coldetgrid(true){}
	~wold(){if(fufo)delete fufo;}
public:
	bool drawaxis,drawgrid,hidezplane,coldet,coldetgrid;
	inline static wold&get(){return wd;}
	inline float gett(){return t;}
	void load(){
//		new obcorp(*this,p3(0,9,4.2f),p3(90,0,0));
//		new obball(*this,p3(0,1,0));
//		new obball(*this,p3(.1f,1,0));

//		fufo=new f3("ufo.f3",p3(1.5,.25,1));//? leak
//		fufo=new f3("ufo.f3",p3(1,1,1));//? leak
//		new obufocluster(*this,p3(50,0,0));
//		hidezplane=true;
//		for(float xx=-bv.r;xx<=bv.r;xx+=s*4)
//		for(float zz=-bv.r;zz<=bv.r;zz+=s*4)
		const float s=1;
		const float spread=-bv.getradius()/2;
		for(int n=0;n<50;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2);
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<50;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<50;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+1.5f*s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<50;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+2*s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<50;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+3*s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		new obcon(*this,p3(bv.getradius(),0,bv.getradius()));

//		new obcorp(*this,p3(0,4.2f,0));
//		new obcorp(*this,p3(0,4.2f,8));
//		new obwom(*this,1,p3(10,.8f,-5));
//		new obwom(*this,3,p3(10,0,-5));

//		for(float xx=-bv.r;xx<=bv.r;xx+=5)
//			for(float zz=-bv.r;zz<=bv.r;zz+=5)
//				new obwom(*this,5,p3(xx,0,zz));
	}
	void gldraw(){
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		const float s=bv.getradius();
		if(drawgrid){
			grd.gldraw();
//			glColor3b(0,0,0x7f);
//	//		//glutWireCube(s);
//			glBegin(GL_LINE_STRIP);
//			glVertex2f(-s,-s);
//			glVertex2f( s,-s);
//			glVertex2f( s, s);
//			glVertex2f(-s, s);
//			glVertex2f(-s,-s);
//			glEnd();
		}
		if(drawaxis){
			//glPolygonOffset
			glBegin(GL_LINE_STRIP);

			glColor3b(127,0,0);
			glVertex3f(0,0,0);

			glColor3b(127,0,0);
			glVertex3f(s,0,0);
			glVertex3f(0,0,0);

			glColor3b(0,0,0);
			glVertex3f(0,0,0);

			glColor3b(0,0,0);
			glVertex3f(0,s,0);

			glColor3b(0,0,127);
			glVertex3f(0,0,0);

			glColor3b(0,0,127);
			glVertex3f(0,0,s);
			glEnd();
		}
		if(!hidezplane){
			const float r=s;
			glPushMatrix();
	//		glTranslatef(0,0,01f);
			const float a=float(sin(.1*t));
			glColor3f(a,a,a);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0,0);
			glVertex2f(r,0);
			const float dtr=3.14159f/180;
			const int di=360/24/2/2;
			for(int i=di;i<=360;i+=di){
				const float rd=i*dtr;
				glVertex3f(r*cos(rd),0,r*sin(rd));
			}
			glEnd();
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		static float lha;
		static float dlha=60*3.1415f/180;
		lha+=dt(dlha);
		static float lht=10000;
		static float dlht=0;
		const float lhx=lht*cos(lha);
		const float lhy=lht*sin(lha);
		const GLfloat lhtpos[]={lhx,0,lhy,1};
		lht+=dt(dlht);
		glLightfv(GL_LIGHT0,GL_POSITION,lhtpos);
		const GLfloat lhtcol[]={0,0,0,1};
		glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,lhtcol);

		glEnable(GL_CULL_FACE);
	}
	void tick(){
		clk::timerrestart();
		t+=dt();
		if(coldetgrid){
			grd.clear();
			grd.addall(chs);
			grd.coldet();
		}
		metrics::dtgrd=clk::timerdt();
		clk::timerrestart();
		if(coldet){
			auto i1=chs.begin();
			while(true){
				auto i2=chs.rbegin();
				if(*i1==*i2)
					break;
				glob&g1=*(*i1);
				do{
					glob&g2=*(*i2);
					g1.coldet(g2);
					i2++;
				}while(*i1!=*i2);
				i1++;
			}
		}
		metrics::dtcoldetbrute=clk::timerdt();
		clk::timerrestart();
		glob::tick();
		metrics::dtupd=clk::timerdt();
	}
};
wold wold::wd;

template<typename T>class lut{
private:
	size_t size;
	static int hash(const char*key,const size_t roll){
		int i=0;
		const char*p=key;
		while(*p)i+=*p++;
		i%=roll;
		return i;
	}
	class el{
	public:
		const char*key;
		T data;
		el*nxt;
		el(const char*key,T data):key(key),data(data),nxt(NULL){}
		~el(){if(nxt)delete nxt;}
	};
	el**array;
public:
	lut(const size_t size=8):size(size){array=(el**)calloc(size,sizeof(el*));}
	~lut(){clear();delete array;}
	T operator[](const char*key)const{
		const int h=hash(key,size);
		el*l=array[h];
		if(!l)
			return (T)NULL;
		while(1){
			if(!strcmp(l->key,(char*)key))return l->data;
			if(l->nxt){l=l->nxt;continue;}
			return (T)NULL;
		}
		return (T)NULL;//?
	}
	void rm(const char*key){
		const int h=hash(key,size);
		el*l=array[h];
		if(!l)
			return;
		el*lp=NULL;
		while(1){
			if(!strcmp(l->key,key)){
				if(lp)
					lp->nxt=l->nxt;
				else
					array[h]=NULL;
				l->nxt=NULL;
				delete l;
				return;
			}
			lp=l;
			if(l->nxt){
				l=l->nxt;
				continue;
			}
			return;
		}
	}
	void put(const char*key,T data){
		const int h=hash(key,size);
		el*l=array[h];
		if(!l){
			array[h]=new el(key,data);
			return;
		}
		while(1){
			if(!strcmp(l->key,key)){
				l->data=data;
				return;
			}
			if(l->nxt){
				l=l->nxt;
				continue;
			}
			l->nxt=new el(key,data);
			return;
		}
	}
	void clear(){
		for(size_t i=0;i<size;i++){
			el*e=array[i];
			if(!e)
				continue;
			delete e;
			array[i]=NULL;
		}
	}
};


class obball:public glob{
	float lft;
	p3 prv;
	p3 dp;
	float colr=1;
public:
	obball(glob&g,const p3&p,const float r=.05f):glob(g,p,p3(90,0,0),r),lft(0),dp(p3()){
		bits|=8;
	}
	inline p3&getdp(){return dp;}
	void gldraw(){}
	virtual void tick(){
		lft+=dt(1);
		if(lft>10){
			rm();
			return;
		}
		colr=1;
		prv.set(*this);
		dp.transl(dt(),dt(-9.f),dt());
		agl().transl(0,0,dt(1));
		transl(dp,dt());
		if(gety()<bv.getradius()){
			dp.scale(0,-.5f,0);
			transl(0,bv.getradius()-gety(),0);
		}
		glob::tick();
	}
	virtual bool oncol(glob&o){
//		flf();l()<<typeid(o).name()<<endl;
		if(!o.issolid())return true;
		set(prv);
		dp.neg().scale(.5f);
		colr=0;
		return true;
	}
};

#include<sys/time.h>
#include<iomanip>

class windo:public glob{
	char ccounter;
	p3 pprv;
	bool gravity;
	float zoom;
	p3 lookat;
	float flappery;
	float rocketry;
	bool dodrawhud;
	bool gamemode=true,fullscr=true;
	bool consolemode=false;
	int item;
	p3 d;
	p3 dd;
	p3 f;
	p3 fi;
	lut<int>keysdn;
	int wi,hi;
	int __w,__h;
	m3 mmv;
public:
	windo(glob&g=wold::get(),const int width=1024,const int height=512,const p3&p=p3(wold::get().getbvol().getradius(),.2f,0),const p3&a=p3(),bool gravity=true,const float zoom=1,const float s=.1f,const p3&bx=p3())
		:glob(g,p,a,s,bx),gravity(gravity),zoom(zoom),dodrawhud(true),wi(width),hi(height),__w(width),__h(height){}
	inline bool isgamemode()const{return gamemode;}
	inline bool isfullscreen()const{return fullscr;}
	inline bool getwidth()const{return wi;}
	inline bool getheight()const{return hi;}
	windo&togglehud(){dodrawhud=!dodrawhud;return*this;}
	void drawframe(){
		cout<<"\rframe("<<metrics::frames++<<")";
		clk::timerrestart();
		glClearColor(.3f,.3f,1,1);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1);
		glShadeModel(GL_SMOOTH);

		float ff=0,f=0;
		GLfloat matspec[]={ff,ff,ff,1};
		ff+=dt(1/10);if(ff>1)ff=0;
		glMaterialfv(GL_FRONT,GL_SPECULAR,matspec);
		GLfloat matshin[]={f};
		f++;if(f>128)f=0;
		glMaterialfv(GL_FRONT,GL_SHININESS,matshin);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE) ;
		glEnable(GL_COLOR_MATERIAL) ;
		glColor3b(127,127,127);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,wi,hi);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45*zoom,(GLdouble)wi/hi,.01,1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(agl().getx(),1,0,0);
		glRotatef(agl().gety(),0,1,0);
		glRotatef(agl().getz(),0,0,1);
		glTranslatef(-getx(),-gety(),-getz());
		GLfloat af[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,af);
		mmv.set(af);

		getglob().draw();
		metrics::dtrend=clk::timerdt();

		if(dodrawhud){
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0,wi,0,hi,0,1);
			glColor3b(0x00,0x00,0x40);
			drawhud();
		}
		cout<<flush;
	}
	virtual bool oncol(glob&g){
		sts<<typeid(g).name()<<"["<<g.getid()<<"]"<<endl;
		if(g.isfood()){
			g.rm();
			item++;
			return true;
		}
		set(pprv);
		d.neg().scale(.2f);
		return true;
		if(g.isblt()){
			set(0,40,0);
			agl().set(45,0,0);
		}
		return true;
	}
	virtual void tick(){
		glob::tick();
		flappery+=dt(3);
		if(flappery>1)flappery=1;
		rocketry+=dt();
		if(rocketry>3)rocketry=3;
		if(gravity){
			dd.set(0,-9.8f,0);
			dd.transl(f);
		}
		dd.transl(fi);
		fi.set(0,0,0);
		d.transl(dd,dt());
		pprv.set(*this);
		transl(d,dt());
		if(gety()<bv.getradius()){
//			flf();l()<<gety()<<" < "<<bv.r<<endl;
			d.neg().scale(.2f);
//			dd.set(0,0,0);
			set(pprv);
		}
//		flf();l()<<dd<<endl;
//		flf();l()<<endl;
	}
	virtual void gldraw(){}
	void fire(){
		p3 lv=mmv.zaxis();
		const float sprd=.05f;
		const float velocity=30;
		const float extravelup=2;
		const float scl=.01f;
		p3 vel=p3(lv).transl(rnd(-sprd,sprd),rnd(-sprd,sprd),rnd(-sprd,sprd)).neg().scale(velocity).transl(0,extravelup,0);
		obball&o=*new obball(wold::get(),lv.scale(bv.getradius()/2).transl(*this),scl);
		o.getdp().set(vel);
	}
	void timer(){
		clk::tk++;
		sts.str("");
		static float a=0;
		static float dr=2;
		static float fromheight=wold::get().getbvol().getradius()*2;
		static float d=3;
		a+=dt(360);
		if(!consolemode){
	//		if(iskeydn('r')&&iskeydn('u')){wn.transl(0,dt(1),0);}
	//		if(iskeydn('v')&&iskeydn('n')){wn.transl(0,-dt(1),0);}
			if(iskeydn('b')){
				const float r=wold::get().getbvol().getradius()/2;
				const float dx=rnd(-r,r);
				const float dz=rnd(-r,r);
				for(int i=0;i<11;i++)
					new obball(wold::get(),p3(dx+dr*cos(a)*rnd(-dr,dr),fromheight,dz+dr*sin(a)*rnd(-dr,dr)));
			}
	//		if(iskeydn('i')){wold::get().transl(0,0,dt(10));}
	//		if(iskeydn('k')){wold::get().transl(0,0,-dt(10));}
			if(iskeydn('w')){transl(mmv.zaxis(),dt(-d));}
			if(iskeydn('s')){transl(mmv.zaxis(),dt(d));}
			if(iskeydn('d')){transl(mmv.xaxis(),dt(d));}
			if(iskeydn('a')){transl(mmv.xaxis(),dt(-d));}
			if(iskeydn('t')){transl(mmv.yaxis(),dt(d));}
			if(iskeydn('g')){transl(mmv.yaxis(),dt(-d));}
			if(iskeydn('x')&&rocketry>0){f.set(0,20,0);rocketry-=dt(6);}else{f.set(0,0,0);}
//			if(iskeydn('k')){wn.d.transl(0,dt(-1),0);}

			if(iskeydn('l')){
				agl().transl(0,dt(180),0);
//				m3 m(wn.mmv);
//				m.roty(-dt(3.1415f));
//				wn.lookat.set(m.zaxis().neg()).scale(100);
			}
			if(iskeydn('j')){
				agl().transl(0,-dt(180),0);
//				m3 m(wn.mmv);
//				m.roty(dt(3.1415f));
//				wn.lookat.set(m.zaxis().neg()).scale(100);
			}
			if(iskeydn(' ')){fire();}
		}
		metrics::coldetsph=metrics::collisions=metrics::mwrefresh=metrics::mpmul=metrics::mmmul=0;
		wold::get().tick();
	}
	bool iskeydn(const unsigned char key,const bool setifnot=0){
		static char k[]={0,0};
		k[0]=(char)key;
		const bool b=keysdn[k]==1;
		if(!b&&setifnot)
			keysdn.put(strcpy(new char[2],k),1);//? bug leak
		return b;
	}
	void keydn(const unsigned char key,const int x,const int y){
		if(iskeydn(key,true))return;
		if(consolemode){
			if(key==13){
				consolemode=false;
				return;
			}
			inp<<key;
			return;
		}
		sts<<"keydn("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
//		nl=true;
		wold&wd=wold::get();
		if(key==0){throw "keyo";}
//		else if(key=='j'){wd.ddegz-=360/60;}
//		else if(key=='f'){wd.ddegz+=360/60;}
//		else if(key=='t'){wd.ddegx-=360/60;}
//		else if(key=='g'){wd.ddegx+=360/60;}
//		else if(key=='y'){wd.ddegz=wd.ddegx=0;wd.agl().set(p3(270,0,0));}
//		else if(key=='h'){wd.ddegz=wd.ddegx=0;wd.agl().set(p3(90.5,0,0));}
		else if(key=='y'){zoom-=.1;}
		else if(key=='h'){zoom+=.1;}
		else if(key=='1'){glob::drawboundingspheres=!glob::drawboundingspheres;}
		else if(key=='2'){wd.drawaxis=!wd.drawaxis;}
		else if(key=='3'){wd.drawgrid=!wd.drawgrid;}
		else if(key=='4'){wd.hidezplane=!wd.hidezplane;}
		else if(key=='5'){wd.coldet=!wd.coldet;}
		else if(key=='6'){wd.coldetgrid=!wd.coldetgrid;}
		else if(key=='9'){set(p3(0,0,3));}
		else if(key==13){inp<<endl;consolemode=!consolemode;}
		else if(key==127){sts.str("");}// bkspc
//		else if(key=='x'){if(wn.rocketry>0){wn.f.set(0,11,0);};wn.rocketry-=dt(30);}
		else if(key=='q'){if(flappery>0){fi.set(0,300,0);flappery-=1;}}
		else if(key=='e'){if(flappery>0){fi.set(0,600,0);flappery-=1;}}
		else if(key=='c'){agl().transl(15,0,0);}
		else if(key=='f'){agl().transl(-15,0,0);}
		else if(key=='r'){agl().set(0,agl().gety(),agl().getz());}
		else if(key==9){togglehud();}
		else if(key==' '){fire();}
		else if(key=='0'){togglefullscr();return;}
	}
	void keyup(const unsigned char key,const int x,const int y){
		const char k[]={(char)key,0};
		keysdn.rm(k);
		sts<<"keyup("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
		if(key==27)// esc
			{if(fullscr)togglefullscr();cout<<endl;exit(0);}
	}
	void mouseclk(const int button,const int state,int x,const int y){
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glGetIntegerv(GL_VIEWPORT,viewport);

		winX=(float)x;
		winY=(float)viewport[3]-(float)y;
		glReadPixels(x,int(winY),1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
		gluUnProject(winX,winY,winZ,modelview,projection,viewport,&posX,&posY,&posZ);
		sts<<"mousclk("<<state<<","<<button<<",["<<x<<","<<y<<",0])";
		cout<<"unproj("<<posX<<" "<<posY<<" "<<posZ<<")";
	}
	void mousemov(const int x,const int y){sts<<"mousmov("<<x<<","<<y<<")";}
	void reshape(const int width,const int height){
		sts<<"reshape("<<wi<<"x"<<hi<<")";
		wi=width;hi=height;
	}
	void togglefullscr(){
		fullscr=!fullscr;
		if(fullscr){
			__w=wi;__h=hi;
			glutFullScreen();
			glutSetCursor(GLUT_CURSOR_NONE);
		}else{
			glutReshapeWindow(__w,__h);
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
	}
private:
	void pl(const char*text,const GLfloat y=0,const GLfloat x=0,const GLfloat linewidth=1,const float scale=1){
		const char*cp=text;
		glPushMatrix();
		glTranslatef(x,y,0);
		glScalef(scale,scale,0);
		glLineWidth(linewidth);
		for(;*cp;cp++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN,*cp);
//			glutStrokeString(GLUT_STROKE_MONO_ROMAN,text);
		glPopMatrix();
	}
	void drawhud(){
		const int dy=hi>>5;
		int y=-dy;

//		pl("glox",y,0,1,.2f);

		timeval tv;gettimeofday(&tv,0);
		const tm&t=*localtime(&tv.tv_sec);
		ostringstream oss;
		oss<<setprecision(2)<<fixed;
		oss<<t.tm_hour<<":"<<":"<<t.tm_min<<":"<<t.tm_sec<<"."<<tv.tv_usec/1000<<"    t("<<wold::get().gett()<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(1);
		oss<<"p("<<*this<<") a("<<agl()<<") zoom("<<zoom<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(2);
		oss<<"frame("<<metrics::frames<<") globs("<<metrics::globs-metrics::globos<<") f3s("<<metrics::f3s<<") vbos("<<metrics::globos<<") p3s("<<metrics::p3s<<") m3s("<<metrics::m3s<<") bvols("<<metrics::bvols<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(3);
		oss<<"upd("<<metrics::dtupd<<")s   draw("<<metrics::dtrend<<")s    "<<((int)(metrics::globs/(metrics::dtrend?metrics::dtrend:1))>>10)<<"Kglobs/s    rendfpsest("<<(1/metrics::dtrend)<<")f/s";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(4);
		oss<<"coldet("<<(wold::get().coldetgrid?"grid":"")<<" "<<(wold::get().coldet?"brute":"")<<") ngrids("<<metrics::ngrids<<") grid("<<metrics::dtgrd<<")s  "<<(((long long int)(metrics::globs/metrics::dtgrd))>>10)<<"Kglobs/s   brutedt("<<metrics::dtcoldetbrute<<")s";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<"sphcolsdet("<<metrics::coldetsph<<") sphcols("<<metrics::collisions<<")"<<" mxrfsh("<<metrics::mwrefresh<<")"<<" mvmul("<<metrics::mpmul<<") mmmul("<<metrics::mmmul<<") ";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(1);
		oss<<"flappery("<<flappery<<") "<<"rocketry("<<rocketry<<") "<<"snowballs("<<item<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		y+=dy;pl(sts.str().c_str(),y,0,1,.1f);
		sts.str("");
	}
};

extern void gnox();

namespace glut{
	windo&wn=*new windo();
	void reshape(const int width,const int height){wn.reshape(width,height);}
	void draw(){wn.drawframe();glutSwapBuffers();}
	void timer(const int value){wn.timer();glutPostRedisplay();glutTimerFunc((unsigned)value,timer,value);}
//	void idle(){return;}
	void keydn(const unsigned char key,const int x,const int y){wn.keydn(key,x,y);}
	void keyup(const unsigned char key,const int x,const int y){wn.keyup(key,x,y);}
	void mouseclk(const int button,const int state,int x,const int y){wn.mouseclk(button,state,x,y);}
	void mousemov(const int x,const int y){wn.mousemov(x,y);}
	static void mainsig(const int i){cerr<<" ••• terminated with signal "<<i<<endl;exit(i);}
	int main(int argc,char**argv){
		cout<<"glox"<<endl;
		for(int i=0;i<32;i++)signal(i,mainsig);//?
		srand(0);
		gnox();
		glutInit(&argc,argv);
		glutIgnoreKeyRepeat(true);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		if(wn.isgamemode()){
			glutGameModeString("1366x768:32");
			glutEnterGameMode();
			glutSetCursor(GLUT_CURSOR_NONE);
		}else{
			glutInitWindowSize(wn.getwidth(),wn.getheight());
			glutCreateWindow("glox");
			if(wn.isfullscreen()){
				glutFullScreen();
				glutSetCursor(GLUT_CURSOR_NONE);
			}
		}

		wold::get().load();

		glutDisplayFunc(draw);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keydn);
		glutKeyboardUpFunc(keyup);
		glutMouseFunc(mouseclk);
		glutMotionFunc(mousemov);
		glutTimerFunc(0,timer,clk::dtms);
//		glutIdleFunc(idle);
//		glutReportErrors();
		const bool sizeofs=false;
		if(sizeofs){
			cout<<endl;
			cout<<"           type :   size : "<<endl;
			cout<<"----------------:--------: "<<endl;
			cout<<setw(16)<<"p3"<<setw(8)<<sizeof(p3)<<endl;
			cout<<setw(16)<<"m3"<<setw(8)<<sizeof(m3)<<endl;
			cout<<setw(16)<<"glob"<<setw(8)<<sizeof(glob)<<endl;
			cout<<setw(16)<<"bvol"<<setw(8)<<sizeof(bvol)<<endl;
			cout<<endl;
		}
		glutMainLoop();
		return 0;
	}
}

int main(){return glut::main(0,NULL);}

extern void gnox(){}
#endif
