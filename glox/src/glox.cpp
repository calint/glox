#ifndef __glox__
#define __glox__


namespace glox{
	const char*name="glox";
	const static int dtms=100;
	const static float dt=glox::dtms/1000.f;
	inline float d(const float f){return f*glox::dt;}
}
using namespace glox;

#include<iostream>
using namespace std;
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
	friend ostream&operator<<(ostream&os,const p3&a);
};
ostream&operator<<(ostream&os,const p3&a){
	os<<a.x<<","<<a.y<<","<<a.z;
	return os;
}

#include<execinfo.h>
class signl{
	const int i;
	const char*s;
public:
	signl(const int i,const char*s):i(i),s(s){
		cout<<" ••• signal "<<i<<" · "<<s<<endl;
		const int na=10;
		void*va[na];
		const size_t n=backtrace(va,na);
		backtrace_symbols_fd(va,n,2);
	}
	inline const int num()const{return i;}
	inline const char* str()const{return s;}
};

template<class T>class array{
private:
	T*a;
	int of;
	int ln;
public:
	inline array(T ae[],const int offset,const int len):a(ae),of(offset),ln(len){}
	inline T&operator[](const int i)const{
		if(i<0||i>=ln)throw signl(1,"indexoutofbounds");
		return a[of+i];
	}
	inline void ro(const int offset,const int len,void(*f)(const T&e)){
		if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofboundsinro");
		T*p=a+of+offset;
		int i=len;
		while(i--)
			(*f)(*p++);
	}
	inline void rw(const int offset,const int len,void(*f)(T&e)){
		if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofboundsinrw");
		T*p=a+of+offset;
		int i=len;
		while(i--)
			(*f)(*p++);
	}
	inline int len()const{return ln;}
};

#include<vector>

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#include <glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

class m3{};
class volume{
	float r;
	p3 v;
public:
	static bool checkcol(const p3&pa,const volume&va,const p3&pb,const volume&vb){
		cout<<"a(r,p3)=("<<va<<")"<<endl;
		cout<<"b(r,p3)=("<<vb<<")"<<endl;
		if(!spherescollide(pa,va,pb,vb)){
			cout<<" · no sphere overlap"<<endl;
			return false;
		}
		cout<<" * sphere overlap"<<endl;
		return true;
	}
	static bool spherescollide(const p3&pa,const volume&a,const p3&pb,const volume&b){
		const p3 vec=p3(pa,pb);
		const float dst=vec.magn();
		if(dst>(a.r+b.r))
			return false;
		return true;
	}
	static bool possibleoverlap(const volume a,const volume b){
		cout<<a<<" "<<b<<endl;
		return false;
	}
	volume(const float sphereradius,const p3 boxcorner):r(sphereradius),v(boxcorner){}
	bool anyboxdotinboxof(const volume a){
		cout<<a<<endl;
		return false;
	}
    friend ostream&operator<<(ostream&os,const volume&a);
};
ostream&operator<<(ostream&os,const volume&a){
	os<<a.r<<",["<<a.v<<"]";
    return os;
}


class object:public p3{
protected:
	object&pt;
	p3 a;
	vector<object*>chs;
public:
	object(object&parent):p3(),pt(parent),a(){
		if(&pt==this)
			return;
		pt.chs.push_back(this);
	}
	virtual ~object(){}
//	inline const p3&agl()const{return a;}
	inline p3&agl(){return a;}
	void draw(){
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1,0,0);
		glRotatef(a.gety(),0,1,0);
		glRotatef(a.getz(),0,0,1);
		gldraw();
		for(unsigned int n=0;n<chs.size();n++){
			glPushMatrix();
			chs[n]->draw();
			glPopMatrix();
		}
	}
	virtual void gldraw(){};
	inline object&rot(const p3&agl){a=agl;return*this;}
	virtual void tick(){
		for(unsigned int i=0;i<chs.size();i++){
			chs[i]->tick();
		}
	}
};


class obteapot:public object{
	float dy;
public:
	obteapot(object&pt):object(pt),dy(.1f){}
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
		object::tick();
		transl(0,d(dy),0);
		if(gety()>10||gety()<0)dy=-dy;
	}
};

