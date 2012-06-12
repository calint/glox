srcdir=src
bindir=bin

src=window.cpp
bin=$bindir/glox

cmd="g++ -o $bin $srcdir/$src \
	-framework GLUT \
	-framework OpenGL \
	-I/System/Library/Frameworks/OpenGL.framework/Versions/Current/Headers/ \
	-I/System/Library/Frameworks/GLUT.framework/Versions/Current/Headers/ \
	-L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries"&&
echo $cmd&&
$cmd&&
ls -la $bin&&
./$bin

#	-framework Carbon \
