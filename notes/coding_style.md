# Coding Style

## File Layout

* File sections should be arranged in the following order
1. Headers
2. Macros
3. Types
4. Global Variables
5. Functions
6. main

* Have a file description at the top of each file

*Example:*

```C
/*
    @author = nickname
    @brief = file description
*/
```

* Each seaction(e.g. file description, declarations, functions...) should be seperated with 2 lines while the elements within it should be seperated with one line

*Example:*

```C
int foo; // Section 1


int foobar(); // Section 2

int foofoo(); // Section 2, element 2
```

* Use the special formating for the function comments and keep all function comments in the header file

*Example:*

```C
/*
    @brief = function description
    @param parameter-name = parameter description
    @param paramter-name2 = parameter description
    @return = return values of the function
*/
```

## Indentation

* Use 4 spaces as tab.
* **DO NOT** use space as an indentation
* Try no to leave white spaces at the end of lines
* Do not put multiple assignments or statements on a single line

*Example:*

**DON'T**
```C
    if (condition) do_this;
    do_something_everytime;

    something; something;
```
**DO**
```C
    if (condition)
        do this;
        do_something_everytime;

    something;
    something;
```

## Blocks

* `{` and `}` are on their own lines, except on `do while` statements.

*Example:*

```C
    if (cond)
    {
        // ...
    }
    else
    {
        // ...
    }

    do
    {
        // ...
    } while (cond);
```

* Do not unnecessarily use braces where a single statement will do, unless only one brach of conditional statement is single statement, in latter case use braces in both branches

*Example:*

```C
    if (condition)
        action();

    if (condition)
    {
        do_this();
        do_that();
    }
    else
    {
        otherwise();
    }
```

## Spaces

* Use spaces after `if`, `for`, `while`, `switch`, `case`, `do`
* Do not use spaces after `sizeof`
* Use one space on each side of `=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :`
* Do not use space after unary `&  *  +  -  ~  !  sizeof  typeof  alignof  __attribute__  defined`, `++  --` or `. ->`

## Functions

* Do not use more then 10 local variables
* All functions that would be used globally should be seperated from local in a seperate header file and be declared as extern
* All local functions should be declared static
* Return type and modifiers on the same line as the function name.

*Example:*

```C
static void foo()
{
    bar;
}
```

## Keywords

* Use a space after `if`, `for`, `while`, `switch`.
* Do not use a space after the opening ( and before the closing ).
* Use () with `sizeof`.

## Headers

* Include libraries first, then include local headers (separate them with a new line).
* Use `.h` as extension.

*Example:*

```C
#include <libsystem/io/Stream.h>

#include "foo.h"
```

## Switch

* Indent cases another level.
* Switch case should NOT falltrough.
* But if you have no other choise (very unlikely) add a `/* FALLTROUGH */` comment.

*Example:*

```C
switch(foo)
{
    case bar:
        printf("foo");
        /* FALLTROUGH */

    case mitzvah:
        printf("barmitzvah\n");
        break;
}
```