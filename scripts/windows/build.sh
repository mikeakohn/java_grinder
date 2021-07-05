#!/usr/bin/env sh

FULLNAME=java_grinder-$1

git clone https://github.com/mikeakohn/java_grinder.git
cd java_grinder

cat <<EOF > common/version.h
#ifndef _VERSION_H
#define _VERSION_H

#define VERSION "$VERSION_H"

#endif
EOF

make clean
CXX=x86_64-w64-mingw32-g++ make
make java
cp /usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/libgcc_s_seh-1.dll .
cp /usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/libstdc++-6.dll .
rm -rf build/api/* build/common/* build/generator/* ${FULLNAME}.zip
rm -rf .git
cd ..
mv java_grinder ${FULLNAME}
zip -r ${FULLNAME}.zip ${FULLNAME}

cp ${FULLNAME}.zip /dist/

