<!---
	@author 	= Taken from SkiftOS
	@brief 		= includes the description of coding style used in the OS
	@original	= https://github.com/skiftOS/skift/blob/main/manual/00-meta/coding_style.md
	@license 	= Unspecified
		Copyright © 2018-2021 N. Van Bossuyt & contributors

		Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

		The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--->



# Coding Style
## File Layout

* File sections should be arranged in the following order
1. Headers
    * Global
    * Local
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
* DO not use space after the opening ( and before the closing )
* Use one space on each side of `=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :`
* Do not use space after unary `&  *  +  -  ~  !  sizeof  typeof  alignof  __attribute__  defined`, `++  --` or `. ->`

## Comments

* Put all of the code description near the code and not above or below it, except for declarations and sections
* If the comment talks about multiple lines use /* */
	*	```c
		int this;				/* Declares a variable for use as a condition and prints hello */
		if (this)
			printf("hello");
* If the comment talks about one line of code use //
	*	```c
		int this; 				// A variable declaration
		```

*Example:*

**DON'T**
```C
    // This does something
    while (condition)
        if (this  == this)
            do_something()
```

**DO**
```C
    while (condition)       // This does something
        if (this  == this)
            do_something();
```

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