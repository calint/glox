#ifndef __glox__
#define __glox__

#include<iostream>
using namespace std;

namespace glox{
	namespace clk{
		int dtms=10;
		float dt=dtms/1000.f;
		clock_t t0=clock();
		clock_t t1=t0;
		inline void timerrestart(){t1=clock();}
		inline clock_t timerdclk(){return clock()-t1;}
		inline float timerdt(){return (float)(clock()-t1)/CLOCKS_PER_SEC;}
	}
	namespace metrics{
		int globs=0;
		int coldetsph=0;
		int frames=0;
		float dtupd=0;
		float dtrend=0;
		int p3s;
		int m3s;
		int bvols;
		int collisions;
		int globos;
	}
	inline float dt(const float f){return f*clk::dt;}
	inline float rnd(const float from,const float tonotincluding){
		return from+(tonotincluding-from)*rand()/RAND_MAX;
	}
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
	inline float magn()const{return sqrt(x*x+y*y+z*z);}
	inline p3&set(const p3&p){x=p.x;y=p.y;z=p.z;return*this;}
	inline p3&set(const float x,const float y,const float z){this->x=x;this->y=y;this->z=z;return*this;}
	inline p3&neg(){x=-x;y=-y;z=-z;return*this;}
	inline p3&scale(const float s){x*=s;y*=s;z*=s;return*this;}
	inline p3&scale(const float sx,const float sy,const float sz){x*=sx;y*=sy;z*=sz;return*this;}
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

//#define mhello()cout<<"hello"<<endl;
#define flf()l("  ",__FILE__,__LINE__,__FUNCTION__);
static inline ostream&l(const char*s="",const char*file="",int lineno=0,const char*func=""){cerr<<file;if(lineno){cerr<<":"<<lineno;}cerr<<" "<<func<<"  "<<s;return cerr;}
//static inline ostream&ll(const char*s="",const char*file="",int lineno=0,const char*func=""){return l(s,file,lineno,func)<<endl;}

class m3{
	float xx,xy,xz,xo;
	float yx,yy,yz,yo;
	float zx,zy,zz,zo;
public:
	inline m3(){metrics::m3s++;}
	inline ~m3(){metrics::m3s--;}
	m3&ident(){xx=1;xy=0;xz=0;xo=0; yx=0;yy=1;yz=0;yo=0; zx=0;zy=0;zz=1;zo=0;return*this;}
	const m3&vx(p3&p)const{p.set(xx,xy,xz);return*this;}
	const m3&vy(p3&p)const{p.set(yx,yy,yz);return*this;}
	const m3&vz(p3&p)const{p.set(zx,zy,zz);return*this;}
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
	const m3&mult(const p3&src,p3&dst)const{
		const float x=src.getx();
		const float y=src.gety();
		const float z=src.getz();
		float rx=x*xx+y*xy+z*xz+xo;
		float ry=x*yx+y*yy+z*yz+yo;
		float rz=x*zx+y*zy+z*zz+zo;
		dst.set(rx,ry,rz);
		return*this;
	}
	friend ostream&operator<<(ostream&,const m3&);
	friend istream&operator>>(istream&,m3&);
};
ostream&operator<<(ostream&os,const m3&m){
	cout<<"["<<p3(m.xx,m.xy,m.xz)<<","<<m.xo<<"] [";
	cout<<p3(m.yx,m.yy,m.yz)<<","<<m.yo<<"] [";
	cout<<p3(m.zx,m.zy,m.zz)<<","<<m.zo<<"]";
	return os;
}
istream&operator>>(istream&is,m3&m){
	p3 p;
	is.ignore();
	is>>p;
	m.xx=p.getx();m.xy=p.gety();m.xz=p.getz();
	is.ignore();
	is>>m.xo;
	is.ignore(3);
	is>>p;
	m.yx=p.getx();m.yy=p.gety();m.yz=p.getz();
	is.ignore();
	is>>m.yo;
	is.ignore(3);
	is>>p;
	m.zx=p.getx();m.zy=p.gety();m.zz=p.getz();
	is.ignore();
	is>>m.zo;
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
public:
	float r;
	p3 v;
	bvol(const float sphereradius,const p3&boxcorner):r(sphereradius),v(boxcorner){
		metrics::bvols++;
	}
	~bvol(){metrics::bvols--;}
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
public:
	bvol bv;
	m3 mw;
	static bool drawboundingspheres;
	static int drawboundingspheresdetail;
	glob(glob&g,const p3&p=p3(),const p3&a=p3(),const float r=0,const p3&pbox=p3()):p3(p),id(metrics::globs++),g(g),a(a),bv(r,pbox){
		if(&g==0)
			return;
		g.chsadd.push_back(this);
	}
	virtual ~glob(){
//		cout<<"~glob("<<id<<")";
		metrics::globs--;
		for(auto i=chs.begin();i!=chs.end();i++)
			delete*i;
		chs.clear();
	}
	inline p3&agl(){return a;}
//	inline glob&seta(const p3&a){this->a.set(a);return*this;}
	void draw(){
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1,0,0);
		glRotatef(a.gety(),0,1,0);
		glRotatef(a.getz(),0,0,1);
		gldraw();
		if(drawboundingspheres){
			const GLbyte i=(GLbyte)rnd(0,32);
			glColor3b(i,0,0);
			glDisable(GL_LIGHTING);
			glutWireSphere(bv.r,drawboundingspheresdetail,drawboundingspheresdetail);
			glEnable(GL_LIGHTING);
		}
//		for(auto g:chs){
//			glPushMatrix();
//			g->draw();
//			glPopMatrix();
//		}
		for(auto i=chs.begin();i!=chs.end();i++){
//		for(list<glob*>::iterator i=chs.begin();i!=chs.end();i++){
			glPushMatrix();
			(*i)->draw();
			glPopMatrix();
		}
	}
	virtual void gldraw(){};
	virtual void tick(){
		if(!chsadd.empty())
			chs.splice(chs.end(),chsadd);
		if(!chsadd.empty())
			cerr<<"pock"<<endl;
		for(auto i=chs.begin();i!=chs.end();i++){
			(*i)->tick();
		}
		for(auto i=chsrm.begin();i!=chsrm.end();i++){
			chs.remove((*i));
			delete*i;
		}
		chsrm.clear();
	}
	inline glob&getglob()const{return g;}
	virtual bool oncol(glob&o){
		metrics::collisions++;
//		cerr<<" collision "<<typeid(*this).name()<<"["<<this->id<<"] "<<typeid(o).name()<<"["<<o.id<<"]"<<endl;
		return &o!=0;
	}
	void rm(){
//		g.chs.remove(this);
		g.chsrm.push_back(this);
	}
};
bool glob::drawboundingspheres;
int glob::drawboundingspheresdetail=7;