class obgridot:public object{
	float r;
	static int n;
	float a;
public:
	obgridot(object&pt,const float r=1):object(pt),r(r){
//		a=.5f*n++*rand()/RAND_MAX;
		a=.25*n++;
	}
	void gldraw(){
		glPushAttrib(GL_ENABLE_BIT);
//		if(((float)rand()/RAND_MAX)>.001)
			glShadeModel(GL_FLAT);
//		else
//			glShadeModel(GL_SMOOTH);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
//		glFrontFace(GL_CW);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glColor4f(0,0,1.f,1.f);
//		const float dr=rng*rand()/RAND_MAX-rng/2;
		const float dr=.5*sin(a);
		glutSolidSphere(r+dr,4,3);
//		glutSolidTetrahedron();
//		glutSolidCube(r+dr);
//		glLineWidth(100);
//		glutWireCube(r+dr);
//		glutWireSphere(r+dr,5,5);
		glPopAttrib();
	}
	virtual void tick(){
//		const float s=1;
//		const float dx=s*((float)rand()/RAND_MAX-.5f);
//		const float dy=0;//s*((float)rand()/RAND_MAX-.5f);
//		const float dz=s*((float)rand()/RAND_MAX-.5f);
//		transl(d(dx),d(dy),d(dz));
//		a+=d(.01*360/60);
	}
};
int obgridot::n=0;

#include<math.h>

class obgrid:public object{
public:
	obgrid(object&pt):object(pt){
		const float s=7;
		const float ds=s/10;
		for(float xx=-s;xx<s;xx+=ds)
			for(float yy=-s;yy<s;yy+=ds){
				if(sqrt(xx*xx+yy*yy)>s)
					continue;
				object*o=new obgridot(*this);
				o->transl(xx,yy,0);
				o->agl().transl(90,0,0);
			}
	}
	virtual void tick(){
		object::tick();
		a.transl(d(360/60),0,0);
	}
};


class obworm:public object{
public:
	obworm(object&pt,const int links):object(pt){
		if(links==0)
			return;
		object*o=new obworm(*this,links-1);
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
		const float dr=.1f*rand()/RAND_MAX;
//		const float dr=0;
		const float r=.4;
		glutSolidSphere(r+dr,6,6);
//		glutSolidCube(r+dr);
		glPopAttrib();
	}
	virtual void tick(){
		object::tick();
		a.transl(d(60),0,d(60));
//		a.transl(0,d(60),d(60));
//		a.transl(d(60),0,0);
//		a.transl(d(60),d(60),0);
	}
};

class world:public object{
public:
	world():object(*this){
		const bool worms=false;
		const bool grid=true;
		if(worms){
			object*o1=new obworm(*this,1);
			o1->transl(2,2,-10);

			object*o2=new obworm(*this,3);
			o2->transl(-2,2,-10);

			object*o3=new obworm(*this,16);
	//		o3->transl(-1.9,.6,-10);
			o3->transl(0,2,-10);
		}
		if(grid){
			object*o4=new obgrid(*this);
			o4->transl(0,-1,-10);
			o4->agl().transl(-68,0,0);
		}
//
//		for(int i=-3;i<3;i++){
//			teapot*o=new teapot(*this);
//			o->rot(p3(i*90,0,0));
//			o->transl(2.f*i,0.f,-10.f);
//			chs.push_back(o);
//		}
	}
//	void gldraw(){}
	virtual void tick(){
//		cout<<"    tick: "<<endl;
		object::tick();
	}
};

