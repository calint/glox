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
	p3():x(0),y(0),z(0){}
	p3(const float x,const float y,const float z):x(x),y(y),z(z){}
	p3(const p3&from,const p3&to):x(to.x-from.x),y(to.y-from.y),z(to.z-from.z){}
	inline const float getx()const{return x;}
	inline const float gety()const{return y;}
	inline const float getz()const{return z;}
	inline p3&transl(const float dx,const float dy,const float dz){x+=dx;y+=dy;z+=dz;return*this;}
	inline const float magn()const{return sqrt(x*x+y*y+z*z);}
	inline p3&set(const p3&p){x=p.x;y=p.y;z=p.z;return*this;}

	friend ostream&operator<<(ostream&,const p3&);
	friend istream&operator>>(istream&,p3&);
};
ostream&operator<<(ostream&os,const p3&p){os<<p.x<<","<<p.y<<","<<p.z;return os;}
istream&operator>>(istream&is,p3&p){is>>p.x;is.ignore();is>>p.y;is.ignore();is>>p.z;return is;}

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
public:
	float c[16];
	m3(){}
	m3(const m3&m){if(&m==0){flf();};}
};
ostream&operator<<(ostream&os,const m3&m){m3(m).c[0]=1;return os;}
istream&operator>>(istream&is,m3&m){m3(m).c[0]=1;return is;}

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
			flf();ll(" · nosphereoverlap");
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
	bvol(const float sphereradius,const p3 boxcorner):r(sphereradius),v(boxcorner){}
	bool anyboxdotinboxof(const p3&p,const m3&m,const bvol&bv){
		flf();l()<<p<<m<<bv<<endl;
		return false;
	}
	friend ostream&operator<<(ostream&,const bvol&);
	friend istream&operator>>(istream&,bvol&);
};
ostream&operator<<(ostream&os,const bvol&b){
	os<<b.r<<",("<<b.v<<")";
	return os;
}
istream&operator>>(istream&is,bvol&bv){
	is>>bv.r;is.ignore(2);
	is>>bv.v;is.ignore();
	return is;
}
///////////////////////////////////////////////////////////////////////////////
extern void gnox(){
	//	flf();ll();
	//	bvol::checkcol(p3(),m3(),bvol(1,p3(1,1,1)),p3(1,0,0),m3(),bvol(1,p3(1,1,1)));
	//	throw signl(1,"gnoxstop");
}
///////////////////////////////////////////////////////////////////////////////

#include<vector>

class glob:public p3{
protected:
	glob&g;
	p3 a;
	vector<glob*>chs;
public:
	static bool drawboundingspheres;
	static int drawboundingspheresdetail;
	bvol bv;
	m3 mw;
	glob(glob&g):p3(),g(g),a(),bv(0,p3()){
		if(&g==this)
			return;
		g.chs.push_back(this);
		metrics::nglobs++;
	}
	virtual ~glob(){
//		cout<<"\r  globs: "<<metrics::nglobs;
		metrics::nglobs--;
		for(unsigned int n=0;n<chs.size();n++)
			delete chs[n];
	}
	inline p3&agl(){return a;}
	inline glob&seta(const p3&a){this->a.set(a);return*this;}
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

class obteapot:public glob{
	float dy;
public:
	obteapot(glob&pt):glob(pt),dy(.1f){}
	void gldraw(){
		glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glutSolidTeapot(1);
		glPopAttrib();
	}
	virtual void tick(){
		glob::tick();
		transl(0,dt(dy),0);
		if(gety()>10||gety()<0)dy=-dy;
	}
};

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
	obcorp(glob&pt):glob(pt){
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
		ff+=dt(1/10);
		if(ff>1)ff=0;
		glMaterialfv(GL_FRONT,GL_SPECULAR,matspec);
		GLfloat matshin[]={f};
//		f+=dt(30);
		if(f>128)f=0;
		flf();cout<<f<<endl;
		glMaterialfv(GL_FRONT,GL_SHININESS,matshin);
	}
	virtual void tick(){
//		transl(dt(s*.01),0,0);
		glob::tick();
	}
};
const float obcorp::s=7;


