# EEL5733 Assignment 1

## Compile & Test

An extended `Makefile` was implemented along with the project.

+ To build the project, run `make` (alias of `make all`).

	Compiled executables will be stored in `./build/`

+ An automated test system was also implemented based on GNU Make.

	By running `make test`, the make system will run all the tests
	according to the names under `./test/` directory. The results are
	stored in corresponding .out files.

	The make script will also use `diff` to check for the correctness
	against references (`*.ref`). Test results will be displayed in
	colorized text.

## Folder structure

```plaintext
~/Lab/EEL5733$ tree
	.
	├── Makefile
	├── README.md
	├── inc
	│   ├── list.h
	│   ├── util.h
	│   └── macros.h
	├── src
	│   ├── list.c
	│   ├── util.c
	│   ├── email_filter.c
	│   ├── calendar_filter.c
	│   └── location_updater.c
	├── build
	│   ├── *.bin
	│   └── *.o
	└── test
		├── Makefile
		├── $EXEC_NAME.$NUMBER.in
		├── $EXEC_NAME.$NUMBER.ref
		└── ...
```
