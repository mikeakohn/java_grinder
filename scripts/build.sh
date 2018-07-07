#/usr/bin/env bash

SOURCEDIR=/storage/git
VERSION=`date +"%Y-%m-%d"`
RPM_VERSION=`date +"%Y.%m.%d"`
VERSION_H=`date +"%B %d, %Y"`
PROGRAM=java_grinder
FULLNAME=${PROGRAM}-${VERSION}

cd ${SOURCEDIR}
git clone https://github.com/mikeakohn/java_grinder.git
cd

rm -rf ${FULLNAME} ${FULLNAME}.tar.gz
cp -r ${SOURCEDIR}/${PROGRAM} ${FULLNAME}

cd ${FULLNAME}
make clean

cat <<EOF > common/version.h
#ifndef _VERSION_H
#define _VERSION_H

#define VERSION "$VERSION_H"

#endif
EOF

rm -rf .git .gitignore

cd ..
tar cvzf ${FULLNAME}.tar.gz ${FULLNAME}

cp ${FULLNAME}.tar.gz /dist

