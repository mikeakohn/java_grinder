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

Adding A New System
-------------------

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

