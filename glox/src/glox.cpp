#ifndef __glox__
#define __glox__

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#include <glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

class glox{
public:
	const static int dtms=100;
	const static float dt;
};
const float glox::dt=glox::dtms/1000.f;
inline float d(const float f){return f*glox::dt;}

class p3{
	float x,y,z;
public:
	p3():x(0),y(0),z(0){}
	p3(const float x,const float y,const float z):x(x),y(y),z(z){}
	inline const float getx()const{return x;}
	inline const float gety()const{return y;}
	inline const float getz()const{return z;}
	inline p3&transl(const float dx,const float dy,const float dz){x+=dx;y+=dy;z+=dz;return*this;}
};






#include<execinfo.h>
#include<iostream>
using namespace std;
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
template<class T>class span{
private:
	T*a;
	int of;
	int ln;
public:
	inline span(T ae[],const int offset,const int len):a(ae),of(offset),ln(len){}
	inline T&operator[](const int i)const{
		if(i<0||i>=ln)throw signl(1,"indexoutofbounds");
		return a[of+i];
	}
	inline void foreach(const int offset,const int len,void(*f)(T&e)){
		if(offset<0||(offset+len)>ln)throw signl(1,"spanoutofbounds");
		T*p=a+of+offset;
		int i=len;
		while(i--)
			(*f)(*p++);
	}
	inline int len()const{return ln;}
};



#include<vector>
class object:public p3{
protected:
	object&pt;
	p3 a;
	vector<object*>chs;
public:
	object(object&parent):p3(),pt(parent),a(){}
	virtual ~object(){}
	void draw(){
		glPushMatrix();
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1.f,0.f,0.f);
		glRotatef(a.gety(),0.f,1.f,0.f);
		glRotatef(a.getz(),0.f,0.f,1.f);
		gldraw();
		for(unsigned int n=0;n<chs.size();n++){
			chs[n]->draw();
		}
//		for(vector<obj*>::iterator it=chs.begin();it!=chs.end();++it){
//			(*it)->draw();
//		}
		glPopMatrix();
	}
	virtual void gldraw(){};
	inline object&rot(const p3&agl){a=agl;return*this;}
	virtual void tick(){
		for(unsigned int i=0;i<chs.size();i++){
			chs[i]->tick();
		}
	}
};


class teapot:public object{
public:
	teapot(object&pt):object(pt){}
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
//		cout<<"teapot tick  "<<getx()<<endl;
		transl(0,d(.1),0);
	}
};


class world:public object{
public:
	world():object(*this){
		for(int i=-3;i<3;i++){
			teapot*o=new teapot(*this);
			o->rot(p3(i*90,0,0));
			o->transl(2.f*i,0.f,-10.f);
			chs.push_back(o);
//			chs.push_back((object*const&)*o);
		}
	}
	void gldraw(){}
	virtual void tick(){
		cout<<"    tick: "<<endl;
		object::tick();
	}
};



