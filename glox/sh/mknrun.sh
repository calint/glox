srcdir=src
bindir=bin

src=glox.cpp
bin=$bindir/glox
bbin=tmpfilefrommknrunsh

cmd="g++ -o $bbin $srcdir/$src \
	-framework GLUT \
	-framework OpenGL \
	-I/System/Library/Frameworks/OpenGL.framework/Versions/Current/Headers/ \
	-I/System/Library/Frameworks/GLUT.framework/Versions/Current/Headers/ \
	-L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries"&&
#echo $cmd&&
$cmd&&
mv -f $bbin $bin&&
ls -la $bin&&
./$bin

#	-framework Carbon \
