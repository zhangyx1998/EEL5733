# EEL5733 Assignment 3

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
