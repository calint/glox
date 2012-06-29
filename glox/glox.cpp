#ifndef __glox__
#define __glox__

#include<iostream>
using namespace std;

#include<sstream>

namespace glox{
	namespace clk{
		int fps=100;
		int dtms=1000/fps;
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
//		int bvols;
		int collisions;
		int globos;
		int f3s;
		int mwrefresh;
		int mpmul;
		int mmmul;
		int ngrids;
		float dtcoldetgrd;
		float dtcoldetbrute;
		float dtnet;
		int cullview;
		int globsrend;
	}
	inline float dt(const float f=1){return f*clk::dt;}
	inline float rnd(const float from,const float tonotincluding){return from+(tonotincluding-from)*rand()/RAND_MAX;}
	inline float rndo(const float tonotincluding){return tonotincluding*rand()/RAND_MAX;}
	inline float rndn(const float s){return rnd(-s,s);}
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
	inline p3(const p3&from,const p3&to,const float len):x(to.x-from.x),y(to.y-from.y),z(to.z-from.z){metrics::p3s++;norm(len);}
	inline ~p3(){metrics::p3s--;}

	inline float getx()const{return x;}
	inline p3&setx(const float f){x=f;return*this;}
	inline float gety()const{return y;}
	inline p3&sety(const float f){y=f;return*this;}
	inline float getz()const{return z;}
	inline p3&transl(const float dx,const float dy,const float dz){x+=dx;y+=dy;z+=dz;return*this;}
	inline p3&transl(const p3&d){x+=d.x;y+=d.y;z+=d.z;return*this;}
	inline p3&transl(const p3&d,const float dt){x+=d.x*dt;y+=d.y*dt;z+=d.z*dt;return*this;}
	inline float magn()const{return sqrt(x*x+y*y+z*z);}
	inline float magn2()const{return x*x+y*y+z*z;}
	inline p3&set(const p3&p){x=p.x;y=p.y;z=p.z;return*this;}
	inline p3&set(const float x,const float y,const float z){this->x=x;this->y=y;this->z=z;return*this;}
	inline p3&neg(){x=-x;y=-y;z=-z;return*this;}
	inline p3&negy(){y=-y;return*this;}
	inline p3&scale(const float s){x*=s;y*=s;z*=s;return*this;}
	inline p3&scale(const float sx,const float sy,const float sz){x*=sx;y*=sy;z*=sz;return*this;}
	inline bool operator==(const p3&p)const{return x==p.x&&y==p.y&&z==p.z;}
	p3&norm(const float length=1){
		float q=sqrt(x*x+y*y+z*z);
		if(q==0){
			x=y=z=0;
			return*this;
		}
		const float t=length/q;
		x=t*x;y=t*y;z=t*z;
		return*this;
	}
	inline float dot(const p3&p)const{return x*p.x+y*p.y+z*p.z;}
	p3&vecprod(const p3&v1,const p3&v2){
		x=v1.y*v2.z-v1.z*v2.y;
		y=v1.z*v2.x-v1.x*v2.z;
		z=v1.x*v2.y-v1.y*v2.x;
		return*this;
	}
	inline p3&pow2(){x*=x;y*=y;z*=z;return*this;}
	inline float sum()const{return x+y+z;}
	inline p3&mult(const p3&p){x*=p.x;y*=p.y;z*=p.z;return*this;}
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
	signl(const int i=0,const char*s="signal"):i(i),s(s){
		cerr<<" ••• signl "<<i<<" · "<<s<<endl;
		const int nva=10;
		void*va[nva];
		int n=backtrace(va,nva);
		backtrace_symbols_fd(va,n,1);
	}
	inline int num()const{return i;}
	inline const char* str()const{return s;}
};

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#include <glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define flf()l("···",__FILE__,__LINE__,__FUNCTION__);
static inline ostream&l(const char*s="",const char*file="",int lineno=0,const char*func=""){cerr<<file;if(lineno){cerr<<":"<<lineno;}cerr<<" "<<func<<"  "<<s;return cerr;}

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
//	m3 inv()const{
//		GLfloat m[16];
//		m[0 ]=xx;m[ 1]=yx;m[ 2]=zx;m[3]=ox;
//		m[4 ]=xy;m[ 5]=yy;m[ 6]=zy;m[7]=oy;
//		m[8 ]=xz;m[ 9]=yz;m[10]=zz;m[11]=oz;
//		m[12]=xo;m[13]=yo;m[14]=zo;m[15]=oo;
//
//		GLfloat mout[16];
//		gluInvertMatrix(m,mout);
//
//		m3 mx;
//		mx.set(mout);
//		return mx;
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
//
//class glob;
//class coll{
//public:
//	const glob&obja;
//	const p3&vtxincol;
//	const glob&objb;
//	const p3&planevtx;
//	const p3&planenml;
//	const float dotpd;
//	const float t;
//};
//class bvol{
//	float r;
//	p3 v;
//public:
//	bvol(const float sphereradius,const p3&boxcorner):r(sphereradius),v(boxcorner){metrics::bvols++;}
//	~bvol(){metrics::bvols--;}
//	inline float radius()const{return r;}
//	bvol&radius(const float r){this->r=r;return*this;}
////	bool spheresoverlap(const p3&p,const p3&pb,const bvol&b){
////		metrics::coldetsph++;
////		const p3 vec=p3(p,pb);
////		const float dst=vec.magn();
////		if(dst>(r+b.r))
////			return false;
////		return true;
////	}
////	bool anyboxdotinboxof(const p3&p,const m3&m,const bvol&bv){
////		flf();l()<<p<<m<<bv<<endl;
////		return false;
////	}
//	friend ostream&operator<<(ostream&,const bvol&);
//	friend istream&operator>>(istream&,bvol&);
//};
//ostream&operator<<(ostream&os,const bvol&b){os<<b.r<<",("<<b.v<<")";return os;}
//istream&operator>>(istream&is,bvol&bv){is>>bv.r;is.ignore(2);is>>bv.v;is.ignore();return is;}
//
//#include<vector>

class p3p:public p3{
public:
	p3 n;
	p3p(const p3&p,const p3&n):p3(p),n(n){}
};

#include <list>

class glob:public p3{
	const int id;
	glob&g;
	p3 a;
	list<glob*>chsrm;
	list<glob*>chsadd;
	int bits;
	m3 mmw;
	m3 mxmw;
	p3 mxmwpos;
	p3 mxmwagl;
	bool rmed;
	float r;
protected:
	list<glob*>chs;
public:
	p3 d;
	p3 np,nd;
	float m;
	static bool drawboundingspheres;
	static int drawboundingspheresdetail;

