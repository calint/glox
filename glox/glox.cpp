#ifndef __glox__
#define __glox__

#include<iostream>
using namespace std;

namespace glox{
	namespace clk{
		int dtms=10;
		float dt=dtms/1000.f;
	}
	namespace metrics{
		int nglobs=0;
		int bvolchecksphcol=0;
		int frame=0;
	}
	inline float dt(const float f){return f*clk::dt;}
	inline float rnd(const float from,const float tonotincluding){
		return from+(tonotincluding-from)*rand()/RAND_MAX;
	}
}
using namespace glox;

#include<math.h>

class p3{
	float x,y,z;
public:
	inline p3():x(0),y(0),z(0){}
	inline p3(const float x,const float y,const float z):x(x),y(y),z(z){}
	inline p3(const p3&from,const p3&to):x(to.x-from.x),y(to.y-from.y),z(to.z-from.z){}
	inline const float getx()const{return x;}
	inline const float gety()const{return y;}
	inline const float getz()const{return z;}
	inline p3&transl(const float dx,const float dy,const float dz){x+=dx;y+=dy;z+=dz;return*this;}
	inline const float magn()const{return sqrt(x*x+y*y+z*z);}
	inline p3&set(const p3&p){x=p.x;y=p.y;z=p.z;return*this;}
	inline p3&set(const float x,const float y,const float z){this->x=x;this->y=y;this->z=z;return*this;}
	friend ostream&operator<<(ostream&,const p3&);
	friend istream&operator>>(istream&,p3&);
};
inline ostream&operator<<(ostream&os,const p3&p){os<<p.x<<","<<p.y<<","<<p.z;return os;}
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
	inline const int num()const{return i;}
	inline const char* str()const{return s;}
};

