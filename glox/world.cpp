#ifndef __world__
#define __world__

#include"teapot.cpp";

class world:public object{
public:
	world():object(*this){
		for(int k=-3;k<3;k++){
			teapot*o=new teapot(*this);
			o->rot(p3(k*90,0,0));
			o->transl(2.f*k,0.f,-10.f);
			chs.push_back(o);
		}
	}
	void gldraw(){}
};


#endif
