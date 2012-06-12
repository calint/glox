#ifndef __teapot__
#define __teapot__

#include"object.cpp";

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
		transl(0,d(1),0);
	}
};


#endif
