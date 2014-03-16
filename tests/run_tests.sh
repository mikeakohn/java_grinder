#!/usr/bin/env bash

make

echo " ---- Testing MSP430 ----"

for file in *.class
do
  file=${file%.class}
  ../java_grinder ${file}.class ${file}.asm msp430g2231 > /dev/null
  naken_asm -l -I/storage/git/naken_asm/include -o ${file}.hex ${file}.asm > /dev/null
  a=`naken_util -run ${file}.hex`
  cycles=`echo ${a} | sed 's/ clock cycles.*$//' | sed 's/^.* //'`
  answer=`echo ${a} | sed 's/^.* r15: //' | sed 's/,.*$//'`
  answer=`printf "%d" ${answer}`
  result=`cat ${file}.result`
  echo -n ${file} ": " ${cycles} "cycles", ${answer}
  if [ ${answer} -ne ${result} ]
  then
    echo " FAIL got ${answer} but expected ${result}"
    exit 1
  fi
  echo " PASS"
done

make clean