	glob(glob&g,const p3&p=p3(),const p3&a=p3(),const float r=1,const float density_gcm3=1):p3(p),id(metrics::globs++),g(g),a(a),bits(1),rmed(false),r(r),d(p3()),np(p3()),nd(p3()),m(density_gcm3*4/3*pi*r*r*r){
		if(&g==0)return;
		g.chsadd.push_back(this);
	}
	virtual~glob(){metrics::globs--;for(auto g:chs)delete g;chs.clear();}
	void rm(){
		if(rmed){
//			flf();l("rmingarmedobj")<<endl;
			return;
		}
		rmed=true;g.chsrm.push_back(this);
	}
	inline p3&agl(){return a;}
	inline glob&parent()const{return g;}
	inline int getid()const{return id;}
	inline const list<glob*>chls()const{return chs;}
	inline float radius()const{return r;}
	glob&radius(const float r){this->r=r;return*this;}
	inline const p3&angle()const{return a;}
	inline bool issolid()const{return bits&1;}
	inline glob&setsolid(const bool b){if(b)bits|=1;else bits&=0xfffffffe;return*this;}
	inline bool isblt()const{return bits&2;}
	inline glob&setblt(const bool b){if(b)bits|=2;else bits&=0xfffffffd;return*this;}
	inline bool iscoldetrec()const{return bits&4;}
	inline glob&setcoldetrec(const bool b){if(b)bits|=4;else bits&=0xfffffffb;return*this;}
	inline bool isitem()const{return bits&8;}
	inline glob&setitem(const bool b){if(b)bits|=8;else bits&=0xfffffff8;return*this;}
	inline bool iscolmx()const{return bits&16;}
	inline glob&setcolmx(const bool b){if(b)bits|=16;else bits&=0xfffffff0;return*this;}
	void coldet(glob&o){
		const p3 wpthis=g.posinwcs(*this);
		const p3 wpo=o.g.posinwcs(o);
		const p3 v(wpthis,wpo);
		const float d=v.magn();//? magn2
		const float dr=radius()+o.radius();
//		const float rr=dr*dr;
		metrics::coldetsph++;
//		flf();l()<<typeid(*this).name()<<"("<<wpthis<<")  "<<typeid(o).name()<<"("<<wpo<<")  "<<d<<"  "<<bv.r<<"   "<<o.bv.r<<endl;
		if(d>=dr){
			if(o.iscoldetrec()){
				for(auto gg:o.chs)
					coldet(*gg);
				return;
			}
			return;
		}
		if(issolid()&&o.issolid()){
			np.set(*this);
			nd.set(this->d);
			o.np.set(o);
			o.nd.set(o.d);
			oncol(o);
			o.oncol(*this);
			this->set(np);
			this->d.set(nd);
			o.set(o.np);
			o.d.set(o.nd);
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
	void culldraw(const int npl,const p3p pl[]){
//		flf();l("consider ")<<typeid(*this).name()<<"["<<id<<"]"<<endl;
		const float r=radius();
		for(int i=0;i<npl;i++){
			const p3p&pp=pl[i];
			const p3 v(pp,*this);
			const float t=v.dot(pp.n);
//			flf();l("t=")<<t<<"  "<<"v("<<v<<")"<<endl;
			if(t>0){// infront
				if(t>r){
//					flf();l("culled at p=(")<<*this<<")  r="<<r<<endl;
					metrics::cullview++;
					return;
				}
			}
		}
		metrics::globsrend++;
//		flf();l("included")<<endl;
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1,0,0);
		glRotatef(a.gety(),0,1,0);
		glRotatef(a.getz(),0,0,1);
		if(drawboundingspheres)drawboundingsphere();
		gldraw();
		for(auto g:chs){glPushMatrix();g->culldraw(npl,pl);glPopMatrix();}//? coordsyschange
	}
	void drawboundingsphere(){
		const GLbyte i=127;
		glColor3b(i,i,i);
		int detail=(int)(.4f*radius()*drawboundingspheresdetail);
		if(detail<drawboundingspheresdetail)
			detail=drawboundingspheresdetail;
//		glShadeModel(GL_SMOOTH);
		glutSolidSphere(radius(),detail,detail);
	}
	void draw(){
		metrics::globsrend++;
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1,0,0);
		glRotatef(a.gety(),0,1,0);
		glRotatef(a.getz(),0,0,1);
		if(drawboundingspheres)drawboundingsphere();
		gldraw();
		for(auto g:chs){glPushMatrix();g->draw();glPopMatrix();}
	}
	virtual void gldraw(){};
	virtual void tick(){
		chs.splice(chs.end(),chsadd);
		for(auto g:chs)g->tick();
		for(auto g:chsrm){chs.remove(g);delete g;}
		chsrm.clear();
	}
	virtual bool oncol(glob&o){metrics::collisions++;return &o==&o;}
protected:
	p3 posinwcs(const p3&p){refreshmxmw();p3 d;mxmw.mult(p,d);return d;}
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
};
bool glob::drawboundingspheres=true;
int glob::drawboundingspheresdetail=6;

class globx:public glob{
protected:
	bool ppsaved;
public:
	p3 dd;
	p3 f;
	p3 fi;
	p3 pp;
	float bf;
	globx(glob&g,const p3&p=p3(),const p3&a=p3(),const float r=1,float bounciness=1):glob(g,p,a,r),f(p3()),fi(p3()),pp(p),bf(bounciness){}
	inline p3&dp(){return d;}
	virtual void tick(){
		if(!ppsaved){
			pp.set(*this);
			ppsaved=false;//?
		}
//		flf();l()<<"f("<<f<<") fi("<<fi<<") m("<<m<<") dd("<<dd<<") d("<<d<<") ("<<*this<<") dt("<<dt()<<") "<<endl;
		dd=p3(f).transl(fi).scale(1/m).scale(dt());
		fi.set(0,0,0);
//		dd.transl(p3(0,-9.82f,0).scale(.1f),dt());
		d.transl(dd);
		transl(d);
//		const p3p gnd(p3(0,0,0),p3(0,1,0));
//		const float dy=gety()-radius()-gnd.gety();
//		if(dy<0){
////			flf();l()<<endl;
//			if(d.gety()!=0){
//				const float t=dy/d.gety();
//				transl(d,-t);
//				p3 nml(gnd.n);
//				nml.scale(d.dot(gnd.n));
//				nml.scale(-2,-2,-2);
//				d.transl(nml);
//				transl(d,1-t);
//				d.scale(bf);
//				const float ndy=gety()-radius()-gnd.gety();
//				if(ndy<0){
//					flf();l("!!!! dy(")<<gety()-radius()<<")"<<endl;
//					transl(0,-ndy,0);
//				}
//			}else{
//				flf();l("!!!")<<endl;
//				d.set(0,0,0);
//			}
//		}
		glob::tick();
	}
	bool solvesecdegeq(const float a,const float b,const float c,float&t1,float&t2)const{
		const float pt2=2*a;
		if(pt2==0){return false;}
		const float pt1=sqrt(b*b-4*a*c);
		if(pt1!=pt1){
			flf();l()<<" nan "<<endl;
			return false;
		}
		t1=(-b-pt1)/pt2;
		t2=(-b+pt1)/pt2;
//		if(t1!=t1&&t2!=t2){
//			flf();l()<<" t1 and t2 nan "<<endl;
//			return false;
//		}
		return true;
	}
	virtual bool oncol(glob&o){//? defunc
		glob::oncol(o);
//		flf();l()<<typeid(*this).name()<<"["<<this->getid()<<"]"<<endl;
		if(!o.issolid())return true;
		const p3&p1=*this;
		const p3&u1=this->d;
		const p3&p2=o;
		const p3&u2=o.d;
		const float r1=radius();
		const float r2=o.radius();
		const float r0=r1+r2;

//		const float a=p3(u1).pow2().sum()+p3(u2).pow2().sum()-2*u1.dot(u2);
//		const float b=2*p1.dot(u1)-2*p2.dot(u1)-2*p1.dot(u2)+2*p2.dot(u2);
//		const float c=-r0*r0+p3(p1).pow2().sum()+p3(p2).pow2().sum()-2*p1.dot(p2);

		const p3 du=p3(u2,u1);
		const float a=p3(du).pow2().sum();
		const p3 dp=p3(p2,p1);
		const float b=2*p3(dp).dot(du);
		const float c=p3(dp).pow2().sum()-r0*r0;
		float t1=0,t2=0;
		if(!solvesecdegeq(a,b,c,t1,t2)){
//			const float d=p3(p1,p2).magn();
//			flf();cout<<"t1="<<t1<<" t2="<<t2<<" a="<<a<<" d="<<d<<" dr="<<r0<<endl;
			return true;//? objects in collision but have no velocities
		}
		float t=min(t1,t2);
		if(t<-1)t=max(t1,t2);
		if(t>0)t=min(t1,t2);
		if(t<-1||t>0){
//			flf();l("no t with 0,1 t1=")<<t1<<" t2="<<t2<<" t="<<t<<"  u1("<<u1<<")"<<endl;
		}
		np.set(p1).transl(u1,t);
		p3 np2(p2);
		np2.transl(u2,t);
		const p3 nml(np,np2,true);

		p3 vu1(nml);
		vu1.scale(u1.dot(nml));

		p3 vu2(nml);
		vu2.scale(u2.dot(nml));

//		p3 v1(u1);
//		v1.transl(vu1,-1);
//		v1.transl(vu2, 1);

		// m1*u1+m2*u2=m1*v1+m2*v2
		const float m1=m;
		const float m2=o.m;
		const float mm=1/(m1+m2);
		p3 v1(u1);
		v1.transl(vu1,-1);
		v1.transl(vu1,(m1-m2)*mm);
		v1.transl(vu2,2*m2*mm);
//		flf();l()<<"nml("<<nml<<") u1("<<u1<<") u2("<<u2<<") vu1("<<vu1<<") vu2("<<vu2<<") v1("<<v1<<") m1("<<m1<<") m2("<<m2<<")"<<endl;
		nd.set(v1);
		np.transl(nd,dt()*(1-t));
		return true;
	}
};

