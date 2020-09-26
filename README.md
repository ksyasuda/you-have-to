# You-Have-To Job Tracker

You-Have-To Job Tracker is a command-line application written in C++, using the
[Getopt](https://en.wikipedia.org/wiki/Getopt "Getopt Wiki page") C library for
parsing command-line arguments.

---

## How to Build

With [GCC/G++](https://gcc.gnu.org/) installed, run make in the You-Have-To
directory. Once the program compiles, the executable `pls.exe` will be
generated.

---

## Usage

### Input Format

Input for the program must be formatted in the format "Job Name|Job
Title|Location|Date applied". Each section must be separated by a single '|'
character and there must be 4 sections in total.

All valid ASCII characters are accepted as input to the program with the
exception of the '|' character.

<p style="text-align: center; font-weight: bold; font-size: 12px;">The '|' character must be used for separating the 4 sections of the input and
nothing else.</p>

### Command-Line Arguments

-   `--verbose,` `-v` Enable verbose output
-   `--print`, `-p` Print content of the file (requires password if encrypted)
-   `--insert`, `-i` Write a new job to the file
-   `--help`, `-h` Get a very useful help prompt
-   `--search`, `-s` Search for a job by job name
-   `--count`, `-c` Print content of the file and number of jobs (requires password if encryped)
-   `--encrypt`, `-e` Encrypt content of the file
-   `--decrypt`, `-d` Decrypt content of the file (requires password)

---
