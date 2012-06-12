#ifndef __object__
#define __object__

#include"p3.cpp";
#include"glox.cpp";
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
