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
};

class object:public p3{
protected:
	p3 a;
	vector<object*>chs;
public:
	object():p3(),a(){}
	object(const p3&p,const p3&a):p3(p),a(a){}
	virtual ~object(){}
	void draw(){
		glPushMatrix();
		glTranslatef(getx(),gety(),getz());
		glRotatef(a.getx(),1.0f,0.0f,0.0f);
		glRotatef(a.gety(),0.0f,1.0f,0.0f);
		glRotatef(a.getz(),0.0f,0.0f,1.0f);
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
};

class teapot:public object{
public:
	teapot(const p3&p,const p3&a):object(p,a){}
	void gldraw(){
		glutSolidTeapot(1);
	}
};

class world:public object{
public:
	world():object(){
		for(int k=-3;k<3;k++)
			chs.push_back(new teapot(p3(k*2,0,-10),p3(k*90,0,0)));
	}
	void gldraw(){}
};

static world wld;
class vm{
public:
	static void draw(){
		cout<<"draw"<<endl;
		glClearColor(0,0,0,0);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
//		glTranslatef(p.getx(),p.gety(),p.getz());
//		glRotatef(a.getx(),1.0f,0.0f,0.0f);
//		glRotatef(a.gety(),0.0f,1.0f,0.0f);
//		glRotatef(a.getz(),0.0f,0.0f,1.0f);
		wld.draw();
		glutSwapBuffers();
	}
	static void reshape(int w,int h){
		cout<<"reshape: "<<w<<" x "<<h<<endl;
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45,1,1,1000);
	}
	static void keyb(unsigned char key,int x, int y){
		cout<<"keyboard: "<<key<<" @ "<<x<<","<<y<<endl;
	}
	static void mouseclk(int button,int state,int x,int y){
		cout<<"mouseclk: "<<state<<"  "<<button<<" @ "<<x<<","<<y<<endl;
	}
	static void timer(int value){
		cout<<"timer: "<<value<<endl;
		glutTimerFunc(1000,timer,value);
	}
	//static void idle(){
	//	printf("idle\n");
	//	return;
	//}
	static void mousemov(int x,int y){
		cout<<"mousemov: "<<x<<","<<y<<endl;
	}
	static int main(int argc,char**argv){
		glutInit(&argc,argv);
		glutInitWindowSize(512,512);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		glutCreateWindow("hello world");
		glutDisplayFunc(vm::draw);
		glutReshapeFunc(vm::reshape);
		glutKeyboardFunc(vm::keyb);
		glutMouseFunc(vm::mouseclk);
		glutTimerFunc(1000,vm::timer,1);
		glutMotionFunc(vm::mousemov);
	//	glutIdleFunc(idle);
	//	glClearColor(0,0,0,1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glutMainLoop();
		return 0;
	}
};
int main(int argc,char**argv) {
	return vm::main(argc,argv);
}
