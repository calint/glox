#ifndef __world__
#define __world__

#include"teapot.cpp";

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
		object::dtms=100;
		cout<<"      dt: "<<dtms<<"ms"<<endl;
		object::dt=object::dtms/1000.;
		object::tms+=object::dtms;
		object::tick();
	}
};


#endif