template<class T>class arai{
private:
	T*ar;
	int of,ln;
	inline void asrt(const int offset,const int len)const{if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofboundsinrw");}
public:
	inline arai(T a[],const int offset,const int len):ar(a),of(offset),ln(len){}
	inline T&operator[](const int i)const{asrt(i,1);return ar[of+i];}
	inline int ofs()const{return of;}
	inline int len()const{return ln;}
	inline void ro(const int offset,const int len,void(*f)(const T&e))const{if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofboundsinro");T*p=ar+of+offset;int i=len;while(i--)(*f)(*p++);}
	inline void rw(const int offset,const int len,void(*f)(T&e)){asrt(offset,len);T*p=ar+of+offset;int i=len;while(i--)(*f)(*p++);}
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

//#define mhello()cout<<"hello"<<endl;
#define flf()l("  ",__FILE__,__LINE__,__FUNCTION__);
static inline ostream&l(const char*s="",const char*file="",int lineno=0,const char*func=""){cerr<<file;if(lineno){cerr<<":"<<lineno;}cerr<<" "<<func<<"  "<<s;return cerr;}
static inline ostream&ll(const char*s="",const char*file="",int lineno=0,const char*func=""){return l(s,file,lineno,func)<<endl;}

class m3{
	float xx,xy,xz,xo;
	float yx,yy,yz,yo;
	float zx,zy,zz,zo;
public:
	m3(){}
	m3&ident(){xx=1;xy=0;xz=0;xo=0; yx=0;yy=1;yz=0;yo=0; zx=0;zy=0;zz=1;zo=0; return*this;}
	const m3&axisx(p3&p)const{p.set(xx,xy,xz);return*this;}
	const m3&axisy(p3&p)const{p.set(yx,yy,yz);return*this;}
	const m3&axisz(p3&p)const{p.set(zx,zy,zz);return*this;}
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
	void mult(const p3&src,p3&dst)const{
		const float x=src.getx();
		const float y=src.gety();
		const float z=src.getz();
		float rx=x*xx+y*xy+z*xz+xo;
		float ry=x*yx+y*yy+z*yz+yo;
		float rz=x*zx+y*zy+z*zz+zo;
		dst.set(rx,ry,rz);
	}
	friend ostream&operator<<(ostream&,const m3&);
	friend istream&operator>>(istream&,m3&);
};
ostream&operator<<(ostream&os,const m3&m){
	cout<<"["<<p3(m.xx,m.xy,m.xz)<<","<<m.xo<<"],[";
	cout<<p3(m.yx,m.yy,m.yz)<<","<<m.yo<<"],[";
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
	static bool checkcol(const p3&p1,const m3&m1,const bvol&bv1,const p3&p2,const m3&m2,const bvol&bv2){
		if(!spherescollide(p1,bv1,p2,bv2)){
//			flf();ll(" · nosphereoverlap ");
			return false;
		}
		flf();ll(" • sphereoverlap");
		flf();l()<<"a p3("<<p1<<")m3("<<m1<<")"<<"bvol("<<bv1<<")    b p3("<<p2<<")m3("<<m2<<")bvol("<<bv2<<")"<<endl;
		return true;
	}
	static bool spherescollide(const p3&pa,const bvol&a,const p3&pb,const bvol&b){
		metrics::bvolchecksphcol++;
		const p3 vec=p3(pa,pb);
		const float dst=vec.magn();
		if(dst>(a.r+b.r))
			return false;
		return true;
	}
	static bool possibleoverlap(const p3&pa,const bvol&bva,const p3&pb,const bvol&bvb){
		flf();l()<<pa<<bva<<pb<<bvb<<endl;
		return false;
	}
	///
	bvol(const float sphereradius,const p3&boxcorner):r(sphereradius),v(boxcorner){}
	bool anyboxdotinboxof(const p3&p,const m3&m,const bvol&bv){
		flf();l()<<p<<m<<bv<<endl;
		return false;
	}
	friend ostream&operator<<(ostream&,const bvol&);
	friend istream&operator>>(istream&,bvol&);
};
ostream&operator<<(ostream&os,const bvol&b){os<<b.r<<",("<<b.v<<")";return os;}
istream&operator>>(istream&is,bvol&bv){is>>bv.r;is.ignore(2);is>>bv.v;is.ignore();return is;}

#include<vector>

class glob:public p3{
protected:
	glob&g;
	p3 a;
	vector<glob*>chs;
public:
	bvol bv;
	m3 mw;
	static bool drawboundingspheres;
	static int drawboundingspheresdetail;
	glob(glob&g,const p3&p=p3(),const p3&a=p3(),const float r=0,const p3&pbox=p3()):p3(p),g(g),a(a),bv(r,pbox){
		metrics::nglobs++;
		if(&g==NULL)
			return;
		g.chs.push_back(this);
	}
	virtual ~glob(){
		metrics::nglobs--;
		for(unsigned int n=0;n<chs.size();n++)
			delete chs[n];
		chs.clear();
//		cout<<metrics::nglobs<<endl;
	}
	inline void rm(){throw "notimpl";}
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
		for(unsigned int n=0;n<chs.size();n++){
			glPushMatrix();
			chs[n]->draw();
			glPopMatrix();
		}
	}
	virtual void gldraw(){};
	virtual void tick(){for(size_t i=0;i<chs.size();i++)chs[i]->tick();}
	inline glob&getglob()const{return g;}
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
	obcorpqb(glob&pt,const float r=1):glob(pt),r(r),a(.25*n++),drscl(.5){}
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
		bv.r=s+1.4;
		const float ds=.1*s;
		const float dz=.5*s;
//		for(float zz=-s;zz<s;zz+=ds)
		for(float zz=-s;zz<=s;zz+=dz)
			for(float xx=-s;xx<=s;xx+=ds)
				for(float yy=-s;yy<=s;yy+=ds){
					if(sqrt(xx*xx+yy*yy+zz*zz)>s)
						continue;
					glob*o=new obcorpqb(*this);
					o->transl(xx,yy,zz);
					o->agl().transl(90,0,0);
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
//		flf();cout<<f<<endl;
		glMaterialfv(GL_FRONT,GL_SHININESS,matshin);
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

class wold:public glob{
	static wold wd;
	float s;
public:
	inline static wold&get(){return wd;}
	float ddegx,ddegz;
	wold():glob(*(glob*)NULL),s(15),ddegx(0),ddegz(.1f){
//		agl().transl(90.1,0,0);
//		transl(0,-.3,0);
		bv.r=s;
		new obcorp(*this,p3(0,0,4.2),p3(90,0,0));
	}
//	~wold(){}
	void gldraw(){
		glDisable(GL_LIGHTING);

		glColor3b(0,0,0x7f);
		//glutWireCube(s);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-s,-s);
		glVertex2f( s,-s);
		glVertex2f( s, s);
		glVertex2f(-s, s);
		glVertex2f(-s,-s);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3b(127,127,127);
		glVertex3f(s,0,0);
		glVertex3f(0,0,0);
		glColor3b(127,127,127);
		glColor3b(0,0,0);
		glVertex3f(0,s,0);
		glVertex3f(0,0,0);
		glColor3b(0,0,0);
		glColor3b(0,0,127);
		glVertex3f(0,0,s);
		glVertex3f(0,0,0);
		glColor3b(0,0,127);
		glEnd();

		const float r=s;
		glPushMatrix();
//		glTranslatef(0,0,01f);
		glColor3b(0,0x7f,0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0,0);
		glVertex2f(r,0);
		const float dtr=3.14159/180;
		const int di=360/24;
		for(int i=di;i<=360;i+=di){
			const float rd=i*dtr;
			glVertex2f(r*cos(rd),r*sin(rd));
		}
		glEnd();
		glPopMatrix();

		glEnable(GL_LIGHTING);
	}
	void tick(){
		agl().transl(-dt(ddegx),0,dt(ddegz));
		const int n=chs.size();
		const int nn=n-1;
		for(int i=0;i<nn;i++){
			glob&g1=*chs[i];
			for(int k=i+1;k<n;k++){
				glob&g2=*chs[k];
				if(bvol::checkcol(g1,g1.mw,g1.bv, g2,g2.mw,g2.bv)){
					flf();ll(" • sphereoverlap ")<<typeid(g1).name()<<" "<<typeid(g2).name()<<endl;

//					cout<<"collisioxx:"<<endl<<g1<<endl<<g2<<endl;
				}
			}
		}
		glob::tick();
	}
};
wold wold::wd;


template<class T>class lut{
private:
	int size;
	class el{
	public:
		const char*key;
		T data;
		el*nxt;
		el(const char*key,T data):key(key),data(data),nxt(NULL){}
		~el(){if(nxt)delete nxt;}
	};
	el**array;
	static int hash(const char*key,const int roll){
		unsigned int i=0;
		const char*p=key;
		while(*p)i+=*p++;
		i%=roll;
		return i;
	}
public:
	lut(const int size=8):size(size){array=(el**)calloc(size,sizeof(el*));}
	~lut(){clear();delete array;}
	T operator[](const char*key)const{
		const int h=hash(key,size);
		el*l=array[h];
		if(!l)
			return (T)NULL;
		while(1){
			if(!strcmp(l->key,key))return l->data;
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
		for(int i=0;i<size;i++){
			el*e=array[i];
			if(!e)
				continue;
			delete e;
			array[i]=NULL;
		}
	}
};

#include<sys/time.h>

class windo:public glob{
public:
	int w,h;
	windo(const p3&p):glob(wold::get()){set(p);bv.r=2;}
	void pl(const char*text,const GLfloat y=0,const GLfloat x=0,const GLfloat linewidth=1,const float scale=1){
		char*cp=(char*)text;
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
		const int dy=h>>3;
		int y=dy>>2;

		pl("glox",y,0,1,.2f);

		timeval tv;gettimeofday(&tv,0);
		const tm&t=*localtime(&tv.tv_sec);//? leak, delrefatblokxit
		char ac[256];
		sprintf(ac,"%02d:%02d:%02d.%03d   frame(%d) globs(%d) coldet(%d,%d) keys(j f e d g h i k ur nv 1) p(%0.0f %0.0f %0.0f) a(%0.0f %0.0f %0.0f)",t.tm_hour,t.tm_min,t.tm_sec,tv.tv_usec/1000,metrics::frame,metrics::nglobs,metrics::bvolchecksphcol,0,getx(),gety(),getz(),agl().getx(),agl().gety(),agl().getz());//? ostream
		y-=dy>>2;pl(ac,y,w>>5,1,.1f);
	}
	void drawframe(){
		cout<<"\rframe("<<metrics::frame++<<") ";
//		glClearColor(0,0,0,1);
		glClearColor(.5f,.5f,1,1);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
		glDisable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
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
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		const GLfloat lhtpos[]={0,20,10000,1};
		glLightfv(GL_LIGHT0,GL_POSITION,lhtpos);
		const GLfloat lhtcol[]={1,1,1,1};
		glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,lhtcol);
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
	int w=512,h=512;
	int __w=w,__h=h;
	lut<int>keysdn;
	bool gamemode=false;
	bool fullscr=false;
	windo&wn=*new windo(p3(0,0,15));
	void reshape(const int width,const int height){
		cout<<" reshape: "<<w<<"x"<<h<<endl;
		w=width;h=height;
	}
	void draw(){
		wn.w=w;wn.h=h;
		wn.drawframe();
		metrics::bvolchecksphcol=0;
		glutSwapBuffers();
	}
	void timer(const int value){
		const char r[]={'r',0};const char u[]={'u',0};
		if(keysdn[r]&&keysdn[u]){wn.transl(0,dt(1),0);}
		const char v[]={'v',0};const char n[]={'n',0};
		if(keysdn[v]&&keysdn[n]){wn.transl(0,-dt(1),0);}

		wold::get().tick();

		glutPostRedisplay();
		glutTimerFunc(value,timer,value);
	}
	bool iskeydn(const char key,bool setifnot=0){
		static char k[]={0,0};
		k[0]=key;
		const bool b=keysdn[k]==1;
		if(!b&&setifnot)
			keysdn.put(strcpy(new char[2],k),1);//? bug leak
		return b;
	}
	void keydn(const unsigned char key,const int x,const int y){
		if(iskeydn(key,true))return;
		cout<<"   keydn("<<(int)key<<",["<<x<<","<<y<<"])"<<key<<endl;
		if(key=='~'){
			fullscr=!fullscr;
			if(fullscr){
				__w=w;__h=h;
				glutFullScreen();
			}else
				glutReshapeWindow(__w,__h);
			return;
		}

		wold&wd=wold::get();
		if(key==0){throw "keyo";}
		else if(key=='j'){wd.ddegz-=360/60;}
		else if(key=='f'){wd.ddegz+=360/60;}
		else if(key=='e'){wd.ddegx-=360/60;}
		else if(key=='d'){wd.ddegx+=360/60;}
		else if(key==' '){wd.ddegz=wd.ddegx=0;}
		else if(key=='g'){wd.ddegz=wd.ddegx=0;wd.agl().set(p3(270,0,0));}
		else if(key=='h'){wd.ddegz=wd.ddegx=0;wd.agl().set(p3(90.5,0,0));}
		else if(key=='i'){wn.transl(0,0,-1);}
		else if(key=='k'){wn.transl(0,0,1);}
		else if(key=='1'){glob::drawboundingspheres=!glob::drawboundingspheres;}
	}
	void keyup(const unsigned char key,const int x,const int y){
		const char k[]={key,0};
		keysdn.rm(k);//? whatif 1 and not handled
		cout<<"   keyup("<<(int)key<<",["<<x<<","<<y<<"])"<<key<<endl;
		if(key==27)// esc
		{glutReshapeWindow(w,h);exit(0);}
	}
	void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<"@"<<x<<","<<y<<endl;}
	//void idle(){
	//	printf("idle\n");
	//	return;
	//}
	void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
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
			if(fullscr)
				glutFullScreen();
		}
		glutDisplayFunc(draw);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keydn);
		glutKeyboardUpFunc(keyup);
		glutMouseFunc(mouseclk);
		glutMotionFunc(mousemov);
		glutTimerFunc(0,timer,glox::clk::dtms);
//		glutIdleFunc(idle);
//		glutReportErrors();

		glutMainLoop();
		return 0;
	}
}

int main(){return glut::main(0,NULL);}

extern void gnox(){}
#endif
