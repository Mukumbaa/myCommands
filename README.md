# myCommands
This repository contains some utils i wanted to build for fun.<br /><br />

## strngs
`strngs` is a copy of the comand `strings`. Given a .exe file in input, it will find the printable strings contained in the file.<br />
<br />
Options for `strngs`:<br />
* `-l num`<br />
  Will find only the strings longer then `num` (which is an integer).<br />
  Default: 4<br />
* `-s "pattern1;pattern2;..."`<br />
 	Will find only the strings that contains the patterns given. You can give more patterns by separating them with a `;`.<br />
 	Is NOT a regex, it will find the same exact word you give.<br />
  Not case sensitive.<br />
* `-S "pattern1;pattern2;..."`<br />
 	Same as `-s` but case sensitive.<br /><br />
### Example of usage:
```
  strngs file.exe -s "thread" -l 10
```
<br />

## c2asm
`c2asm` take in input a .c file and gives a .asm file.<br />
This is for learning assembly and it will NOT work with complex c file.<br />
This is ment for c files that don't use external libraries.<br />
Needs gcc to function.<br />
<br />
The basic usage is:<br />
```
  c2asm file.c
```
This will create a output.asm file with the assembly version of the c code in file.c.
If you want to change the name for the .asm file you can specify the name like this:
```
  c2asm file.c example
```
This will create a example.asm file.
<br /><br />
Options for `c2asm`:<br />
* `-e`:<br />
	Compile the .c file and creates a output.asm as before, plus a output.exe and output.txt. This output.txt contains the objdump of the .exe file.
* `-ed`:<br />
	Same as `-e` but delete the .exe file.
<br /><br />
### Example of usage:
```
  c2asm -ed file.c out
```
creates an out.asm and out.txt file.<br /><br />
## ftree
`ftree` will print a simple tree version of the directory where it is launched.<br /><br /> 
Options for `ftree`:<br />
* `-d`:<br />
  Will go in loop and, if any changes are detected in the repository, it will print the update tree of the directory.<br /><br />
## hxd
`hxd` will take a decimal or hexadecimal number and convert it in their corrispective decimal or hexadecimal rappresentation.<br /><br />
Option for `hxd`:<br />
* `-d`:<br />
  Will convert to decimal the given number.
* `-h`:<br />
  Will convert to hexadecimal the given number.
* `-64`:<br />
  Will use a 64 bit rappresentation.<br />
  Running `hxd 0xffffffff` will give `-1` in output, running `hxd -64 0xffffffff` will give `4294967295` in output.<br /><br />

`hxd` is capable of understending if the number given in input is decimal or hexadecimal,
so the flags `-d` and `-h` can be omitted for most cases.<br /><br />
## libstr
`libstr` is a library to manage and manipulate strings in c. Uses a String structure with
the fields str, containing the char*, and len, containing the length of the string.<br /><br />
### List of the functions:<br />
#### `String str_init(const char *s, int max_len)`

Initializes a `String` structure by copying a null-terminated C string, with bounds checking.

**Parameters:**<br/>
- `s` (*char \**): Pointer to the source null-terminated C string  
  *Must not be NULL*
- `max_len` (*int*): Maximum number of characters to copy (excluding null-terminator)  
  *Must be ≥ 0*

__Return__ (*String*)<br/>
---
### void str_reset(String *str){
Description<br />
Reset a given String structure<br />
param str<br />
Pointer to a String structure<br />
return<br />
void<br />
### void str_resetr(char **str, int len)
Description<br />
Reset a given C-style string<br />
param str<br />
Pointer to a C-style string<br />
param len<br />
Maximum length to check<br />
return<br />
void<br />
### void str_cat(String *dest, const String src, int index)
Description<br />
Perform a concatenation of the content of two String structure.<br />
This modify the first String structure given.<br />
Takes also an index, the content of the second String will<br />
go from the index given of the first String. If the value of index<br />
is less or greater of the sum of the two String, will perform the<br />
simple concatenation at the end of the first String.<br />
param dest<br />
First String, this will be modified<br />
param src<br />
Second String<br />
param index<br />
Index of the first String where the content of the second String will be inserted<br />
return<br />
void<br />
 
