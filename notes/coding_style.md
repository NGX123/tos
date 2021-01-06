# Coding Style

## File Layout

1. Headers
2. Macros
3. Types
4. Global Variables
5. Functions
6. main

## Leading whitespace

* **DO** use 4 spaces as tab.

## Blocks

* `{` on own line.
* `}` also on own line except on `do while` statements.

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

while (cond)
{
    // ...
}

do
{
    // ...
} while (cond);

class FooBar
{
    FooBar();
};

namespace bazz
{

} // namespace bazz

extern "C"
{
    // ...
}

void do_foo()
{
    // ...
}
```

## Comments

* Have a file description at the top of each file
```C
/*
    @author = nickname
    @brief = file description
*/
* Put the function description comments only in header files and not in source files
* Use the special formating for the function comments
```C
/*
    @brief = function description
    @param parameter-name = parameter description
    @param paramter-name2 = parameter description
    @return = return values of the function
*/
int foo();
```
* Avoid comment that paraphrases the code. Comments should answer the why and the code the how.

## Functions

* Return type and modifiers on the same line as the function name.
* The arguments is adjacent to the function name.
* All functions that would be used globally should be seperated from local in a seperate header file and be declared as extern
* All local functions should be declared static

*Example:*

```C
static void foo()
{
    bar;
}

foo();
```

## Keywords

* Use a space after `if`, `for`, `while`, `switch`.
* Do not use a space after the opening ( and before the closing ).
* Preferably use () with `sizeof`.
* Use `nullptr` when necessary otherwise use `NULL`.
* Mostly use curly braces after `if`, `for`, `while`, `switch` statements.

**DO**
```C
if (foo)
{
    bar;
}
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

## Headers

* Include libraries first, then include local headers (separate them with a new line).
* Use `.h` as extension.

*Example:*

```C
#include <libsystem/io/Stream.h>

#include "foo.h"
```

## Line length

* Keep lines between 60 and 80 characters long.

## Handling Errors

* `return -1` when there is an error
* `return 0` when there is not

## Formating
* Each seaction(e.g. file description, declarations, functions...) should be seperated with 2 lines while the elements within it should be seperated with one line

*Example:*

```C
int foo;


int foobar();

int foofoo();
```