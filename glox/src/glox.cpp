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
//	const static float dt;
};
//const float glox::dt=glox::dtms/.1f;
inline float d(const float f){return f*.1f;}
#endif




#ifndef __p3__
#define __p3__
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
#endif




#ifndef __object__
#define __object__
#include<iostream>
#include<vector>
using namespace std;
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
#endif



#ifndef __teapot__
#define __teapot__
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
#endif





#ifndef __world__
#define __world__
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
#endif






#ifndef __window__
#define __window__
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
		glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
		return 0;
	}
};
world&window::wld=*new world();
int window::w=512;
int window::h=512;
p3 window::p=p3(0,0,-.5);
p3 window::a=p3();
int main(int argc,char**argv){return window::main(argc,argv);}
#endif






