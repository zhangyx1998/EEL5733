# EEL5733 Assignment 4

## Notes

This project is based on my previous implementation of assignment 2.
The following modifications are made to meet the requirements for assignment 4:

1.	The initialization of `Queue` uses `mmap` instead of `malloc`, making it
	accessible from any process. Please check it out at `lib/queue.c:35`.

2.	The lock structs are also initialized on `mmap` region instead of `malloc`
	region. I used `semaphore` instead of `mutex` for race synchronization. You
	can check it out at `lib/thread.c:117`.

> P.S.
> Support for unnamed semaphore was somehow broken on MacOS. After some
> researches online, I switched to named semaphore specifically for MacOS.
> [reference](https://stackoverflow.com/questions/1413785/sem-init-on-os-x)

## Compile & Test

An extended `Makefile` was implemented along with the project.

+ To build the project, run `make` (alias of `make all`).

	Compiled executables will be stored in `./build/`

+ An automated test system was also implemented based on GNU Make.

	By running `make test`, the make system will run all the tests according to
	the names under `./test/`. The results are stored in corresponding `.tmp`
	files.

	The make script will use `diff` to check for the correctness against
	references (`*.out`). Test results will be displayed in colorized text.

	If a test fails, the `diff` result will be written into its corresponding
	`EXEC_NAME.TEST_ID.diff` file.

## Folder structure

```
.
├── README.md		| This document
├── Makefile		| Make system
├── lib				|
│   ├── *.h			| library headers
│   └── *.c			| library implementations
├── inc				|
│   └── ... 		| headers and sources included by src/*.c ONLY
├── src				|
│   └── *.c			| c sources to be compiled into executables
├── build			|
│   ├── *.o			| objects from both src and lib
│   └── ... 		| executables for this assignment
└── test			|
    ├── Makefile	|
    ├── *.in		| inputs for test
    ├── *.out		| reference outputs for test
    └── *.diff		| only exits if test fails, untracked
```
