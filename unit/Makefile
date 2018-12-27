
CFLAGS=-O3 -Wall -I../common -I../api -std=c++0x
OBJECTS=../build/Util.o ../build/invoke.o

default:
	g++ -o method_sanitize method_sanitize.cxx $(CFLAGS) $(OBJECTS)
	g++ -o get_static_function get_static_function.cxx $(CFLAGS) $(OBJECTS)
	g++ -o get_virtual_function get_virtual_function.cxx $(CFLAGS) $(OBJECTS)

clean:
	@rm -f method_sanitize get_static_function get_virtual_function
	@echo "Clean!"

