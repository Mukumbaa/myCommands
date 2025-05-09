# myCommands
This repository contains some utils i wanted to build for fun.

## strngs
`strngs` is a copy of the comand `strings`. Given a .exe file in input, it will find the printable strings contained in the file.  
  
Options for `strngs`:
* `-l num`
  Will find only the strings longer then `num` (which is an integer).  
  Default: 4  
* `-s` "pattern1;pattern2;..."
  Will find only the strings that contains the patterns given. You can give more patterns by separating them with a `;`.  
  Is NOT a regex, it will find the same exact word you give.  
  Not case sensitive.  
* `-S` "pattern1;pattern2;..."
  Same as `-s` but case sensitive  

## c2asm