//class obteapot:public glob{
//	float dy;
//public:
//	obteapot(glob&pt):glob(pt),dy(.1f){}
//	void gldraw(){
//		glPushAttrib(GL_ENABLE_BIT);
//		glEnable(GL_CULL_FACE);
//		glFrontFace(GL_CW);
//		glEnable(GL_LIGHTING);
//		glEnable(GL_LIGHT0);
//		glutSolidTeapot(1);
//		glPopAttrib();
//	}
//	virtual void tick(){
//		glob::tick();
//		transl(0,dt(dy),0);
//		if(gety()>10||gety()<0)dy=-dy;
//	}
//};

class obcorpqb:public glob{
	float r;
	static int n;
	float a,drscl,dr;
public:
	obcorpqb(glob&pt,const float r=1):glob(pt),r(r),a(.25f*n++),drscl(.5){}
	void gldraw(){
		glutSolidSphere(r+dr,4,3);
	}
	virtual void tick(){
		const float s=.1f;
		const float dx=rnd(-s,s);
		const float dy=rnd(-s,s);
		const float dz=0;
		transl(dt(dx),dt(dy),dt(dz));
		dr=drscl*sin(a);
		bv.r=r+dr;
		glob::tick();
	}
};
int obcorpqb::n=0;


class obcorp:public glob{
public:
	static const float s;
	obcorp(glob&pt,const p3&p,const p3&a):glob(pt,p,a){
		bv.r=s+1.4f;
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
	GLfloat f,ff;
	void gldraw(){
		glShadeModel(GL_FLAT);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		GLfloat matspec[]={ff,ff,ff,1};
		ff+=dt(1/10);if(ff>1)ff=0;
		glMaterialfv(GL_FRONT,GL_SPECULAR,matspec);
		GLfloat matshin[]={f};
		f++;if(f>128)f=0;
		glMaterialfv(GL_FRONT,GL_SHININESS,matshin);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE) ;
		glEnable(GL_COLOR_MATERIAL) ;
		glColor3b(127,127,127);

	}
};
const float obcorp::s=7;

