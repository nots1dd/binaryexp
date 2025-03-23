# Binary Exploitation

This is a sort of tutorial / demo workshop aimed to pique anyones interest towards Binary Exploitation

## Demos 

Each demo is quite unique and essential in understanding the basics and ground work of Bin Exp

Here is the file structure of the demos:

```md
BinaryExp
├── CMakeLists.txt
├── demos
│   ├── c
│   │   ├── demo1
│   │   │   ├── README.md
│   │   │   └── segfault.c
│   │   ├── demo2
│   │   │   ├── flag.txt
│   │   │   ├── README.md
│   │   │   └── system.c
│   │   └── demo3
│   │       ├── exploit.c
│   │       ├── README.md
│   │       └── victim.c
│   ├── cpp
│   │   ├── env_var.cpp
│   │   ├── exploit.py
│   │   └── README.md
│   ├── python
│   │   ├── actualrick.txt
│   │   ├── flag.txt
│   │   ├── payload.py
│   │   ├── picklerick.py
│   │   ├── rick.txt
│   │   └── serialization.py
│   └── rust
│       ├── Cargo.lock
│       ├── Cargo.toml
│       ├── lib.c
│       ├── README.md
│       └── src
│           └── main.rs
├── LICENSE
├── Makefile
└── README.md
```

> [!NOTE]
> 
> Each demo has the following to help you understand the demo in detail:
> 
> 1. Source code comments that explain the **issue**, **solution** and **topics**
> 2. **README** file that explains everything **IN DETAIL**
> 


## References

If you are really interested in binary exploitation and want to go explore more,

Here are some great websites/source material I have used in the past that have helped me A LOT:

1. **[CTF HANDBOOK](https://ctf101.org/binary-exploitation/overview/)**
2. **[GitBook](https://ir0nstone.gitbook.io/notes/binexp/stack/introduction)**
3. **[More resources](https://github.com/alex-bellon/cybersecurity-resources/blob/master/binary-exploitation.md)**
4. **[Other random resources](https://github.com/W3rni0/W3rni0-awesome-resources)**
5. **[Intro to buffer overflow](https://bugbase.ai/blog/introduction-to-binary-exploitation-exploiting-buffer-overflows)**

## Building

It is very simple just run these commands:

```bash 
make build-all # build ALL the codes (C,C++,Rust)
make run_rust # to run the Rust binary 
make run_segfault # to run the segfault binary
```

## BASIC COMMANDS

1. To compile C code: 

```sh 
gcc <name>.c 
```

2. To execute a binary (normally it is `a.out`): 

```sh 
./<name>
```

3. To use GDB:

```sh 
gdb ./<executable>
```


## NOTES 

Needless to say, any issues or additions that may feel is useful, you can always create an **[issue](https://github.com/nots1dd/binaryexp/issues)** or a **[PR](https://github.com/nots1dd/binaryexp/pulls)**

## AUTHOR

References, notes and demos created by [nots1dd](https://github.com/nots1dd)

## LICENSE

This reference is licensed under GNU Lesser General Public License 3.0
