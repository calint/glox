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
	}

	static void draw() {
		cout << "draw" << endl;
		glClearColor(0, 0, 0, 0);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		gluPerspective(45,(GLdouble)(w) / h, .1, 1000);
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
		glutSolidSphere(.2,10,10);
		glColor3f(1,1,1);
		pl(0,0,"hello");
	}
	static void keyb(const unsigned char key,const int x,const int y){cout<<"keyboard: "<<key<<" @ "<<x<<","<<y<<endl;}
	static void mouseclk(const int button,const int state,int x,const int y){cout<<"mouseclk: "<<state<<"  "<<button<<" @ "<<x<<","<<y<<endl;}
	static void timer(const int value){cout<<"timer: "<<value<<endl;glutTimerFunc(value,timer,value-1);}
	//static void idle(){
	//	printf("idle\n");
	//	return;
	//}
	static void mousemov(const int x,const int y){cout<<"mousemov: "<<x<<","<<y<<endl;}
	static int main(int argc,char**argv){
		printf("glox ");
		glutInit(&argc,argv);
		glutInitWindowSize(w,h);
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
		glutCreateWindow("glox");
		glutDisplayFunc(draw);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyb);
		glutMouseFunc(mouseclk);
		glutMotionFunc(mousemov);
		glutTimerFunc(0,timer,1000);
	//	glutIdleFunc(idle);
		glutMainLoop();
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
