#ifndef __glox__
#define __glox__

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#include <glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

class glox{
public:
	const static int dtms=100;
//	const static float dt;
};
//const float glox::dt=glox::dtms/.1f;
inline float d(const float f){return f*.1f;}
#endif