class obcorpqb:public glob{
	static int n;
	float a,drscl,dr;
public:
	obcorpqb(glob&pt,const p3&p=p3(),const p3&a=p3(),const float r=1):glob(pt,p,a,r),a(.25f*n++),drscl(.5){
		setcolmx(true);
	}
	void gldraw(){glutSolidSphere(radius(),4,3);}
	void tick(){
		const float s=.1f;
		const float dx=rnd(-s,s);
		const float dy=rnd(-s,s);
		const float dz=0;
		const float r=1;
		transl(dt(dx),dt(dy),dt(dz));
		dr=drscl*sin(a);
		radius(r+dr);
		glob::tick();
	}
};
int obcorpqb::n=0;


class obcorp:public glob{
	static const float s;
	float f,ff;
public:
	obcorp(glob&pt,const p3&p=p3(),const p3&a=p3()):glob(pt,p,a){
		setcolmx(true);
//		radius(s+1.57f).setsolid(false);
		const float ds=.1f*s;
		const float dz=.5f*s;
		for(float zz=-s;zz<=s;zz+=dz)
			for(float xx=-s;xx<=s;xx+=ds)
				for(float yy=-s;yy<=s;yy+=ds){
					if(sqrt(xx*xx+yy*yy+zz*zz)>s)
						continue;
					new obcorpqb(pt,p3(xx,yy,zz).transl(p),p3(90,0,0));
				}
	}
	void gldraw(){
		glShadeModel(GL_FLAT);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glColor4f(.5,.5,.5,1);
	}
};
const float obcorp::s=7;


class obwom:public glob{
	int links;
public:
	obwom(glob&pt,const int links=4,const p3&p=p3()):glob(pt,p,p3(),.4f),links(links){
		if(links==0)
			return;
		setcoldetrec(true).setblt(true);
		new obwom(*this,links-1,p3(0,.4f,.4f));
	}
	void gldraw(){glColor3b(127,127,127);glutSolidSphere(radius(),6,6);}
	void tick(){if(links>0)agl().transl(0,dt(60),0);glob::tick();}
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
	void gldraw(){
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
	globo(glob&g,f3&f,const p3&pos=p3()):glob(g,pos,p3(),f.scale().magn()),f(f){metrics::globos++;}
	~globo(){metrics::globos--;}
	void gldraw(){f.gldraw();}
};

namespace f3s{
	f3*ufo=0;

	void load(){
		ufo=new f3("ufo.f3",p3(1.5,.25,1));//? leak
	}
	void clear(){
		if(ufo)delete ufo;
	}
}

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
						new globo(*this,*f3s::ufo,p3(p.getx()+xx,p.gety()+yy,p.getz()+zz));
	}
};