class window{
public:
	static world&wld;
	static int w;
	static int h;
	static p3 p;
	static p3 a;
	static void reshape(const int width,const int height){cout<<" reshape: "<<w<<"x"<<h<<endl;w=width;h=height;}
	static void draw() {
		cout<<"    draw: "<<endl;
//		char*xx=0;*xx=0;
//		throw signl(2,"testing exception ");
		glClearColor(0, 0, 0, 0);
		glClearDepth(1);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    static void pl(const GLfloat x,const GLfloat y,const char*text){
      char*cp=(char*)text;
      glPushMatrix();
      glTranslatef(x, y, 0);
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
		pl(0,0,"glox");
		glDisable(GL_BLEND);
	}
	static void keybd(const unsigned char key,const int x,const int y){
//		throw signl(__LINE__,__FUNCTION__);
		cout<<" keydown: "<<key<<" "<<(int)key<<"@"<<x<<","<<y<<endl;
	}
	static void keybu(const unsigned char key,const int x,const int y){
		cout<<"   keyup: "<<key<<" "<<(int)key<<"@"<<x<<","<<y<<endl;
		if(key==27)//esc
			exit(0);
	}
	static void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<" @ "<<x<<","<<y<<endl;}
	static void timer(const int value){
		cout<<"   timer: "<<value<<endl;
		wld.tick();
		glutPostRedisplay();
		glutTimerFunc(value,timer,value-1);
	}
	//static void idle(){
	//	printf("idle\n");
	//	return;
	//}
	static void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
	static int main(int argc,char**argv){
		printf("glox ");
		glutInit(&argc,argv);
		glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
		glutIgnoreKeyRepeat(true);
		glutInitWindowSize(w,h);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		glutCreateWindow("glox");
		glutDisplayFunc(draw);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keybd);
		glutKeyboardUpFunc(keybu);
		glutMouseFunc(mouseclk);
		glutMotionFunc(mousemov);
		glutTimerFunc(0,timer,100);//? world::fixdt
	//	glutIdleFunc(idle);
//		glutReportErrors();
		glutMainLoop();
//		catch(exception const&e){cout<<"•••• error: "<<e.what()<<endl;stktrace();}
// 		catch(const int i){cout<<"••• error: "<<i<<endl;stktrace();}
//		catch(const char*s){cout<<"••• error: "<<s<<endl;stktrace();}
//		catch(const void*s){cout<<"•• error: "<<s<<" "<<s<<endl;stktrace();}
//		catch(...){cerr<<"• error:"<<endl;stktrace();}
//		return 1;


//		glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
		return 0;
	}
};
world&window::wld=*new world();
int window::w=512;
int window::h=512;
p3 window::p=p3(0,0,-.5);
p3 window::a=p3();



//
//#include <execinfo.h>
//extern "C"{
//	static void stktrace(){
//		const int na=20;
//	  void*va[na];
//	  const size_t n=backtrace(va,na);
//	  backtrace_symbols_fd(va,n,2);
//	  exit(1);
//	}
//}
//try{return window::main(0,NULL);}
//catch(const char*p){cout<<"••• error: "<<p<<endl;stktrace();}
//catch(void*p){cout<<"•• error: "<<p<<" "<<p<<endl;stktrace();}
//catch(...){cerr<<"•••• error:"<<endl;stktrace();}


//static void f(char&c){cout<<c;}
//static void f2(char&c){cout<<c;c=' ';}
//static void main_sigf(const int a){
//	cout<<" ••• terminated with signal "<<a<<endl;
//	exit(a);
//}
int main(){
//	for(int i=0;i<32;i++)//?
//		signal(i,main_sigf);
	return window::main(0,NULL);
//
//	char s[]="hello world";
//	span<char>a=span<char>(s,0,5);
//	span<char>b=span<char>(s,6,5);
//	char&c=b[1];
//	c='a';
//	a.foreach(0,a.len(),f);cout<<endl;
//	b.foreach(2,2,f2);cout<<endl;
//	b.foreach(1,5,f2);cout<<endl;
}




















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
public:
	static unsigned int hash(const char*key,const unsigned int roll){
		unsigned int i=0;
		const char*p=key;
		while(*p){
			*p=0;
			i+=*p++;
		}
		i%=roll;
		return i;
	}
	lut(const int size=8):size(size){array=(el**)calloc(size,sizeof(el*));}
	~lut(){clear();delete array;}
	T operator[](const char*key)const{
		const int h=hash(key,size);
		el*l=array[h];
		if(!l)
			return NULL;
		while(1){
			if(!strcmp(l->key,key))
				return l->data;
			if(l->nxt){
				l=l->nxt;
				continue;
			}
			return NULL;
		}
		return NULL;
	}
	void put(const char*key,T data){
		const int h=hash(key,size);
		const el*l=array[h];
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
			const el*e=array[i];
			if(!e)
				continue;
			delete e;
			array[i]=NULL;
		}
	}
};



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




