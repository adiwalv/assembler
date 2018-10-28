## Installation
> ### How to compile:
> 1. Navigate to the right folder with a console.
> 2. Enter :
>```bash
>make 
>```

### Usage
```bash
./assembler file_name [-options] [-h for help]
```

### Options 

> 1. -p : To print source program.
> 2. -s : To print symbol table.
> 3. -i : To print immediate code.
> 4. -t : To print literal table.
> 5. -l : To print lst of the source file.

### To use as a shell command

```bash
make
make install
```
### Usage
```bash
assembler file_name [-options] [-h for help]
```
### To view the lst file, file_name.lst is created when you pass -l
```bash
cat file_name.lst
```

---

# OP Code Table

modrm can be looked up from table below

addresses are in little endian format

| Operation        | Opcode           |
| ------------- |:-------------:|
| mov reg, reg     | 89 modrm | 
| mov eax, lit      | B8 address      |
| mov ecx, lit      | B9 address      |
| mov edx, lit      | BA address      |
| mov ebx, lit      | BB address      |
| mov esp, lit      | BC address      |
| mov ebp, lit      | BD address      |
| mov esi, lit      | BE address      |
| mov edi, lit      | BF address      |

| zebra stripes | are neat      |


# Modrm Table

![alt text](src/images/mod.png "Mod RM")