//
//class obquad:public glob{
//public:
//	obquad(glob&g):glob(g){}
//	void gldraw(){
//		const float s=15.f;
//
//		glColor3b(0,0,0x7f);
//		glBegin(GL_QUADS);
//		glVertex2f(-s,-s);
//		glVertex2f( s,-s);
//		glVertex2f( s, s);
//		glVertex2f(-s, s);
//		glEnd();
//
//		glColor3b(0,0,127);
//		glBegin(GL_LINE_STRIP);
//		glVertex3f(s,0,.01f);
//		glVertex3f(0,0,.01f);
//		glColor3b(127,127,127);
//		glVertex3f(0,s,.02f);
//		glEnd();
//
//		const float r=.75*s;
//		glPushMatrix();
//		glTranslatef(0,0,.01f);
//		glColor3b(0,0x7f,0);
//		glBegin(GL_TRIANGLE_FAN);
//		glVertex2f(0,0);
//		glVertex2f(r,0);
//		const float dtr=3.14159/180;
//		const int di=360/12;
//		for(int i=di;i<=360;i+=di){
//			const float rd=i*dtr;
//			glVertex2f(r*cos(rd),r*sin(rd));
//		}
//		glEnd();
//		glPopMatrix();
//	}
//	virtual void tick(){
//		agl().transl(dt(360/60),0,0);
//		glob::tick();
//	}
//};
//
//
//class obwom:public glob{
//public:
//	obwom(glob&pt,const int links):glob(pt){
//		if(links==0)
//			return;
//		glob*o=new obwom(*this,links-1);
//		o->transl(0,.4f,0);
//	}
//	void gldraw(){
//		glPushAttrib(GL_ENABLE_BIT);
//		glShadeModel(GL_SMOOTH);
////		glShadeModel(GL_FLAT);
//		glEnable(GL_CULL_FACE);
//		glFrontFace(GL_CCW);
//		glEnable(GL_LIGHTING);
//		glEnable(GL_LIGHT0);
//		glColor3b(0,0x20,0x60);
//		const float dr=rnd(0,.1f);
////		const float dr=0;
//		const float r=.4;
//		glutSolidSphere(r+dr,6,6);
////		glutSolidCube(r+dr);
//		glPopAttrib();
//	}
//	virtual void tick(){
//		glob::tick();
//		agl().transl(dt(60),0,dt(60));
////		a.transl(0,d(60),d(60));
////		a.transl(d(60),0,0);
////		a.transl(d(60),d(60),0);
//	}
//};
//


class obball:public glob{
	p3 dp;
	float lft;
public:
	obball(glob&g,const p3&p,const float r=.05f):glob(g,p,p3(-180,0,0),r),dp(p3()),lft(0){}
	inline p3&getdp(){return dp;}
	virtual void gldraw(){
		glShadeModel(GL_SMOOTH);
//		glScalef(1,1,2);

//		GLfloat matspec[]={127,0,0,1};
//		glMaterialfv(GL_FRONT,GL_SPECULAR,matspec);
//		GLfloat matshin[]={127};
//		glMaterialfv(GL_FRONT,GL_SHININESS,matshin);

//		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		glColor3f(.5f,.5f,1);
//		glColor3b(0,0,127);

		glutSolidSphere(bv.r,3,7);
	}
	virtual void tick(){
		lft+=dt(1);
		if(lft>10){
			rm();
			return;
		}
		dp.transl(0,dt(-2),dt(-9.f));
		agl().transl(0,0,dt(720));
		transl(dt(dp.getx()),dt(dp.gety()),dt(dp.getz()));
		if(getz()<bv.r){
			dp.scale(0,0,-.5f);
			transl(0,0,bv.r-getz());
		}
		glob::tick();
	}
	virtual bool oncol(glob&o){
		const float dx=rnd(-1,1)*dt(bv.r);
		const float dy=rnd(-1,1)*dt(bv.r);
		const float dz=rnd(-1,1)*dt(bv.r);
		if(&o)
		transl(dx,dy,dz);
		return true;
	}
};

