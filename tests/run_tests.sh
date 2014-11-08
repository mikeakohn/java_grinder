#!/usr/bin/env bash

make

run_msp430_test()
{
  file=$1
  ../java_grinder $2 ${file}.class ${file}.asm msp430g2231 > /dev/null
  if [ $? -ne 0 ]
  then
    echo "${file} : GRIND FAILED ***"
    exit 1
  fi
  ../../naken_asm/naken_asm -l -I../../naken_asm/include -o ${file}.hex ${file}.asm > /dev/null
  a=`../../naken_asm/naken_util -run ${file}.hex`
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
}

run_6502_test()
{
  file=$1
  ../java_grinder $2 ${file}.class ${file}.asm c64 > /dev/null
  ../../naken_asm/naken_asm -l -I../../naken_asm/include -o ${file}.hex ${file}.asm > /dev/null
  a=`../../naken_asm/naken_util -65xx -run ${file}.hex`
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
}

echo " ---- Testing MSP430 ----"

for file in *.class
do
  file=${file%.class}
  run_msp430_test ${file}
done

echo " ---- Testing MSP430 (Unoptimized) ----"

for file in *.class
do
  file=${file%.class}
  run_msp430_test ${file} -O0
done

#echo " ---- Testing 6502 ----"

#for file in *.class
#do
#  file=${file%.class}
#  run_6502_test ${file}
#done

make clean


