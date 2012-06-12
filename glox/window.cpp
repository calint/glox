#ifndef __window__
#define __window__

#include"world.cpp";
class window{
public:
	static world&wld;
	static int w;
	static int h;
	static p3 p;
	static p3 a;
	static void reshape(const int width,const int height){
		cout<<"reshape: "<<w<<" x "<<h<<endl;
		w=width;h=height;
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
	static void keyb(const unsigned char key,const int x,const int y){cout<<"keyboard: "<<key<<" @ "<<x<<","<<y<<endl;}
	static void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<" @ "<<x<<","<<y<<endl;}
	static void timer(const int value){cout<<"timer: "<<value<<endl;glutTimerFunc(value,timer,value-1);}
	//static void idle(){
	//	printf("idle\n");
	//	return;
	//}
	static void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
	static int main_(int argc,char**argv){
		glutInit(&argc,argv);
		glutInitWindowSize(w,h);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		glutCreateWindow("hello world");
		glutDisplayFunc(window::draw);
		glutReshapeFunc(window::reshape);
		glutKeyboardFunc(window::keyb);
		glutMouseFunc(window::mouseclk);
		glutMotionFunc(window::mousemov);
		glutTimerFunc(0,window::timer,1000);
	//	glutIdleFunc(idle);
		glutMainLoop();
		return 0;
	}
};

world&window::wld=*new world();
int window::w=512;
int window::h=512;
p3 window::p=p3();
p3 window::a=p3();

int main(int argc,char**argv){return window::main_(argc,argv);}

#endif