namespace db{
	const int sq_pt_dim=2;
	const GLfloat sq_pt[]={0,0, 1,0, 1,1, 0,1};
	const int sq_pt_nbytes=sizeof(sq_pt);
	const GLubyte sq_ix[]={0,1,2,3,0};
	const int sq_ix_nbytes=sizeof(sq_ix);
	const int sq_ix_count=sq_ix_nbytes/sizeof(GLubyte);
}
#include<fstream>
class globo:public glob{
	GLuint glpt;
	GLuint glix;
protected:
	virtual void getvtxs(const GLfloat*&buf,int&bufsizebytes,int&floatspervertex){
		bufsizebytes=db::sq_pt_nbytes;
		floatspervertex=db::sq_pt_dim;
		buf=db::sq_pt;
	}
	virtual void getixs(const GLubyte*&buf,int&bufsizebytes,int&ixcount){
		bufsizebytes=db::sq_ix_nbytes;
		ixcount=db::sq_ix_count;
		buf=db::sq_ix;
	}
public:
	int vtxbufsizebytes,vtxdim,ixbufsizebytes,ntri;
	globo(glob&g,const char*filepath="ufo.f3d",const p3&scale=p3(1,1,1),const p3&p=p3()):glob(g,p),glpt(0),glix(0){
		metrics::globos++;
		agl().transl(90,0,0);

		ifstream ifs(filepath);
		if(!ifs.good())throw signl(-1,filepath);
		ifs>>skipws;
		int n;
		ifs>>n;
		vtxdim=3;
		GLfloat*pt=new GLfloat[(unsigned long)(n*vtxdim)];//? bug leak
		vtxbufsizebytes=n*vtxdim*(int)sizeof(GLfloat);
		GLfloat*pp=pt;
		while(n--){
			ifs>>*pp;*pp*=scale.getx();pp++;
			ifs>>*pp;*pp*=scale.gety();pp++;
			ifs>>*pp;*pp*=scale.getz();pp++;
		}
		glGenBuffers(1,&glpt);
		glBindBuffer(GL_ARRAY_BUFFER,glpt);
		glBufferData(GL_ARRAY_BUFFER,vtxbufsizebytes,pt,GL_STATIC_DRAW);
//		delete pt;

		ifs>>n;
		ntri=n;
		GLubyte*ix=new GLubyte[(unsigned int)ntri*3];
		ixbufsizebytes=(int)sizeof(GLubyte)*ntri*3;
		GLubyte*p1=ix;
		while(n--){
			int i0,i1,i2;
			int nv;ifs>>nv;
			if(nv!=3)throw signl(-3,"surfacenottriangle");
			ifs>>i0;*p1++=(GLubyte)i0;
			ifs>>i1;*p1++=(GLubyte)i1;
			ifs>>i2;*p1++=(GLubyte)i2;
			GLshort rgb[3];
			ifs>>rgb[0]>>rgb[1]>>rgb[2];
			cout<<rgb[0]<<" "<<rgb[1]<<endl;
		}
		ifs.close();
//		cout<<filepath<<"(nvtx("<<vtxbufsizebytes/vtxdim/(int)sizeof(GLfloat)<<") nix("<<ntri<<"))"<<endl;

		glGenBuffers(1,&glix);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glix);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,ixbufsizebytes,ix,GL_STATIC_DRAW);
//		delete ix;
	}
	~globo(){
		if(glpt)glDeleteBuffers(1,&glpt);
		if(glix)glDeleteBuffers(1,&glix);
		metrics::globos--;
	}
	virtual void gldraw(){
		glBindBuffer(GL_ARRAY_BUFFER, glpt);
		glVertexAttribPointer(0,vtxdim,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(0);

//		glPointSize(3);
//		glColor3b(0,0,0);
//		glDrawArrays(GL_POINTS,0,vtxbufsizebytes/vtxdim);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glix);
		glDrawElements(GL_TRIANGLES,ixbufsizebytes,GL_UNSIGNED_BYTE,0);
	}
};

