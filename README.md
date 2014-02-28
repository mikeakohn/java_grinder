java_grinder
============

Compile Java bytecode to microcontroller assembly.  Currently supporting MSP430, dsPIC, and 6502/6510.

Authors:<br>
Michael Kohn<br>
Joe Davisson<br>

For more info:<br>
http://www.mikekohn.net/micro/java_grinder.php<br>
<br>

<hr>

<p><b>How it works</b></p>

<p>This is just a litte blurb on how this all fits together maybe to
help others who want to make a generator for another CPU architecture.
Basically the code can be seperated into 4 distinct modules in 4
directories:</p>

<ul>
<li>java: This directory holds all the Java code which will be compiled
with javac into a .jar file.  Anyone who is programming against the
Java Grinder API will use classes / methods from this package.</li>

<li>common: This is the main module with a Java .class file reader and
a compiler.  The compile.cxx functions will make calls to either the
Generator module or to the api/invoke.cxx module.</li>

<li>generator: This holds the abstract Generator super-class which each
CPU architecture extends in order to create an engine for each CPU type.
Anyone creating code for a new architecture will hopefully spend 100%
of their time in here and 0% of their time anywhere else in Java Grinder.
</li>

</ul>

<p>There is also a directory called "tests" which hold little test programs
used to make sure Java Grinder is working properly.</p>

<p><b>Generator</b></p>

<p>The Generator is an abstract class with some abstract functions and
some functions that will return -1.  If a function returns -1 it simply
is telling the compile.cxx module that this function isn't implemented.
Some functions in the Generator are only for optimizations and can be
left unimplemented.  When compile.cxx hits these functions it will call
the non-optimized version of the function instead.  Here's an example:</p>

<p>If we had the following Java code:</p>
<pre>a = b + 1</pre>

<p>Both a and b are local variables, so the Java byte code ends up being:</p>

<pre>
       8: iload_1      (push local variable 1 onto stack)
       9: iconst_1     (push the constant 1 on the stack)
      10: iadd         (pop 2 values off stack, add them, push answer)
</pre>

<p>In Generator.h there are two functions used for adding integers:</p>
<pre>
  virtual int and_integer() = 0;
  virtual int and_integer(int num) { return -1; }
</pre>

<p>The first one is the unoptimized version and is abstract.  It must be
implemented by the architecture.  The second one is called by compile.cxx's
optimzer.  If the optimizer sees a constant pushed on the stack and notices
that the next instruction is to add them, then it will call add_integer(int num)
first to see if that's implemented.  If it returns -1, then it will call
the unoptimized version add_integer().  The difference is in the unoptimized
version it would do:</p>

<pre>
  mov.w -4(r12), r4
  mov.w #1, r5
  add.w r5, r4
</pre>

<p>While add_integer(int num) knows what the constant is and can just add
it to whatever is at the top of the stack.</p>

<pre>
  mov.w -4(r12), r4
  add.w #1, r4
</pre>

<p>The optimized function could also be implemented and return 0 if
some conditions are met or return -1 if it can do it.  For example
if the add_integer(int num) function for a certain CPU could only
add an immediate value between 0 and 255, then the generator for that
CPU can check if "num" is between 0 and 255, return -1 if false,
or fprintf() some assembly code and return 0 if true.</p>

<p>The same thing goes for API calls.  There are some API calls (such
as initializing SPI in MSP430 and dsPIC) that if one or two parameters
in Java are constants, the generator can produce optimized code for them.
so spi_init() has an overloaded version of this function that takes in
constants.  The optimizer in compile.cxx will automatically try and use
it first.</p>




