
#JOBJS=$(shell ls *.java | sed 's/\.java/\.class/')

default:
	@cd build && make

test: $(JOBJS)

%.class: %.java
	javac $*.java

clean:
	@rm -f *.o java_grinder *.class build/*.o
	@echo "Clean!"