#include<typeinfo>

class wold:public glob{
	static wold wd;
	float s;
	wold():glob(*(glob*)0),s(15),ddegx(0),ddegz(.1f){
		agl().transl(-111,0,0);
//		transl(0,-.3,0);
		bv.r=s;
		new obcorp(*this,p3(0,0,4.2f),p3(90,0,0));
//		new obball(*this,p3(0,0,10));
//		new obball(*this,p3(.1f,0,10));
	}
//	~wold(){cout<<endl<<" ~wold() ";}
public:
	inline static wold&get(){return wd;}

	bool drawaxis,drawgrid,hidezplane;
	float ddegx,ddegz;
	void initvbo(){
		new globo(*this,"ufo.f3d",p3(3,.5,2),p3(0,0,20));
	}
	void gldraw(){
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);

		if(drawgrid){
			glColor3b(0,0,0x7f);
	//		//glutWireCube(s);
			glBegin(GL_LINE_STRIP);
			glVertex2f(-s,-s);
			glVertex2f( s,-s);
			glVertex2f( s, s);
			glVertex2f(-s, s);
			glVertex2f(-s,-s);
			glEnd();
		}
		if(drawaxis){
			//glPolygonOffset
			glBegin(GL_LINE_STRIP);

			glColor3b(127,127,127);
			glVertex3f(s,0,0);

			glColor3b(127,127,127);
			glVertex3f(0,0,0);

			glColor3b(0,0,0);
			glVertex3f(0,s,0);

			glColor3b(0,0,0);
			glVertex3f(0,0,0);

			glColor3b(0,0,127);
			glVertex3f(0,0,s);
			glEnd();
		}
		if(!hidezplane){
			const float r=s;
			glPushMatrix();
	//		glTranslatef(0,0,01f);
			glColor3b(0,0x7f,0);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0,0);
			glVertex2f(r,0);
			const float dtr=3.14159f/180;
			const int di=360/24/2/2;
			for(int i=di;i<=360;i+=di){
				const float rd=i*dtr;
				glVertex2f(r*cos(rd),r*sin(rd));
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
		agl().transl(-dt(ddegx),0,dt(ddegz));
		list<glob*>::iterator i1=chs.begin();
		while(true){
			list<glob*>::reverse_iterator i2=chs.rbegin();
			if(*i1==*i2)
				break;
			glob&g1=*(*i1);
			do{
				glob&g2=*(*i2);
				if(g1.bv.spheresoverlap(g1,g2,g2.bv)){
					g1.oncol(g2);
					g2.oncol(g1);
				}
				i2++;
			}while(*i1!=*i2);
			i1++;
		}
		glob::tick();
	}
};
wold wold::wd;

//template<typename T>class lut;
//template<typename T>ostream&operator<<(ostream&,const lut<T>&);
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
//		friend ostream&operator<< <T>(ostream&,const lut<T>&);
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
//	friend ostream&operator<< <T>(ostream&,const lut<T>&);
};
//template<typename T>ostream&operator<< <T>(ostream&,const lut<T>&){
//
//}
//template<typename T>ostream&operator<<(ostream&os,const lut<T>&lt){
//	for(size_t i=0;i<lt.size;i++){
//		lut<T>::el*l=lt.array[i];
//		if(!l)continue;
//		while(l){
////			os<<(l.key)<<"("<<l.data<<")";
//			l=l->nxt;
////		}
//	}
//	return os;
//}


#include<sys/time.h>
#include<sstream>
#include <iomanip>

class windo:public glob{
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
		const int dy=h>>5;
		int y=dy>>2;

		pl("glox",y,0,1,.2f);

