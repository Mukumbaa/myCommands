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
___
#### `String str_init(const char *s, int max_len)`

Initializes a `String` structure by copying a null-terminated C string, with bounds checking.

**Parameters:**<br/>
- `s` (*char \**): Pointer to the source null-terminated C string  
  *Must not be NULL*
- `max_len` (*int*): Maximum number of characters to copy (excluding null-terminator)  
  *Must be ≥ 0*

**Return ->** (*String*)
___
#### `void str_reset(String *str)`

Reset a given String structure.

**Parameters:**<br />
- `str` (*String \**): Pointer to a String structure

**Return ->** (*void*)
___
#### `void str_resetr(char *str, int len)`

Reset a given C string, with bounds checking.

**Parameters:**<br />
- `str` (*char \**): Pointer to a C-style string
- `len` (*int*): Maximum bounds check for string

**Return ->** (*void*)
___
#### `void str_cat(String *dest, const String src, int index)`

Perform a concatenation of the content of two String structure.
This modify the first given String structure.
Takes also an index, the content of the second String will
go from the index given of the first String. If the value of index
is less or greater of the sum of the two Strings, will perform the
simple concatenation at the end of the first String.

**Parameters:**<br />
- `dest` (*String \**): Pointer to a String structure.
  *This parameter WILL be MODIFIED*
- `src` (*String*): String structure, its content will be inserted in `dest`
- `index` (*int*): Indicate the index relative to the first String (`dest`) where the content of the second String
(`src`) will be inserted.

**Retrun ->** (*void*)
___
#### `int str_cmp(String str1, String str2)`

Compare the two given Strings. If their contents are equals returns 0, else 1.

**Parameters:**<br />
- `str1` (*String*): First String.
- `str2` (*String*): Second String.

**Return ->**  (*int*)
___
#### `int str_contains(String str, String pattern, int is_case_sensitive)`

Controls if a given String (`pattern`) is contained in another String (`str`).
Returns the index where the pattern start in the first String.

**Parameters:**<br />
- `str` (*String*): String where to find the pattern.
- `pattern` (*String*): String containing the pattern.
- `is_case_sensitive` (*int*): Flag, 1 for case sensitive search, 0 for non case sensitive search.  

**Returns ->** (*int*)
___
#### `int str_split(String **arr, String str, char delimiter, int token_max_len)`

Splits the given String by the given char delimiter. Its required a max length for the tokens.
Returns the number of splits.

**Parameters:**<br />
- `arr` (*String \*\**): Pointer to String*, it will be populated with the tokens found by the split.
- `str` (*String*): String to split.
- `delimiter` (*char*): Delimiter for the split.
- `token_max_len` (*int*): Max length for the tokens found.

**Returns ->** (*int*)
___
#### `void str_cpy(String *dest, const String src)`

Copies the content of the second String to the first String.

**Parameters:**<br />
- `dest` (*String \**): Where the content will be copied.
- `src` (*String*): Content to be copied.

**Returns ->** (*void*)
___