template<class T>class lut{
private:
        int size;
        class el{
        public:
                const char*key;
                T data;
                el*nxt;
                el(const char*key,T data):key(key),data(data),nxt(NULL){}
                ~el(){
                        if(nxt)
                                delete nxt;
                }
        };
        el**array;
public:
        static unsigned int hash(const char*key,const unsigned int roll){
                unsigned int i=0;
                const char*p=key;
                while(*p)
                        i+=*p++;
                i%=roll;
                return i;
        }
        lut(const int size=8):size(size){
                array=(el**)calloc(size,sizeof(el*));
        }
        ~lut(){
                clear();
                delete array;
        }
        T operator[](const char*key)const{
                const int h=hash(key,size);
                el*l=array[h];
                if(!l)
                        return NULL;
                while(1){
                        if(!strcmp(l->key,key)){
                                return l->data;
                        }
                        if(l->nxt){
                                l=l->nxt;
                                continue;
                        }
                        return NULL;
                }
                return NULL;//?
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


class window{
public:
	static world&wld;
	static int w;
	static int h;
	static p3 p;
	static p3 a;
	static lut<int>&lutkeys;
	static void reshape(const int width,const int height){
		cout<<" reshape: "<<w<<"x"<<h<<endl;
		w=width;h=height;
	}
	static void draw(){
		static int frame=0;
		cout<<"\rframe(#)="<<frame++<<" "<<flush;
//		glClearColor(0,0,0,1);
		glClearColor(.5f,.5f,1,1);
		glClearDepth(1);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45,(GLdouble)w / h, .1, 1000);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-p.getx(), -p.gety(), -p.getz());
		glRotatef(-a.getz(), 0, 0, 1);
		glRotatef(-a.getx(), 1, 0, 0);
		glRotatef(-a.gety(), 0, 1, 0);

		wld.draw();
		drwhud();
		glutSwapBuffers();
	}
    static void pl(const GLfloat x,const GLfloat y,const char*text,const GLfloat linewidth=1){
      char*cp=(char*)text;
      glPushMatrix();
      glTranslatef(x,y,0);
      glLineWidth(linewidth);
      for(;*cp;cp++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN,*cp);
//      glutStrokeString(GLUT_STROKE_MONO_ROMAN,text);
      glPopMatrix();
    }
	static void drwhud(){
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0,512,0,512,0,1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glColor3f(1,1,1);
		glEnable(GL_BLEND);
		pl(0,h>>1,"glox",(float)(h>>7));
		glDisable(GL_BLEND);
	}
	static void keybd(const unsigned char key,const int x,const int y){
		char*ks=new char[2];
		char s[]={key,0};
		strncpy(ks,s,2);//? bug leak
		if(lutkeys[ks]==1){
			return;
		}
		lutkeys.put(ks,1);
		cout<<" keydown: "<<key<<" "<<(int)key<<"@"<<x<<","<<y<<endl;
		if(key==96)// `
			glutFullScreen();
		else if(key==126)// ~
			glutReshapeWindow(w,h);
		if(key==32)// spc
			{object*o=new obworm(wld,7);o->transl(0,3,-10);}
	}
	static void keybu(const unsigned char key,const int x,const int y){
		char*ks=new char[2];
		char s[]={key,0};
		strncpy(ks,s,2);//? bug leak
//		cout<<ks<<endl;
		lutkeys.put(ks,0);//? if 1 and not handled
//		cout<<__LINE__<<":: "<<(void*)&lutkeys<<" "<<lutkeys[ks]<<endl;
		cout<<"   keyup: "<<key<<" "<<(int)key<<"@"<<x<<","<<y<<endl;
		if(key==27)// esc
			exit(0);
	}
	static void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<"@"<<x<<","<<y<<endl;}
	static void timer(const int value){
//		cout<<"\rtimer: "<<value;
		wld.tick();
		glutPostRedisplay();
//		glutTimerFunc(value,timer,value-1);
		glutTimerFunc(value,timer,value);
	}
	//static void idle(){
	//	printf("idle\n");
	//	return;
	//}
	static void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
	static int main(int argc,char**argv){
		printf("glox ");
		glutInit(&argc,argv);
		glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
//		glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
		glutIgnoreKeyRepeat(true);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);

//		glutGameModeString("1024x768:32");
//		glutEnterGameMode();

		glutInitWindowSize(w,h);
		glutCreateWindow("glox");

//		glutFullScreen();

		glutDisplayFunc(draw);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keybd);
		glutKeyboardUpFunc(keybu);
		glutMouseFunc(mouseclk);
		glutMotionFunc(mousemov);
		glutTimerFunc(0,timer,glox::dtms);
//		glutIdleFunc(idle);
//		glutReportErrors();

		glutMainLoop();

		glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

		return 0;
	}
};
world&window::wld=*new world();
int window::w=512;
int window::h=512;
p3 window::p=p3(0,0,-.5);
p3 window::a=p3();
lut<int>&window::lutkeys=*new lut<int>();

static void main_sigf(const int a){
	cout<<" ••• terminated with signal "<<a<<endl;
	exit(a);
}
int main(){
	for(int i=0;i<32;i++)//?
		signal(i,main_sigf);

	volume a=volume(1,p3(1,1,1));
	volume b=volume(2,p3(2,1,1));
//	bool col=volume::checkcol(p3(),a,p3(),b);
//	cout<<"collision: "<<col<<endl;
	cout<<"collision: "<<volume::checkcol(p3(3,0,0),a,p3(),b)<<endl;



	return window::main(0,NULL);
}


class xser{
private:
	FILE*in;
	FILE*out;
public:
	xser(FILE*in,FILE*out):in(in),out(out){}
	xser&w(const size_t d){fprintf(out,"%lu ",d);return*this;}
	xser&r(size_t&d){fscanf(in,"%lu ",&d);return*this;}
	xser&w(const char*b,const size_t size=0){fprintf(out,"%lu %s ",size?size:strlen(b),b);return*this;}
	xser&r(char**buf,size_t&size){
		if(*buf)
			delete *buf;//?
		fscanf(in,"%lu ",&size);
		*buf=new char[size];
		const size_t s=fread(*buf,size,1,in);
		if(s!=1){
			perror("rs");
			throw "error while reading";
//			exit(101);
		}
		return*this;
	}
	xser&r(char**buf){size_t size=0;return r(buf,size);}
	xser&flush(){fflush(out);return*this;}
};

#endif