class obquad:public glob{
public:
	obquad(glob&g):glob(g){
		glob*o=new obcorp(*this);
		o->agl().transl(-90,0,0);
	}
	void gldraw(){
		const float s=15.f;

		glColor3b(0,0,0x7f);
		glBegin(GL_QUADS);
		glVertex2f(-s,-s);
		glVertex2f( s,-s);
		glVertex2f( s, s);
		glVertex2f(-s, s);
		glEnd();

		glColor3b(0,0,127);
		glBegin(GL_LINE_STRIP);
		glVertex3f(s,0,.01f);
		glVertex3f(0,0,.01f);
		glColor3b(127,127,127);
		glVertex3f(0,s,.02f);
		glEnd();

		const float r=.75*s;
		glPushMatrix();
		glTranslatef(0,0,.01f);
		glColor3b(0,0x7f,0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0,0);
		glVertex2f(r,0);
		const float dtr=3.14159/180;
		const int di=360/12;
		for(int i=di;i<=360;i+=di){
			const float rd=i*dtr;
			glVertex2f(r*cos(rd),r*sin(rd));
		}
		glEnd();
		glPopMatrix();
	}
	virtual void tick(){
		agl().transl(dt(360/60),0,0);
		glob::tick();
	}
};


class obwom:public glob{
public:
	obwom(glob&pt,const int links):glob(pt){
		if(links==0)
			return;
		glob*o=new obwom(*this,links-1);
		o->transl(0,.4f,0);
	}
	void gldraw(){
		glPushAttrib(GL_ENABLE_BIT);
		glShadeModel(GL_SMOOTH);
//		glShadeModel(GL_FLAT);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glColor3b(0,0x20,0x60);
		const float dr=rnd(0,.1f);
//		const float dr=0;
		const float r=.4;
		glutSolidSphere(r+dr,6,6);
//		glutSolidCube(r+dr);
		glPopAttrib();
	}
	virtual void tick(){
		glob::tick();
		agl().transl(dt(60),0,dt(60));
//		a.transl(0,d(60),d(60));
//		a.transl(d(60),0,0);
//		a.transl(d(60),d(60),0);
	}
};


class wold:public glob{
	static wold wd;
public:
	inline static wold&get(){return wd;}
	float ddegx,ddegz;

	wold():glob(*this),ddegx(0),ddegz(.1f){
		agl().transl(90.1,0,0);
		transl(0,-.3,0);
		glob&g=*new obcorp(*this);
		g.agl().transl(90,0,0);
		g.transl(0,0,4.2f);
	}
	~wold(){ll();}
	void gldraw(){
		const float s=15.f;
		glDisable(GL_LIGHTING);

		glColor3b(0,0,0x7f);
		glBegin(GL_QUADS);
		glVertex2f(-s,-s);
		glVertex2f( s,-s);
		glVertex2f( s, s);
		glVertex2f(-s, s);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3f(s,0,.01f);
		glColor3b(0,127,127);
		glVertex3f(0,0,.01f);
		glColor3b(0,0,127);
		glVertex3f(0,s,.02f);
		glEnd();

		const float r=s;
		glPushMatrix();
		glTranslatef(0,0,.01f);
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
	int w,h,frame;
	windo(glob&g,const p3&p):glob(g){set(p);}
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

		pl("glox",y,0,1,.5f);

		timeval tv;gettimeofday(&tv,0);
		const tm&t=*localtime(&tv.tv_sec);//? leak, delrefatblokxit
		char ac[256];
		sprintf(ac,"%02d:%02d:%02d.%03d   frame(%d) globs(%d) coldet(%d,%d) keys(j f e d g h i k ur nv 1) p(%0.0f %0.0f %0.0f) a(%0.0f %0.0f %0.0f)",t.tm_hour,t.tm_min,t.tm_sec,tv.tv_usec/1000,metrics::frame,metrics::nglobs,metrics::bvolchecksphcol,0,getx(),gety(),getz(),agl().getx(),agl().gety(),agl().getz());//? ostream
		y-=dy>>2;pl(ac,y,w>>5,1,.1f);
	}
	void drawframe(){
		cout<<"\rframe("<<frame++<<") ";
//		glClearColor(0,0,0,1);
		glClearColor(.5f,.5f,1,1);
		glClearDepth(1);

		glEnable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
		glDisable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,(GLdouble)w/h,.01,10000);
//		gluLookAt(getx(),gety(),getz(), 0,0,0, 0,1,0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
//		glRotatef(-a.getz(), 0, 0, 1);
//		glRotatef(-a.gety(), 0, 1, 0);
//		glRotatef(-a.getx(), 1, 0, 0);
		glTranslatef(-getx(), -gety(), -getz());

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		GLfloat lhtpos[]={0,20,10000,1};
		glLightfv(GL_LIGHT0,GL_POSITION,lhtpos);
		GLfloat lhtcol[]={1,1,1,1};
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
	}
};

