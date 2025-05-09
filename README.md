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
creates an out.asm and out.txt file.<br />
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
so the flags `-d` and `-h` can be omitted for most cases.<br />
## libstr
 
