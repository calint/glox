srcdir=.
bindir=.
src=$srcdir/glox.cpp
bin=$bindir/glox
cmd="g++ -o $bin $src\
	-Wfatal-errors -Wall -Wextra -Werror -Wconversion -pedantic -pedantic-errors\
	-framework OpenGL\
	-framework GLUT\
	-I/System/Library/Frameworks/GLUT.framework/Versions/Current/Headers\
	-I/System/Library/Frameworks/OpenGL.framework/Versions/Current/Headers\
	-L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries
"&&
echo $cmd&&
$cmd&&
ls -la $bin&&
./$bin
