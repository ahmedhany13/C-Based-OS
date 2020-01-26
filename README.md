# C-Based-Operating-System

This a simulation for a simple Operating System Shell

## First in order to run you need : 

• Bochs x86 Processor Simulator: type sudo apt-get install bochs
and sudo apt-get install bochs-x in a terminal to download Bochs.

• bcc (Bruce Evan’s C Compiler): a 16-bit C Compiler. Type
sudo apt-get install bcc to obtain bcc.

• as86, ld86: A 16 bit assembler and linker that typically comes with bcc.
To get as86 and ld86 type sudo apt-get install bin86.

• gcc: the standard 32-bit GNU C compiler. This generally comes with Unix.

• nasm: the netwide assembler. To get nasm type sudo apt-get install nasm.

• hexedit: a utility that allows you to edit a file in hexadecimal byte-by-byte.
To get hexedit type sudo apt-get install hexedit.

• dd: a standard low-level copying utility. This generally comes with Unix.

• A text editor.

## Now to run the files open the terminal and enter the following commands inside the terminal making sure that you're in the correct path , which is the path of the file:
```
./CompileOS.sh 
```
this command compiles the files 

```
bochs -f config.bxrc

```
now if you faced a black screen in bochs window , just type 
```
c
```

in the terminal and it will do the trick 




Contributers :
Ahmed Hany Abdelaziz . Email: ahmedhany.abdelaziz1@gmail.com	github : ahmedhany13
Moustafa mahmoud , github : mostafamhmod

