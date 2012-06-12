#ifndef __window__
#define __window__

#include"world.cpp";

static world wld;
static int wi=512;
static int hi=512;
static p3 p=p3();
static p3 a=p3();
class window{
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


#endif
