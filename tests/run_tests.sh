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
  a=`/home/mike/source/naken_asm/naken_util -run ${file}.hex`
  cycles=`echo ${a} | sed 's/ clock cycles.*$//' | sed 's/^.* //'`
  answer=`echo ${a} | sed 's/^.* r15: //' | sed 's/,.*$//'`
  answer=`printf "%d" ${answer}`
  result=`cat ${file}.java | grep '^// result=' | sed 's/\/\/ result=//'`
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
  a=`/home/mike/source/naken_asm/naken_util -6502 -run ${file}.hex`
  cycles=`echo ${a} | sed 's/ clock cycles.*$//' | sed 's/^.* //'`
  answer=`echo ${a} | sed 's/^.* r15: //' | sed 's/,.*$//'`
  answer=`printf "%d" ${answer}`
  result=`cat ${file}.java | grep '^// result=' | sed 's/\/\/ result=//'`
  echo -n ${file} ": " ${cycles} "cycles", ${answer}
  if [ ${answer} -ne ${result} ]
  then
    echo " FAIL got ${answer} but expected ${result}"
    exit 1
  fi
  echo " PASS"
}

run_mips_test()
{
  file=$1
  ../java_grinder $2 ${file}.class ${file}.asm r4000 > /dev/null
  if [ $? -ne 0 ]
  then
    echo "${file} : GRIND FAILED ***"
    exit 1
  fi
  ../../naken_asm/naken_asm -l -I../../naken_asm/include -o ${file}.hex ${file}.asm > /dev/null
  a=`/home/mike/source/naken_asm/naken_util -run -mips ${file}.hex`
  /home/mike/source/naken_asm/naken_util -run -mips ${file}.hex > out.txt
  #cycles=`echo ${a} | grep cycles | tail -n 1 | sed 's/ clock cycles.*$//' | sed 's/^.* //'`
  #answer=`echo ${a} | grep '$v0' | sed 's/^.* $v0: //' | sed 's/ .*$//'`
  cycles=`grep cycles out.txt | tail -n 1 | sed 's/ clock cycles.*$//' | sed 's/^.* //'`
  answer=`grep '$v0' out.txt | tail -n 1 | sed 's/^.* $v0: //' | sed 's/ .*$//'`
  #echo ${a} | grep cycles
  #echo ${cycles}
  #echo ${answer}
  answer=`printf "%d" ${answer}`
  result=`cat ${file}.java | grep '^// result=' | sed 's/\/\/ result=//'`
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

echo " ---- Testing MIPS32 ----"

for file in *.class
do
  file=${file%.class}
  run_mips_test ${file}
done

echo " ---- Testing MIPS32 (Unoptimized) ----"

for file in *.class
do
  file=${file%.class}
  run_mips_test ${file} -O0
done

make clean