		const p3&a=wold::get().agl();
		timeval tv;gettimeofday(&tv,0);
		const tm&t=*localtime(&tv.tv_sec);
		ostringstream oss;
		oss<<setprecision(2)<<fixed;
		oss<<t.tm_hour<<":"<<":"<<t.tm_min<<":"<<t.tm_sec<<"."<<tv.tv_usec/1000;
		oss<<setprecision(3);
		oss<<"          rend.dt("<<metrics::dtrend<<")s   upd.dt("<<metrics::dtupd<<")s     "<<((int)(metrics::globs/(metrics::dtrend?metrics::dtrend:1))>>10)<<"Kglobs/s    rendonly "<<(1/metrics::dtrend)<<"fps";
		y=h-dy;pl(oss.str().c_str(),y,0,1,.1f);


		oss.str("");
		oss<<setprecision(2);
		oss<<"frame("<<metrics::frames<<") globs("<<metrics::globs-metrics::globos<<") vbos("<<metrics::globos<<") p3s("<<metrics::p3s<<") m3s("<<metrics::m3s<<") bvols("<<metrics::bvols<<") sphdet("<<metrics::coldetsph<<") sphcols("<<metrics::collisions<<") xz("<<a.getx()<<" "<<a.getz()<<") p("<<*this<<")";
//		oss<<"keys("<<glut::keysdn<<")";
		y-=dy;pl(oss.str().c_str(),y,0,1,.1f);
	}
	char ccounter;
public:
	int w,h;
	windo(const p3&p):glob(wold::get()){set(p);bv.r=2;}
	void drawframe(){
		cout<<"\rframe("<<metrics::frames++<<")";
//		glClearColor(0,0,0,1);
		glClearColor(.5f,.5f,1,1);

		glEnable(GL_CULL_FACE);
//		glFrontFace(GL_CCW);
//		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glClearDepth(1);

		glShadeModel(GL_SMOOTH);

//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

//		glEnable(GL_LIGHTING);
//		glEnable(GL_LIGHT0);
//		static float lht;
//		static float dlht=1;
//		static float lha;
//		static float dlha=60*3.1415f/180;
//		const float lhx=lht*cos(lha);
//		const float lhy=lht*sin(lha);
//		const GLfloat lhtpos[]={lhx,lhy,0,1};
//		lht+=dt(dlht);
//		lha+=dt(dlha);
//		glLightfv(GL_LIGHT0,GL_POSITION,lhtpos);
//		const GLfloat lhtcol[]={0,0,0,1};
//		glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,lhtcol);


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45,(GLdouble)w/h,.01,10000);
		glTranslatef(-getx(), -gety(), -getz());
//		gluLookAt(getx(),gety(),getz(), 0,0,0, 0,1,0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
//		glRotatef(-a.getz(), 0, 0, 1);
//		glRotatef(-a.gety(), 0, 1, 0);
//		glRotatef(-a.getx(), 1, 0, 0);
		getglob().draw();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,w,0,h,0,1);
		glColor3b(0x7f,0x7f,0x7f);
		drawhud();
		cout<<flush;
	}
};

extern void gnox();

