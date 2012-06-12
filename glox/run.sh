src=sh/mknrun.sh

pwd=`pwd`&&
wd=`dirname $0`&&
#echo workdir: $wd
cd $wd&&
. $src&&
cd $pwd&&
#pwd&&
echo "done"