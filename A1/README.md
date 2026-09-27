# IPv4 Extractor

A C++ program that extracts the first valid IPv4 address, optionally followed by a port, from each input line.

## Requirements

- A C++11 compiler (`c++`, Clang, or GCC)
- Make and Python 3 to run the automated tests

The commands below work in a macOS or Linux terminal. Open the terminal in the folder containing this README, `main.cpp`, and `ipv4.cpp`.

## Build and run

Compile:

```sh
c++ -std=c++11 -Wall -Wextra -Wpedantic main.cpp ipv4.cpp -o ipv4
```

Run:

```sh
./ipv4
```

At the program's prompt, enter a line of text, then press Enter. For example:

```text
server=10.0.0.255:8080end
```

Expected result:

```text
Extracted IPv4 address: 10.0.0.255 (decimal value: 167772415, port: 8080)
```

Enter uppercase `END` to exit. The program prints `Program terminated.` Lowercase `end` does not exit. End-of-file also terminates the program.

## Run automated tests

Exit the program with `END` first. At the terminal's shell prompt (not the program's input prompt), run:

```sh
make test
```

The test runner builds the program and tests, checks the parser, and compares the command-line output against expected text. Successful output includes:

```text
Passed 66617 parser checks.
Passed 3 exact-output CLI checks.
```

A failed check reports an error and returns a nonzero exit status. Compiler commands may also appear in the output.

The parser tests cover every valid port, every octet value in each position, malformed tokens, numeric limits, leading zeros, long input, output reset on failure, and recovery after invalid candidates. The CLI tests cover the assignment's sample output, case-sensitive termination, and end-of-file.

## Manual test cases

Run `./ipv4` and enter each input separately:

| Input | Expected result |
| --- | --- |
| `connecting to 192.168.1.1 now` | Address `192.168.1.1`, decimal `3232235777`, port `none` |
| `192a168.1.1.1` | Address `168.1.1.1`, decimal `2818638081`, port `none` |
| `0.0.0.0:0` | Address `0.0.0.0`, decimal `0`, port `0` |
| `255.255.255.255:65535` | Address `255.255.255.255`, decimal `4294967295`, port `65535` |
| `192.168.1.1.` | Invalid input |
| `192.168.01.1` | Invalid input |
| `1.2.3.4:99999` | Invalid input |
| `1.2.3.4:` | Invalid input |
| `1.2.3.4:80:90` | Invalid input |
| `12.34.56` | Invalid input |
| `1.2.3.4. then 5.6.7.8` | Address `5.6.7.8`, decimal `84281096`, port `none` |

For invalid input, the exact message is:

```text
Invalid input: no valid IPv4 address found
```

## Parsing decisions

A candidate is a complete, uninterrupted run of ASCII digits, periods, and colons. Every other character separates candidates. The parser rejects malformed candidates in full instead of accepting a smaller piece inside them. Numeric values are accumulated by hand.

If a line contains multiple valid candidates, the function returns the first one. This is the implementation's interpretation of extracting a single address per line; the assignment does not explicitly specify that case.

## Files

- `main.cpp`: input loop and display formatting
- `ipv4.h`: required extraction function declaration
- `ipv4.cpp`: scanning and validation implementation
- `tests/test_ipv4.cpp`: parser tests
- `tests/test_cli.py`: exact-output tests
- `Makefile`: build and test commands

The compiled `ipv4` and `test_ipv4` files are generated locally and are excluded by `.gitignore`.
