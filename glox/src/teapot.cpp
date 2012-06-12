#ifndef __teapot__
#define __teapot__

#include"object.cpp";

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


#endif
