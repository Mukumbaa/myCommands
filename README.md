# myCommands
This repository contains some utils i wanted to build for fun.<br />

## strngs
`strngs` is a copy of the comand `strings`. Given a .exe file in input, it will find the printable strings contained in the file.<br />
<br />
Options for `strngs`:<br />
* `-l num`<br />
  Will find only the strings longer then `num` (which is an integer).<br />
  Default: 4<br />
* `-s` "pattern1;pattern2;..."<br />
  Will find only the strings that contains the patterns given. You can give more patterns by separating them with a `;`.<br />
  Is NOT a regex, it will find the same exact word you give.<br />
  Not case sensitive.<br />
* `-S` "pattern1;pattern2;..."<br />
  Same as `-s` but case sensitive<br />

## c2asm
