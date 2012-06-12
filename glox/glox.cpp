#include<iostream>
#include<vector>
#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#include <glut.h>
#endif
using namespace std;

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
};

class teapot:public object{
public:
	teapot(object&pt):object(pt){}
	void gldraw(){
		glEnable(GL_CULL_FACE);//? pushpop enables
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		glFrontFace(GL_CW);
		glutSolidTeapot(1);
	}
};

class world:public object{
public:
	world():object(*this){
		for(int k=-3;k<3;k++){
			teapot&o=*new teapot(*this);
			o.rot(p3(k*90,0,0)).transl(2.f*k,0.f,-10.f);
			chs.push_back(&o);
		}
	}
	void gldraw(){}
};





static world wld;
static int wi=512;
static int hi=512;
static p3 p=p3();
static p3 a=p3();
class vm{
public:
	static void reshape(int w,int h){
		cout<<"reshape: "<<w<<" x "<<h<<endl;
		wi=w;hi=h;
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45,(GLdouble)w/h,.1,1000);
	}
	static void draw(){
		cout<<"draw"<<endl;
		glClearColor(0,0,0,0);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-p.getx(),-p.gety(),-p.getz());
		glRotatef(-a.getz(),0,0,1);
		glRotatef(-a.getx(),1,0,0);
		glRotatef(-a.gety(),0,1,0);

		wld.draw();

		glutSwapBuffers();
	}
	static void keyb(unsigned char key,const int x,const int y){cout<<"keyboard: "<<key<<" @ "<<x<<","<<y<<endl;}
	static void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<" @ "<<x<<","<<y<<endl;}
	static void timer(const int value){cout<<"timer: "<<value<<endl;glutTimerFunc(value,timer,value-1);}
	//static void idle(){
	//	printf("idle\n");
	//	return;
	//}
	static void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
	static int main(int argc,char**argv){
		glutInit(&argc,argv);
		glutInitWindowSize(wi,hi);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		glutCreateWindow("hello world");
		glutDisplayFunc(vm::draw);
		glutReshapeFunc(vm::reshape);
		glutKeyboardFunc(vm::keyb);
		glutMouseFunc(vm::mouseclk);
		glutMotionFunc(vm::mousemov);
		glutTimerFunc(0,vm::timer,1000);
	//	glutIdleFunc(idle);
		glutMainLoop();
		return 0;
	}
};
int main(int argc,char**argv){return vm::main(argc,argv);}
