Developer Guidelines
====================

For me, Java Grinder has been a pretty big learning experience.
I've learned how to program a bunch of different CPUs I probably
would never have a reason to mess with and played around with some
older systems.

It seems this project could be fun for others or maybe useful in
a classroom setting, so I put together this small document with
a small checklist on how to add a new CPU or system and what the
coding standards for the project is.

Adding A New CPU
----------------

Adding a new CPU requires extending the generator/Generaor.cxx class
and implementing all the functions that are important to the CPU.
The easiest way to do it is either to copy a generator/Template.cxx
and generator/Template.h file and modifying it match the new CPU.
Another option is to copy one of the other CPU's .cxx/h file and
modifying that.

Most of the functions are there because they need to be, but there are
a few functions that are optimizations.  For example:

    int add_integer();

Tells the Generator to pop 2 values off the stack, add the values, and
push the result to the top of the stack.  In order for this to work
there must have been 2 calls to one of the generator functions to push
values onto the stack.  If the second value was a constant, most CPUs
would allow a version of adding an immediate value to a register so
by haveing an optimized version:

    int add_integer(int num);

Java Grinder can detect this optimization and call the add_intger(int num)
instead of pushing pushing the value onto the stack.  If the new CPU
can't do optimizations like (like MIPS can only do this optimization if
the constant value is between -32768 and 32677) the function can return
-1 to let Java Grinder's core know this optimization didn't happen.  This
function is also not abstract so if it's not defined it will always return
-1.

So as far as the "stack" goes.  This doesn't really mean the CPU's stack
has to be used.  Using PUSH / POP for operations would be quite slow.
Depending on the CPU, the stack more refers to registers.  For example
MIPS has 8 registers (t0 to t7) so when Java Grinder calls
push_int(int32_t n), it will most likely hit code that does:

    fprintf(out, "  move $t%d, $0\n", reg);
    reg++;

So calling add_integer() will output code that looks like:

    fprintf(out, "  add $t%d, $t%d, $t%d\n", REG_STACK(reg-2), REG_STACK(reg-2), REG_STACK(reg-1));
    reg--;

So the two registers at the top of the stack are addeded to gether with
the destination register being the second to top.  If all the registers
are used, then the generator will have to use memory.

After creating the new CPU, the following files have to be edited (note
that any list of files should always be in alphabetical order):

* Edit build/Makefile and add the CPU to CPU= list.
* If the CPU is a system also (like MSP430), edit common/java_grinder.cxx

Adding A New System
-------------------

If this is a microcontroller and no APIs need to be added, adding things
like SPI, digital I/O, etc could be as simple as just defining unimplemented
virtual functions from any of the generator/API_* files.

If this is a game console or such, it could be as easy as creating a
a generator file that extends one of the already implemented CPUs and
implementing function s that are already defined in generator/API_*.

If this is something that needs all new APIs, then steps would be:

* Edit common/java_grinder.cxx to add the new system.
* Edit build/Makefile to add the new system to the SYSTEM= list matching filenames of what will be added to the generator directory.
* Add a new generator/*.cxx and generator/*.h file that extends the CPU class needed.
* Add API files to java/net/mikekohn/java_grinder/ .. functions added here should be empty function definitions or just return something if needed.  No code added here would be used by Java Grinder.
* Edit java/Makefile for the new API.
* If needed add a generator/API_* file into generator and make sure generator/Generator.h inherits from it.  All functions should be virtual and return -1.
* Add files to api/* for the new API.
* Edit api/invoke_static.cxx adding the new class.

Coding Style
------------

Most of the files follow this coding standard so some of the other
assemblers can be used as an example, but here is the basic coding
standard:

* Indentation is 2 spaces (no tabs).
* Make sure there are no trailing spaces at the end of lines.
* 1 space always after the keyword "if" and "for".  For example:

This is okay:

    for (i = 0; i < 1; i++)
    if (i < 5)

This isn't:

    for(i = 0; i < 10; i++)
    if(i < 5)

This is also isn't okay:

    if ( i < 5 )
    if (i < 5 )
    if ( i < 5)

* Always leave (only) 1 space between functions and blocks of if / for while statements.

This is okay:

    if (i < 5)
    {
    }
      else
    {
    }

    if (n < 10)
    {
    }

Not okay:


    if (i < 5)
    {
    }
      else
    {
    }
    if (n < 10)
    {
    }

* Function names are always lowercase with _ separating words.
* Variable names are always lowercase with _ separating words.

I've actually been tempted to change the function name coding style
to camelcase of some kind, but for now this is the way it is.

