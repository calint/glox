srcdir=.
bindir=.
src=$srcdir/glox.cpp
bin=$bindir/glox
#cc=g++
cc=clang++
cmd="$cc -o $bin $src\
	-std=c++0x -Wfatal-errors -Wall -Wextra -Werror -Wconversion -pedantic -pedantic-errors\
	-framework OpenGL\
	-framework GLUT\
	-I/System/Library/Frameworks/GLUT.framework/Versions/Current/Headers\
	-I/System/Library/Frameworks/OpenGL.framework/Versions/Current/Headers\
	-L/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries
"&&
echo $cmd&&
$cmd&&
echo&&
ls -la $bin&&
cat $bin|wc&&
gzip  -c $bin|wc&&
echo&&
ls -la $src&&
cat $src|wc&&
gzip  -c $src|wc&&
echo&&
./$bin