class obtex:public glob{
	GLuint gltx;
protected:
	int wihi;
	GLubyte*rgba;
	float s;
public:
	obtex(glob&g,const int wihi=4*32,const float s=1,const p3&p=p3(),const p3&a=p3(),const float r=1):glob(g,p,a,r),gltx(0),wihi(wihi),s(s){
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
protected:
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
	obcon(glob&g,const p3&p=p3(),const p3&a=p3(),const char*title="gnox console"):obtex(g,32*bp,1,p,a),p(rgba+wihi*bp+bp),title(title){
		setcolmx(true);
	}
	inline obcon&phom(){p=rgba+wihi*bp+bp;return*this;}
	inline obcon&prnt(const char*s){return prnt(strlen(s),s);}
	obcon&prnt(const size_t len,const char*s){
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
	void tick(){
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
	p3 po;
	float s;
	list<glob*>ls;
	list<glob*>lsmx;
	grid*grds[8];
	const size_t splitthresh=100;
	const int subgridlevels=4;
public:
	grid(const float size,const p3&p=p3()):po(p),s(size),grds({0,0,0,0,0,0,0,0}){metrics::ngrids++;}
	~grid(){metrics::ngrids--;clear();}
	void gldraw(){
		const GLbyte c=(GLbyte)(po.gety()/15*127);
		glColor3b(0,0,c);
		glPushMatrix();
		glTranslatef(po.getx(),po.gety(),po.getz());
		glutWireCube(s*2);
		glPopMatrix();
		for(auto gr:grds){
			if(!gr)
				continue;
			gr->gldraw();
		}
	}
	void clear(){
		ls.clear();
		lsmx.clear();
		for(auto&g:grds)
			if(g){
				g->clear();
				delete g;//? recycle
				g=0;
			}
	}
	void addall(const list<glob*>&ls){
		for(auto g:ls)
			putif(g,*g,g->radius());
		splitif(subgridlevels);//? splititonthefly
	}
	void coldet(){
		if(!ls.empty()){
			auto i1=ls.begin();
			while(true){
				auto i2=ls.rbegin();
				if(*i1==*i2)
					break;
				glob&g1=*(*i1);
				do{	glob&g2=*(*i2);
					g1.coldet(g2);
					i2++;
				}while(*i1!=*i2);
				i1++;
			}
			if(!lsmx.empty()){
				for(auto g1:ls){
					for(auto g2:lsmx){
						g1->coldet(*g2);
					}
				}
//				auto i1=ls.begin();
//				while(*i1){
//					glob&g1=*(*i1);
//					auto i2=lsmx.begin();
//					while(*i2){
//						glob&g2=*(*i2);
//						g1.coldet(g2);
//						i2++;
//					}
//					i1++;
//				}
			}
		}
		for(auto g:grds)
			if(g)
				g->coldet();
	}
private:
	bool putif(glob*g,const p3&p,const float r){
		if((p.getx()+s+r)<po.getx())return false;
		if((p.getx()-s-r)>po.getx())return false;
		if((p.getz()+s+r)<po.getz())return false;
		if((p.getz()-s-r)>po.getz())return false;
		if((p.gety()+s+r)<po.gety())return false;
		if((p.gety()-s-r)>po.gety())return false;
		if(g->iscolmx()){
			lsmx.push_back(g);
//			flf();l()<<lsmx.size()<<endl;
		}else
			ls.push_back(g);
		return true;
	}
	bool splitif(const int nrec){
//		if((ls.size())<splitthresh)//? alg
		if((ls.size()+lsmx.size())<splitthresh)//? alg
			return false;
		if(nrec==0)
			return false;
		const float ns=s/2;
		grds[0]=new grid(ns,p3(po).transl(-ns,ns,-ns));//?
		grds[1]=new grid(ns,p3(po).transl( ns,ns,-ns));
		grds[2]=new grid(ns,p3(po).transl(-ns,ns, ns));
		grds[3]=new grid(ns,p3(po).transl( ns,ns, ns));

		grds[4]=new grid(ns,p3(po).transl(-ns,-ns,-ns));
		grds[5]=new grid(ns,p3(po).transl( ns,-ns,-ns));
		grds[6]=new grid(ns,p3(po).transl(-ns,-ns, ns));
		grds[7]=new grid(ns,p3(po).transl( ns,-ns, ns));

		for(auto g:grds){
			for(auto o:ls)
				g->putif(o,*o,o->radius());//?
			for(auto o:lsmx)
				g->putif(o,*o,o->radius());//?
			g->splitif(nrec-1);//?
		}
		ls.clear();
		lsmx.clear();
		return true;
	}
};


class obball:public globx{
	float lft;
	float colr=1;
public:
	obball(glob&g,const p3&p,const float r=.05f,const float lft=2,const float bounciness=.3f):globx(g,p,p3(90,0,0),r,bounciness),lft(lft){
		setblt(true).setitem(true);
	}
	void gldraw(){}
	virtual void tick(){
		lft-=dt();
		if(lft<0){
			rm();
			return;
		}
		colr=1;
		globx::tick();
	}
};


class obiglo:public globx{
public:
	obiglo(glob&g,const p3&p,const float size=.05f):globx(g,p,p3(90,0,0),size){}
	bool oncol(glob&o){
		if(o.isblt()){
			radius(radius()-.01f);
			return true;
		}
		return globx::oncol(o);
	}
};

class wold:public glob{
	static wold wd;
	float t=0;
	grid grd;
	wold(const float r=15):glob(*(glob*)0,p3(),p3(),r),grd(r){}
	~wold(){f3s::clear();}
public:
	bool drawaxis=false,drawgrid=true,hidezplane=false,coldetbrute=false,coldetgrid=true;
	inline static wold&get(){return wd;}
	inline float gett(){return t;}
	inline void applyg(p3&dd)const{dd.transl(0,-9.82f,0);}
	void load(){
//		new obcon(*this,p3(radius(),0,radius()),p3(0,45,0));
//		new obcorp(*this,p3(0,4.2f,-6.5f));
//		new obcorp(*this,p3(0,0, 6.5f));
//		mkiglos();
//		mkexperiment();
		mkcradle();
	}
	void mkexperiment(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g;
		new obball(*this,p3( 0,    r,  0),r,lft,bounc);
		new obball(*this,p3( 0,r+2*r,  0),r,lft,bounc);
		new obball(*this,p3( 0,    r,2*r),r,lft,bounc);
		new obball(*this,p3( 0,r+2*r,2*r),r,lft,bounc);
		g=new obball(*this,p3(5, r*2,r),r,lft,bounc);
		g->d.set(-.01f,0,0);
//		new obball(*this,p3(0,r*1.5f,-5),r,lft,bounc);
	}
	void mkexperiment6(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g;
		g=new obball(*this,p3(0,r,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
		new obball(*this,p3(0,r,0),r,lft,bounc);
		g=new obball(*this,p3(0,r,-6),r,lft,bounc);
		g->d.set(0,0,.05f);
//		new obball(*this,p3(0,r*1.25f,-3),r,lft,bounc);
//		new obball(*this,p3(0,r*1.5f,-5),r,lft,bounc);
	}
	void mkexperiment5(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g;
		g=new obball(*this,p3(0,r*2,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
		new obball(*this,p3(0,r,0),r,lft,bounc);
		new obball(*this,p3(0,r,2),r,lft,bounc);
		g=new obball(*this,p3(0,r,-6),r,lft,bounc);
		g->d.set(0,0,.05f);
	}
	void mkexperiment4(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g;
		g=new obball(*this,p3(0,r,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
		new obball(*this,p3(0,r,0),r,lft,bounc);
		new obball(*this,p3(0,r,2),r,lft,bounc);
		g=new obball(*this,p3(0,r,-6),r,lft,bounc);
		g->d.set(0,0,.05f);
	}
	void mkexperiment3(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g;
		g=new obball(*this,p3(0,r,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
		new obball(*this,p3(0,r,-2),r,lft,bounc);
		new obball(*this,p3(0,r,0),r,lft,bounc);
		new obball(*this,p3(0,r,2),r,lft,bounc);
		g=new obball(*this,p3(0,r,-6),r,lft,bounc);
		g->d.set(0,0,.05f);
	}
	void mkexperiment2(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g;
		new obball(*this,p3(0,r,17),10*r,lft,bounc);
		g=new obball(*this,p3(0,r*2.7f,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
		new obball(*this,p3(0,r,2),r,lft,bounc);
		new obball(*this,p3(0,r,0),r,lft,bounc);
		new obball(*this,p3(0,r,-2),r,lft,bounc);
		new obball(*this,p3(0,r,-15),10*r,lft,bounc);
	}
	void mkexperiment1(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g=new obball(*this,p3(0,r,17),10*r,lft,bounc);
		g->d.set(0,0,0);
		g=new obball(*this,p3(0,r*2,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
//		new obball(*this,p3(0,r,2),r,lft,bounc);
		new obball(*this,p3(0,r,0),r,lft,bounc);
//		new obball(*this,p3(0,r,-2),r,lft,bounc);
//		g=new obball(*this,p3(0,r,-15),10*r,lft,bounc);
//		g->d.set(0,0,0);
	}
	void mkcradle(){
		const float r=1;
		const float lft=1000;
		const float bounc=1;
		globx*g=new obball(*this,p3(0,r,17),10*r,lft,bounc);
		g->d.set(0,0,0);
		g=new obball(*this,p3(0,r,6),r,lft,bounc);
		g->d.set(0,0,-.05f);
		new obball(*this,p3(0,r,2),r,lft,bounc);
		new obball(*this,p3(0,r,0),r,lft,bounc);
		new obball(*this,p3(0,r,-2),r,lft,bounc);
		g=new obball(*this,p3(0,r,-15),10*r,lft,bounc);
		g->d.set(0,0,0);
	}
	void mkiglos(){
		const float s=1;
		const float spread=-radius()/2;
		for(int n=0;n<50;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2);
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<40;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<30;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+1.5f*s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<20;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+2*s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
		for(int n=0;n<10;n++){
			const float xx=rnd(-spread,spread);
			const float yy=rnd(-s/2,s/2)+3*s;
			const float zz=rnd(-spread,spread);
			new obiglo(*this,p3(xx,yy,zz),s);
		}
	}
	void gldraw(){
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		const float r=radius();
		if(drawgrid&&coldetgrid)
			grd.gldraw();
		if(drawaxis){
			//glPolygonOffset
			glBegin(GL_LINE_STRIP);

			glColor3b(127,0,0);
			glVertex3f(0,0,0);

			glColor3b(127,0,0);
			glVertex3f(r,0,0);
			glVertex3f(0,0,0);

			glColor3b(0,0,0);
			glVertex3f(0,0,0);

			glColor3b(0,0,0);
			glVertex3f(0,r,0);

			glColor3b(0,0,127);
			glVertex3f(0,0,0);

			glColor3b(0,0,127);
			glVertex3f(0,0,r);
			glEnd();
		}
		if(!hidezplane){
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
		glob::tick();
		metrics::dtupd=clk::timerdt();

		clk::timerrestart();
		t+=dt();
		if(coldetgrid){
			grd.clear();
			grd.addall(chls());
			grd.coldet();
		}
		metrics::dtcoldetgrd=clk::timerdt();

		clk::timerrestart();
		if(coldetbrute){
			auto i1=chs.begin();//? chls() givescrash,const?
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
	}
};
wold wold::wd;


//template<typename T>class lut{
//private:
//	size_t size;
//	static int hash(const char*key,const size_t roll){
//		int i=0;
//		const char*p=key;
//		while(*p)i+=*p++;
//		i%=roll;
//		return i;
//	}
//	class el{
//	public:
//		const char*key;
//		T data;
//		el*nxt;
//		el(const char*key,T data):key(key),data(data),nxt(NULL){}
//		~el(){if(nxt)delete nxt;}
//	};
//	el**array;
//public:
//	lut(const size_t size=8):size(size){array=(el**)calloc(size,sizeof(el*));}
//	~lut(){clear();delete array;}
//	T operator[](const char*key)const{
//		const int h=hash(key,size);
//		el*l=array[h];
//		if(!l)
//			return (T)NULL;
//		while(1){
//			if(!strcmp(l->key,(char*)key))return l->data;
//			if(l->nxt){l=l->nxt;continue;}
//			return (T)NULL;
//		}
//		return (T)NULL;//?
//	}
//	void rm(const char*key){
//		const int h=hash(key,size);
//		el*l=array[h];
//		if(!l)
//			return;
//		el*lp=NULL;
//		while(1){
//			if(!strcmp(l->key,key)){
//				if(lp)
//					lp->nxt=l->nxt;
//				else
//					array[h]=NULL;
//				l->nxt=NULL;
//				delete l;
//				return;
//			}
//			lp=l;
//			if(l->nxt){
//				l=l->nxt;
//				continue;
//			}
//			return;
//		}
//	}
//	void put(const char*key,T data){
//		const int h=hash(key,size);
//		el*l=array[h];
//		if(!l){
//			array[h]=new el(key,data);
//			return;
//		}
//		while(1){
//			if(!strcmp(l->key,key)){
//				l->data=data;
//				return;
//			}
//			if(l->nxt){
//				l=l->nxt;
//				continue;
//			}
//			l->nxt=new el(key,data);
//			return;
//		}
//	}
//	void clear(){
//		for(size_t i=0;i<size;i++){
//			el*e=array[i];
//			if(!e)
//				continue;
//			delete e;
//			array[i]=NULL;
//		}
//	}
//};

#include<sys/time.h>
#include<iomanip>

#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>

namespace gloxnet{
	const int nplayers=2;
	const int nkeys=32;
	const int keyslen=nplayers*nkeys;
	const char*host="127.0.0.1";
	const char*port="8085";
	const char*playername="noname";
	bool sockio=true;

	char keys[nplayers][nkeys];
	int player=0;
	int sockfd=0;
	struct addrinfo*ai=0;
	void sendkeys(){
		const ssize_t bytes_sent=send(sockfd,keys[player],nkeys,0);
		flf();l("sent keys for player ")<<player<<"  bytessent("<<bytes_sent<<endl;
		if(bytes_sent==-1){flf();l(strerror(errno))<<endl;throw signl(1,"sendkeys");}
	}
	void print(){
		cout<<hex;
		for(int i=0;i<nplayers;i++){
			cout<<"k["<<i<<"](";
			for(int j=0;j<nkeys;j++){
				if(j>0)cout<<" ";
				cout<<int(keys[i][j]);
			}
			cout<<")"<<endl;
		}

	}
	void reckeys(){
		const ssize_t reclen=recv(sockfd,keys,keyslen,0);
		if(reclen==0){flf();l("closed")<<endl;exit(1);}
		if(reclen==-1){flf();l(strerror(errno))<<endl;exit(2);}
		if(reclen!=keyslen)throw signl(3,"uncompleterec");//?
//		print();
	}
	void start(){
		flf();l()<<"connect "<<host<<":"<<port<<endl;

		struct addrinfo hints;
		memset(&hints,0,sizeof hints);
		hints.ai_family=AF_UNSPEC;
		hints.ai_socktype=SOCK_STREAM;
		if(getaddrinfo(host,port,&hints,&ai)){flf();l(strerror(errno))<<endl;throw signl();}
		sockfd=socket(ai->ai_family,ai->ai_socktype,ai->ai_protocol);
		if(sockfd==-1){flf();l(strerror(errno))<<endl;return;}
	//	flf();l()<<"socket "<<sockfd<<"  errno("<<errno<<")"<<endl;
		if(connect(sockfd,ai->ai_addr,ai->ai_addrlen)){flf();l(strerror(errno))<<endl;throw signl();}
		flf();l("connected")<<endl;
		memset(keys,0,sizeof keys);
		if(!sockio){
			strncpy(keys[player],playername,nkeys);
		}else{
			string s="get /gloxnet .\r\ncookie:i=";
			s.append(playername).append("\r\n\r\n");
			const char*sc=s.c_str();
			flf();l(sc)<<endl;
			const size_t sclen=s.length();
			flf();l()<<sclen<<endl;
			const ssize_t bytes_sent=send(sockfd,sc,(size_t)sclen,0);
			if(bytes_sent!=(signed)sclen){flf();l(strerror(errno))<<endl;throw signl(1,"sockio");}
			const ssize_t bytes_sent2=send(sockfd,sc,(size_t)sclen,0);
			if(bytes_sent2!=(signed)sclen){flf();l(strerror(errno))<<endl;throw signl(2,"sockio");}
		}
		sendkeys();

		flf();l("connected. waiting for other players.")<<endl;
		reckeys();
		flf();l("all players connected.")<<endl;
		int i=0;
		for(auto s:keys){
			if(!strcmp(playername,s)){
				player=i;
				break;
			}
			i++;
		}
		flf();l("u r player #")<<player<<endl;
		print();
		memset(keys,0,sizeof keys);
	}
	void stop(){
		if(sockfd&&close(sockfd)){flf();l(strerror(errno))<<endl;}
		if(ai)freeaddrinfo(ai);
	}
}

class windo:public globx{
	m3 mxv;
//	lut<int>keysdn;
	bool gravity=true,dodrawhud=true,gamemode=false,fullscr=false,consolemode=false;
	float zoom;
	int wi,hi;
	int wiprv=wi,hiprv=hi;

	//? newclass
	float fwdbckrate=.01f;
	float straferate=.01f;
	float turnrate=270;
	float rocketforce=150*m;
	float rocketfuelburnrate=6;
	float smallflapimpulseforce=7*m;
	float smallflapfuelburn=.4f;
	float bigflapimpulseforce=15*m;
	float bigflapfuelburn=1;
	float leapimpulseforce=17*m;
	float leapfuelburn=2;
	float flapperyrecoveryrate=3;
	float flapperymax=1;
	float rocketryrecoveryrate=1;
	float rocketrymax=3;
	float initflappery=0;
	float initrocketry=0;

	float flappery=initflappery;
	float rocketry=initrocketry;
	int items;
public:
	void handlekeys(){
//		rain();
		pp.set(*this);ppsaved=true;
		mxv.mw(*this,agl());
//		flf();l("handlekeys")<<player<<endl;
		if(hdlkeydn('w')){fi.transl(mxv.zaxis().sety(0).norm().scale(-fwdbckrate));}
		if(hdlkeydn('s')){fi.transl(mxv.zaxis().sety(0).norm().scale(fwdbckrate));}
		if(hdlkeydn('d')){fi.transl(mxv.xaxis().scale(straferate));}
		if(hdlkeydn('a')){fi.transl(mxv.xaxis().scale(-straferate));}
		if(hdlkeydn('l')){agl().transl(0,dt(turnrate),0);}
		if(hdlkeydn('j')){agl().transl(0,-dt(turnrate),0);}
		if(hdlkeydn(',')&&rocketry>0){f.transl(0,dt(rocketforce),0);rocketry-=dt(rocketfuelburnrate);}else{f.set(0,0,0);}
		if(hdlkeydn(' ')){fire();}
		if(hdlkeydn('b')){rain();}
		if(hdlkeydn('t')){transl(mxv.yaxis(),dt(fwdbckrate));}
		if(hdlkeydn('g')){transl(mxv.yaxis(),dt(-fwdbckrate));}
		if(hdlkeytg(9)){togglehud();}// tab
		if(hdlkeytg('y')){zoom-=.1;}
		if(hdlkeytg('h')){zoom+=.1;}
		if(hdlkeytg('i')){if(flappery>0){fi.transl(0,smallflapimpulseforce,0);flappery-=smallflapfuelburn;}}
		if(hdlkeytg('k')){if(flappery>0){fi.transl(0,bigflapimpulseforce,0);flappery-=bigflapfuelburn;}}
		if(hdlkeytg('m')){if(flappery>0){fi.transl(mxv.zaxis().neg().negy().scale(leapimpulseforce));flappery-=leapfuelburn;}}
		if(hdlkeytg('x')){agl().transl(7,0,0);}
		if(hdlkeytg('c')){agl().transl(-7,0,0);}
		if(hdlkeytg('z')){agl().setx(0);}
		if(hdlkeytg('1')){glob::drawboundingspheres=!glob::drawboundingspheres;}
		if(hdlkeytg('2')){wold::get().drawaxis=!wold::get().drawaxis;}
		if(hdlkeytg('3')){wold::get().drawgrid=!wold::get().drawgrid;}
		if(hdlkeytg('4')){wold::get().hidezplane=!wold::get().hidezplane;}
		if(hdlkeytg('5')){wold::get().coldetgrid=!wold::get().coldetgrid;}
		if(hdlkeytg('6')){wold::get().coldetbrute=!wold::get().coldetbrute;}
		if(hdlkeytg('0')){togglefullscr();return;}
//		if(hdlkeytg(13)){inp<<endl;consolemode=!consolemode;}
//		if(hdlkeytg(127)){sts.str("");}// bkspc
		if(hdlkeytg(27)){if(fullscr)togglefullscr();cout<<endl;exit(0);}// esc
	}
public:
	int player=0;
	windo(glob&g=wold::get(),const p3&p=p3(10.4f,.1f,10.5f),const p3&a=p3(-21,-44.8f,0),const float r=.1f,const int width=1024,const int height=512,const float zoom=1.5):globx(g,p,a,r,.25f),zoom(zoom),wi(width),hi(height){}
	inline bool isgamemode()const{return gamemode;}
	inline bool isfullscreen()const{return fullscr;}
	inline int width()const{return wi;}
	inline int height()const{return hi;}
	inline windo&togglehud(){dodrawhud=!dodrawhud;return*this;}
	void reshape(const int width,const int height){
		sts<<"reshape("<<wi<<"x"<<hi<<")";wi=width;hi=height;
	}
	void drawframe(){
		cout<<"\rframe("<<metrics::frames++<<")";
		clk::timerrestart();

		glClearColor(.3f,.3f,1,1);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
//		glClearDepth(1);
//		glShadeModel(GL_SMOOTH);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,wi,hi);


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

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		const float viewangle_deg=45*zoom;
		gluPerspective(viewangle_deg,(GLdouble)wi/hi,.01,1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		const p3 lookat=p3(mxv.zaxis().neg()).transl(*this);
		gluLookAt(getx(),gety(),getz(), lookat.getx(),lookat.gety(),lookat.getz(), 0,1,0);
//		glRotatef(angle().getx(),1,0,0);
//		glRotatef(angle().gety(),0,1,0);
//		glRotatef(angle().getz(),0,0,1);
//		glTranslatef(-getx(),-gety(),-getz());
		GLfloat mf[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,mf);
		m3 m;
		m.set(mf);

		const p3 xaxis=m.xaxis();
		const p3 yaxis=m.yaxis();
		const p3 zaxis=m.zaxis();


		p3p backplane(*this,m.zaxis());

		const float viewangle_rad=degtorad(viewangle_deg);
		const float scrdst=(wi/2)/tan(viewangle_rad)/zoom;
		const float ww=wi/4;
		const float hh=hi/4;

//		flf();l()<<"scrdst="<<scrdst<<"   zaxis("<<zaxis<<")"<<endl;
		p3 ptr(*this);
		ptr.transl(p3(zaxis).neg().scale(scrdst));
		ptr.transl(p3(xaxis).scale(ww));
		ptr.transl(p3(yaxis).scale(hh));
//		flf();l()<<ptr<<endl;
		glPushMatrix();
		glTranslatef(ptr.getx(),ptr.gety(),ptr.getz());
		glColor3b(127,127,127);
		glutSolidCube(2);
		glPopMatrix();

		p3 pbr(*this);
		pbr.transl(p3(zaxis).neg().scale(scrdst));
		pbr.transl(p3(xaxis).scale(ww));
		pbr.transl(p3(yaxis).scale(-hh));
//		flf();l()<<ptr<<endl;
		glPushMatrix();
		glTranslatef(pbr.getx(),pbr.gety(),pbr.getz());
		glColor3b(127,0,0);
		glutSolidCube(2);
		glPopMatrix();

		p3 pbl(*this);
		pbl.transl(p3(zaxis).neg().scale(scrdst));
		pbl.transl(p3(xaxis).scale(-ww));
		pbl.transl(p3(yaxis).scale(-hh));
//		flf();l()<<ptr<<endl;
		glPushMatrix();
		glTranslatef(pbl.getx(),pbl.gety(),pbl.getz());
		glColor3b(127,127,0);
		glutSolidCube(2);
		glPopMatrix();

		p3 ptl(*this);
		ptl.transl(p3(zaxis).neg().scale(scrdst));
		ptl.transl(p3(xaxis).scale(-ww));
		ptl.transl(p3(yaxis).scale( hh));
//		flf();l()<<ptr<<endl;
		glPushMatrix();
		glTranslatef(ptl.getx(),ptl.gety(),ptl.getz());
		glColor3b(127,0,127);
		glutSolidCube(2);
		glPopMatrix();



		p3 rightplanenml(*this,p3());
		rightplanenml.vecprod(pbr,ptr).norm();
//		flf();l()<<"rightplanenml("<<rightplanenml<<endl;
		p3p rightplane(*this,rightplanenml);


		p3 leftplanenml(*this,p3());
		leftplanenml.vecprod(ptl,pbl).norm();
//		flf();l()<<"leftplanenml("<<leftplanenml<<endl;
		p3p leftplane(*this,leftplanenml);

		p3 topplanenml(*this,p3());
		topplanenml.vecprod(ptr,ptl).norm();
//		flf();l()<<"topplanenml("<<topplanenml<<endl;
		p3p topplane(*this,topplanenml);

		p3 btmplanenml(*this,p3());
		btmplanenml.vecprod(pbl,pbr).norm();
//		flf();l()<<"btmplanenml("<<btmplanenml<<endl;
		p3p btmplane(*this,btmplanenml);


		metrics::cullview=metrics::globsrend=0;
		p3p cullplanes[]{backplane,rightplane,leftplane,topplane,btmplane};
		parent().culldraw(5,cullplanes);
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
//	void timer(){
//		clk::tk++;
//		sts.str("");
//		handlekeys();
////		rain();
//		metrics::coldetsph=metrics::collisions=metrics::mwrefresh=metrics::mpmul=metrics::mmmul=0;
//		wold::get().tick();
//	}
//	void presskey(const char key){
//		keydn(key,0,0);
//	}
	void keydn(const char key,const int x,const int y){
		const int i=keyix(key);
		if(!i)return;
		int s=gloxnet::keys[player][i];
		if(s==1)return;
//		if(s!=0)throw signl(2,"lostkeybordsynch");
		gloxnet::keys[player][i]=1;
		sts<<"keydn("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
//
//		static char k[]={0,0};
//		k[0]=key;
//		const int ks=keysdn[k];
//		if(ks==1)return;
//		if(ks!=0){flf();return;}
//		keysdn.put(strcpy(new char[2],k),1);//? bug leak
	}
	void keyup(const char key,const int x,const int y){
		const int i=keyix(key);
		int s=gloxnet::keys[player][i];
		if(s==0)return;
		if(s==1)return;
		if(s!=2)throw signl(2,"unknownstate");
		gloxnet::keys[player][i]=0;
		sts<<"keyup("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";
//
//		static char k[]={0,0};
//		k[0]=key;
//		const int ks=keysdn[k];
//		if(ks==0){flf();return;}
//		if(ks==1)return;
//		if(ks!=2){flf();return;};
//		keysdn.rm(k);
	}
	void mouseclk(const int button,const int state,int x,const int y){sts<<"mousclk("<<state<<","<<button<<",["<<x<<","<<y<<",0])";}
	void mousemov(const int x,const int y){sts<<"mousmov("<<x<<","<<y<<")";}
	void tick(){
		flappery+=dt(flapperyrecoveryrate);
		if(flappery>flapperymax)flappery=flapperymax;
		rocketry+=dt(rocketryrecoveryrate);
		if(rocketry>rocketrymax)rocketry=rocketrymax;
		globx::tick();
	}
private:
	void togglefullscr(){
		if(gamemode)
			return;
		fullscr=!fullscr;
		if(fullscr){
			wiprv=wi;hiprv=hi;
			glutFullScreen();
			glutSetCursor(GLUT_CURSOR_NONE);
		}else{
			glutReshapeWindow(wiprv,hiprv);
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
	}
	int keyix(const char key){
		switch(key){
		case 'w':return 1;
		case 'j':return 2;
		case 's':return 3;
		case 'l':return 4;
		case 'a':return 5;
		case 'd':return 6;
		case 'i':return 7;
		case 'k':return 8;
		case 'm':return 9;
		case ',':return 10;
		case 'z':return 11;
		case 'x':return 12;
		case 'c':return 13;
		case ' ':return 14;
		case 'b':return 15;
		case 9:return 16;
		case 27:return 17;
		case 't':return 18;
		case 'g':return 19;
		case 'y':return 20;
		case 'h':return 21;
		case '1':return 22;
		case '2':return 23;
		case '3':return 24;
		case '4':return 25;
		case '5':return 26;
		case '6':return 27;
		case '7':return 28;
		case '8':return 29;
		case '9':return 30;
		case '0':return 31;
		default:return 0;
		}
	}
	bool hdlkeydn(const char key){
		const int i=keyix(key);
		if(!i)return false;
		int s=gloxnet::keys[player][i];
		if(s==0)return false;
		if(s==2)return true;
		if(s!=1)throw signl(2,"unknownstate");
		gloxnet::keys[player][i]=2;
		return true;
//
//		static char k[]={0,0};
//		k[0]=key;
//		const int ks=keysdn[k];
//		if(ks==0)return false;
//		if(ks==2)return true;
//		if(ks!=1)throw signl(2,"unknownstate");
//		keysdn.put(strcpy(new char[2],k),2);//? bug leak
//		return true;
	}
	bool hdlkeytg(const char key){
		const int i=keyix(key);
		if(!i)return false;
		int s=gloxnet::keys[player][i];
		if(s==0)return false;
		if(s==2)return false;
		if(s!=1)return false;
		gloxnet::keys[player][i]=2;
		return true;
//
//
//
//		static char k[]={0,0};
//		k[0]=key;
//		const int ks=keysdn[k];
//		if(ks==0)return false;
//		if(ks==2)return false;
//		if(ks!=1){flf();return false;}
//		keysdn.put(strcpy(new char[2],k),2);//? bug leak
//		return true;
	}

//	bool iskeydn(const char key,const bool setifnot=false,const bool handled=false){
//		static char k[]={0,0};
//		k[0]=key;
//		const bool b=(keysdn[k]==1);
//		const int s;
//		if(!b&&setifnot)
//			keysdn.put(strcpy(new char[2],k),handled?2:1);//? bug leak
//		return b;
//	}
	void rain(){
		static float fromheight=wold::get().radius()/2;
		static float a=0;
		const float r=wold::get().radius()/4;
		const float dr=r/2;
		const float dx=-2+rnd(-r,r);
		const float dz=rnd(-r,r);
		const float dy=rnd(-r,r)/2;
		a+=dt(60);
		for(int i=0;i<11;i++){
			globx&o=*new obball(wold::get(),p3(dx+r*cos(a)*rnd(-dr,dr),fromheight+dy,dz+r*sin(a)*rnd(-dr,dr)),.04f+rndn(.02f),1.2f,.1f+rndn(.03f));
			o.fi.set(4*o.m,0,2*o.m);
		}
//		for(int i=0;i<11;i++)
//			new obball(wold::get(),p3(getx()+rndn(dr),fromheight+rndn(dr/4),getz()+rndn(dr)));
	}
	float firereload=0;
	void fire(){
		firereload+=dt(20);if(firereload>1)firereload=1;
		if(firereload<1)return;
		firereload-=1;
		p3 lv=mxv.zaxis().neg();
		const float r=.05f;
		const float v=.5f;
		p3 vv=p3(d).transl(p3(lv).scale(v).transl(0,r,0));
		const float sprd=r/5;
		const float sx=rnd(-sprd,sprd);
		const float sy=rnd(-sprd,sprd);
		const float sz=rnd(-sprd,sprd);
		vv.transl(sx,sy,sz);
//		globx&o=*new obball(wold::get(),lv.scale(radius()+r).transl(sx,sy,sz).transl(*this),r);
		globx&o=*new obball(wold::get(),*this,r);
		o.d.set(vv);
	}
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
		const int dy=hi>>5;int y=-dy;

		timeval tv;gettimeofday(&tv,0);
		const tm&t=*localtime(&tv.tv_sec);
		ostringstream oss;
		oss<<setprecision(2)<<fixed;
		oss<<t.tm_hour<<":"<<":"<<t.tm_min<<":"<<t.tm_sec<<"."<<tv.tv_usec/1000<<"    t("<<wold::get().gett()<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(1);
		oss<<"p("<<*this<<") d("<<this->d<<") fi("<<this->fi<<") a("<<angle()<<") zoom("<<zoom<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(2);
		oss<<"frame("<<metrics::frames<<") globs("<<metrics::globs-metrics::globos<<") f3s("<<metrics::f3s<<") vbos("<<metrics::globos<<") p3s("<<metrics::p3s<<") m3s("<<metrics::m3s<<")";// bvols("<<metrics::bvols<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(3);
		oss<<"upd("<<metrics::dtupd<<")s   draw("<<metrics::dtrend<<")s    "<<((int)(metrics::globs/(metrics::dtrend?metrics::dtrend:1))>>10)<<"Kglobs/s    rendfpsest("<<(1/(metrics::dtrend+metrics::dtupd+metrics::dtcoldetgrd))<<")f/s";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(4);
		oss<<"coldet("<<(wold::get().coldetgrid?"grid":"")<<" "<<(wold::get().coldetbrute?"brute":"")<<") ngrids("<<metrics::ngrids<<") grid("<<metrics::dtcoldetgrd<<")s  "<<(((long long int)(metrics::globs/(wold::get().coldetgrid?metrics::dtcoldetgrd:metrics::dtcoldetbrute)))>>10)<<"Kglobs/s   brutedt("<<metrics::dtcoldetbrute<<")s";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<"sphcolsdet("<<metrics::coldetsph<<") sphcols("<<metrics::collisions<<")"<<" mxrfsh("<<metrics::mwrefresh<<")"<<" mvmul("<<metrics::mpmul<<") mmmul("<<metrics::mmmul<<") ";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(4);
		oss<<"cullview("<<metrics::cullview<<") globstorend("<<metrics::globsrend<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(4);
		oss<<"player("<<player<<") dtnet("<<metrics::dtnet<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		oss.str("");
		oss<<setprecision(1);
		oss<<"flappery("<<flappery<<") "<<"rocketry("<<rocketry<<") "<<"snowballs("<<items<<")";
		y+=dy;pl(oss.str().c_str(),y,0,1,.1f);

		y+=dy;pl(sts.str().c_str(),y,0,1,.1f);
		sts.str("");
	}
};

class windobot{
public:
	windo*wn;
	void tick(){
//		flf();
//		wn->keydn('j',0,0);
//		wn->keydn('w',0,0);
//		wn->keydn(' ',0,0);
//		wn->keydn('c',0,0);
	}
};

namespace glut{
	const int nplayers=2;
	bool multiplayer=false;
	windo*players[nplayers];//? bug deleted twice,wold and program exit
	windobot bot;
//	windo&wn=*new windo();
	void reshape(const int width,const int height){players[gloxnet::player]->reshape(width,height);}
	void draw(){
		players[gloxnet::player]->drawframe();
		glutSwapBuffers();
	}
	void timer(const int value){
		clk::tk++;
		sts.str("");
//		for(auto&o:players){
//			cout<<"p("<<o<<") a("<<o.angle()<<")"<<endl;
//		}
		if(multiplayer){
			clk::timerrestart();
			gloxnet::sendkeys();
			gloxnet::reckeys();
			metrics::dtnet=clk::timerdt();
//			gloxnet::print();
		}else{
			bot.tick();
		}
//		players[player].handlekeys();
//		players[0].handlekeys();
//		players[1].handlekeys();
		for(windo*o:players)
			o->handlekeys();
//		rain();
//		wn.timer();
		metrics::coldetsph=metrics::collisions=metrics::mwrefresh=metrics::mpmul=metrics::mmmul=0;
		wold::get().tick();
		glutPostRedisplay();
		glutTimerFunc((unsigned)value,timer,value);
	}
	void keydn(const unsigned char key,const int x,const int y){players[gloxnet::player]->keydn((char)key,x,y);}
	void keyup(const unsigned char key,const int x,const int y){players[gloxnet::player]->keyup((char)key,x,y);}
	void mouseclk(const int button,const int state,int x,const int y){players[gloxnet::player]->mouseclk(button,state,x,y);}
	void mousemov(const int x,const int y){players[gloxnet::player]->mousemov(x,y);}
	static void mainsig(const int i){cerr<<" ••• terminated with signal "<<i<<endl;exit(i);}
	int main(int argc,char**argv){
		for(int i=0;i<32;i++)signal(i,mainsig);//?
		srand(0);

		cout<<"glox";
		if(argc>2){
			cout<<" multiplayer"<<endl;
			multiplayer=true;
			gloxnet::host=argv[1];
			gloxnet::port=argv[2];
			gloxnet::playername=argv[3];
			cout<<"· connect to "<<gloxnet::host<<":"<<gloxnet::port<<endl;
			gloxnet::start();
		}
		const float r=wold::get().radius()/2;
		players[0]=new windo();
		players[0]->player=0;
		players[0]->set(-r,r,0);
		players[0]->agl().set(0,90,0);
		players[1]=new windo();
		players[1]->player=1;
		players[1]->set(r,1,0);
		players[1]->agl().set(0,-90,0);
		if(!multiplayer){
			bot.wn=players[0];
			gloxnet::player=1;
		}
		windo*plr=players[gloxnet::player];
		glutInit(&argc,argv);
		glutIgnoreKeyRepeat(true);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		if(plr->isgamemode()){
			glutGameModeString("1366x768:32");
			glutEnterGameMode();
			glutSetCursor(GLUT_CURSOR_NONE);
		}else {
		glutInitWindowSize(plr->width(),plr->height());
			glutCreateWindow("glox");
			if(plr->isfullscreen()){
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
//			cout<<setw(16)<<"bvol"<<setw(8)<<sizeof(bvol)<<endl;
			cout<<endl;
		}
		glutMainLoop();
		return 0;
	}
}
int main(int argc,char**argv){return glut::main(argc,argv);}
#endif
