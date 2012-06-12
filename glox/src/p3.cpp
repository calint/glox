#ifndef __p3__
#define __p3__

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


#endif
