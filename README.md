Java Grinder
============

Compile Java bytecode to microcontroller assembly.  Currently supporting
MSP430, dsPIC, 6502/6510, 68000, MIPS, TMS9900, and Z80 with platforms that
include Commodore 64, Sega Genesis, Atari 2600, Apple IIgs, ChipKit (PIC32),
TI99, TI84, Playstation 2, and more.

Authors:
- Michael Kohn
- Joe Davisson
- Carsten Dost

For more info:
[http://www.mikekohn.net/micro/java_grinder.php](http://www.mikekohn.net/micro/java_grinder.php)

Examples
--------
* [Sega Genesis](http://www.mikekohn.net/micro/sega_genesis_java.php)
* [Commodore 64](http://www.mikekohn.net/micro/c64_java.php)
* [Atari 2600](http://www.mikekohn.net/micro/atari2600_java.php)
* [TI/99a](http://www.mikekohn.net/micro/ti99_java.php)
* [dsPIC](http://www.mikekohn.net/micro/dspic_mandelbrots.php)
* [Apple IIgs](http://www.mikekohn.net/micro/apple_iigs_java.php)
* [Parallax Propeller](http://www.mikekohn.net/micro/propeller_java.php)
* [Playstation 2](http://www.mikekohn.net/micro/playstation2_java.php)

How it works
------------

This is just a little blurb on how this all fits together maybe to
help others who want to make a generator for another CPU architecture.
Basically the code can be separated into 4 distinct modules in 4
directories:

* java: This directory holds all the Java code which will be compiled
with javac into a .jar file.  Anyone who is programming against the
Java Grinder API will use classes / methods from this package.

* common: This is the main module with a Java .class file reader and
a compiler.  The compile.cxx functions will make calls to either the
Generator module or to the api/invoke.cxx module.

* generator: This holds the abstract Generator super-class which each
CPU architecture extends in order to create an engine for each CPU type.
Anyone creating code for a new architecture will hopefully spend 100%
of their time in here and 0% of their time anywhere else in Java Grinder.


There is also a directory called "tests" which hold little test programs
used to make sure Java Grinder is working properly.

Generator
---------

The Generator is an abstract class with some abstract functions and
some functions that will return -1.  If a function returns -1 it simply
is telling the compile.cxx module that this function isn't implemented.
Some functions in the Generator are only for optimizations and can be
left unimplemented.  When compile.cxx hits these functions it will call
the non-optimized version of the function instead.  Here's an example:

If we had the following Java code:

```
a = b + 1
```

Both a and b are local variables, so the Java byte code ends up being:

```
 8: iload_1     (push local variable 1 onto stack)
 9: iconst_1    (push the constant 1 on the stack)
10: iadd        (pop 2 values off stack, add them, push answer)
```

In Generator.h there are two functions used for adding integers:

```
virtual int add_integer() = 0;
virtual int add_integer(int num) { return -1; }
```

The first one is the unoptimized version and is abstract.  It must be
implemented by the architecture.  The second one is called by compile.cxx's
optimizer.  If the optimizer sees a constant pushed on the stack and notices
that the next instruction is to add them, then it will call `add_integer(int num)`
first to see if that's implemented.  If it returns -1, then it will call
the unoptimized version `add_integer()`.  The difference is in the unoptimized
version it would do:
```
mov.w -4(r12), r4
mov.w #1, r5
add.w r5, r4
```
While `add_integer(int num)` knows what the constant is and can just add
it to whatever is at the top of the stack.

    mov.w -4(r12), r4
    add.w #1, r4

The optimized function could also be implemented and return 0 if
some conditions are met or return -1 if it can do it.  For example
if the `add_integer(int num)` function for a certain CPU could only
add an immediate value between 0 and 255, then the generator for that
CPU can check if "num" is between 0 and 255, return -1 if false,
or `fprintf()` some assembly code and return 0 if true.

The same thing goes for API calls.  There are some API calls (such
as initializing SPI in MSP430 and dsPIC) that if one or two parameters
in Java are constants, the generator can produce optimized code for them.
So if `spi_init()` has an overloaded version of this function that takes in
constants, the optimizer in compile.cxx will automatically try and use
it first.