namespace glut{
	int w=512,h=512,__w=w,__h=h;
	lut<int>keysdn;
	bool gamemode=false;
	bool fullscr=false;
	windo&wn=*new windo(p3(0,6,15));
	bool nl;
	void reshape(const int width,const int height){
		cout<<" reshape("<<w<<"x"<<h<<")";nl=true;
		w=width;h=height;
	}
	void draw(){
		wn.w=w;wn.h=h;
		clk::timerrestart();
		wn.drawframe();
		metrics::dtrend=clk::timerdt();
		metrics::coldetsph=metrics::collisions=0;
		glutSwapBuffers();
		if(nl){
			cout<<endl;
			nl=false;
		}
	}
	bool iskeydn(const unsigned char key,const bool setifnot=0){
		static char k[]={0,0};
		k[0]=(char)key;
		const bool b=keysdn[k]==1;
		if(!b&&setifnot)
			keysdn.put(strcpy(new char[2],k),1);//? bug leak
		return b;
	}
	void timer(const int value){
		static float a=0;
		static float dr=2;
		static float fromheight=20;
		if(iskeydn('r')&&iskeydn('u')){wn.transl(0,dt(1),0);}
		if(iskeydn('v')&&iskeydn('n')){wn.transl(0,-dt(1),0);}
		if(iskeydn('a')){for(int i=0;i<11;i++)new obball(wold::get(),p3(dr*cos(a)*rnd(-dr,dr),dr*sin(a)*rnd(-dr,dr),fromheight+rnd(0,dr)));}
		if(iskeydn('i')){wold::get().transl(0,0,dt(10));}
		if(iskeydn('k')){wold::get().transl(0,0,-dt(10));}
		a+=dt(360);

		clk::timerrestart();
		wold::get().tick();
		metrics::dtupd=clk::timerdt();

		glutPostRedisplay();
		glutTimerFunc((unsigned)value,timer,value);
	}
	void keydn(const unsigned char key,const int x,const int y){
		if(iskeydn(key,true))return;
		cout<<"keydn("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";nl=true;
		if(key=='0'){
			fullscr=!fullscr;
			if(fullscr){
				__w=w;__h=h;
				glutFullScreen();
				glutSetCursor(GLUT_CURSOR_NONE);
			}else{
				glutReshapeWindow(__w,__h);
				glutSetCursor(GLUT_CURSOR_INHERIT);
			}
			return;
		}

		wold&wd=wold::get();
		if(key==0){throw "keyo";}
		else if(key=='j'){wd.ddegz-=360/60;}
		else if(key=='f'){wd.ddegz+=360/60;}
		else if(key=='t'){wd.ddegx-=360/60;}
		else if(key=='g'){wd.ddegx+=360/60;}
		else if(key==' '){wd.ddegz=wd.ddegx=0;}
		else if(key=='y'){wd.ddegz=wd.ddegx=0;wd.agl().set(p3(270,0,0));}
		else if(key=='h'){wd.ddegz=wd.ddegx=0;wd.agl().set(p3(90.5,0,0));}
		else if(key=='i'){wn.transl(0,0,-1);}
		else if(key=='k'){wn.transl(0,0,1);}
		else if(key=='1'){glob::drawboundingspheres=!glob::drawboundingspheres;}
		else if(key=='2'){wd.drawaxis=!wd.drawaxis;}
		else if(key=='3'){wd.drawgrid=!wd.drawgrid;}
		else if(key=='4'){wd.hidezplane=!wd.hidezplane;}
	}
	void keyup(const unsigned char key,const int x,const int y){
		const char k[]={(char)key,0};
		keysdn.rm(k);//? whatif 1 and not handled
		cout<<"keyup("<<(int)key<<",["<<x<<","<<y<<"],"<<key<<")";nl=true;
		if(key==27)// esc
		{glutReshapeWindow(w,h);exit(0);}
	}
	void mouseclk(const int button,const int state,int x,const int y){cout<<"mousclk("<<state<<","<<button<<",["<<x<<","<<y<<",0])"<<endl;}
	//void idle(){
	//	printf("idle\n");
	//	return;
	//}
	void mousemov(const int x,const int y){cout<<"mousmov("<<x<<","<<y<<")"<<endl;}
	static void mainsig(const int i){cerr<<" ••• terminated with signal "<<i<<endl;exit(i);}
//	static void mainxit(){
//		if(metrics::nglobs==0){
//			cout<<" globs recycled"<<endl;
//			return;
//		}
//		cout<<" !¡! globsdealloc  "<<metrics::nglobs<<" "<<endl;
//	}
	int main(int argc,char**argv){
//		atexit(mainxit);
		cout<<"glox ";
		for(int i=0;i<32;i++)signal(i,mainsig);//?
		srand(0);
		gnox();
		glutInit(&argc,argv);
		glutIgnoreKeyRepeat(true);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		if(gamemode){
			glutGameModeString("1366x768:32");
			glutEnterGameMode();
			glutSetCursor(GLUT_CURSOR_NONE);
		}else{
			glutInitWindowSize(w,h);
			glutCreateWindow("glox");
			if(fullscr){
				glutFullScreen();
				glutSetCursor(GLUT_CURSOR_NONE);
			}
		}

		wold::get().initvbo();

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
		}
		cout<<endl;
		glutMainLoop();
		return 0;
	}
}

int main(){return glut::main(0,NULL);}

extern void gnox(){}
#endif
