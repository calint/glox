src=glox.cpp
bin=glox
g++ -o $bin $src \
	-framework GLUT \
	-framework OpenGL \
	-I/System/Library/Frameworks/OpenGL.framework/Versions/Current/Headers/ \
	-I/System/Library/Frameworks/GLUT.framework/Versions/Current/Headers/ \
	-L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries &&
ls -la $bin &&
./$bin

#	-framework Carbon \