namespace glut{
	int w=512,h=512;
	lut<int>lutkeys;
	bool gamemode=false;
	bool fullscr=false;
	windo&wn=*new windo(wold::get(),*new p3(0,0,15));
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
		if(lutkeys[r]&&lutkeys[u]){wn.transl(0,dt(1),0);}
		const char v[]={'v',0};const char n[]={'n',0};
		if(lutkeys[v]&&lutkeys[n]){wn.transl(0,-dt(1),0);}

		wold::get().tick();

		glutPostRedisplay();
		glutTimerFunc(value,timer,value);
	}
	void keydn(const unsigned char key,const int x,const int y){
		char*ks=new char[2];
		char s[]={key,0};
		strncpy(ks,s,2);//? bug leak
		if(lutkeys[ks]==1){//? impl array[]
			return;
		}
		lutkeys.put(ks,1);
		cout<<"  keydn("<<(int)key<<",["<<x<<","<<y<<"])"<<key<<endl;
		if(key=='~'){
			fullscr=!fullscr;
			if(fullscr)
				glutFullScreen();
			else
				glutReshapeWindow(w,h);
			return;
		}

//		if(key==32){glob*o=new obwom(wld,14);o->transl(0,0,-10);}
		wold&wd=wold::get();//? wn.getglob();
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
		char*ks=new char[2];
		char s[]={key,0};
		strncpy(ks,s,2);//? bug leak
		lutkeys.put(ks,0);//? if 1 and not handled
		cout<<"   keyup("<<(int)key<<",["<<x<<","<<y<<"])"<<key<<endl;
		if(key==27)// esc
			exit(0);
	}
	void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<"@"<<x<<","<<y<<endl;}
	//void idle(){
	//	printf("idle\n");
	//	return;
	//}
	void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
	int main(int argc,char**argv){
		cout<<"glox ";
		glutInit(&argc,argv);
//		glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
//		glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
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
//		cout<<glGetString(GL_VENDOR)<<"  "<<glGetString(GL_VERSION)<<"  "<<glGetString(GL_VERSION)<<"  glsl "<<GL_SHADING_LANGUAGE_VERSION<<endl;
//		cout<<"opengl("<<glGetString(GL_VERSION)<<")"<<" glsl("<<GL_SHADING_LANGUAGE_VERSION<<")"<<endl;
		cout<<"opengl ";
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

		glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

		return 0;
	}
}

extern void gnox();
static void mainsig(const int i){cerr<<" ••• terminated with signal "<<i<<endl;exit(i);}
int main(){
	for(int i=0;i<32;i++)signal(i,mainsig);//?
	srand(0);
	gnox();
	return glut::main(0,NULL);
}
#endif




