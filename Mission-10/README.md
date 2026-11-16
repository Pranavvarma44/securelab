# Mission 10 — The Dangerous Array

## Objective

Understand how an unchecked copy can corrupt memory, while keeping the experiment inside the VM and avoiding exploit development.

## Concepts

- Stack memory
- Buffer overflow
- Bounds checking
- Undefined behavior

## Experiment

Created and compiled `memory_overflow_demo.c` to demonstrate the consequences of copying data beyond a fixed-size buffer.

The program declares:

```c
char buffer[16];
```

It then copies a string into the buffer using `memcpy()`.

### Compilation

```bash
gcc -Wall -Wextra -O0 -g memory_overflow_demo.c -o memory_overflow_demo
```

## Test 1 — Short Input

Input:

```c
const char input[] = "HELLO";
```

Observed output:

```text
Copying 6 bytes into a 16-byte buffer...
Program reached the end of the copy.
```

Analysis:

- Buffer capacity: 16 bytes.
- Input characters: 5.
- Null terminator: 1 byte.
- Total copied: 6 bytes.

The input fits within the buffer, so no buffer overflow occurs in this test.

## Test 2 — Longer Input

Input:

```c
const char input[] = "THIS_INPUT_IS_LONGER_THAN_SIXTEEN";
```

### Compiler Observation

GCC issued a warning indicating that `memcpy()` was writing 34 bytes into a destination region of only 16 bytes.

### Runtime Output

```text
Copying 34 bytes into a 16-byte buffer...
Program reached the end of the copy.
```

### Analysis

| Property | Observed value |
|---|---:|
| Buffer capacity | 16 bytes |
| Input characters | 33 |
| Null terminator | 1 byte |
| Total bytes copied | 34 |
| Bytes beyond buffer | 18 |

The program completed without crashing. However, this does not mean the operation was safe. Writing beyond the array boundary causes undefined behavior in C.

## Evidence — Buffer Boundary Diagram

```text
Total bytes copied: 34

+----------------------+------------------------+
|  Intended buffer     |   Beyond buffer        |
|      16 bytes        |      18 bytes          |
+----------------------+------------------------+
|                      |                        |
|   Valid capacity     |   Out-of-bounds write  |
|                      |                        |
+----------------------+------------------------+
```

## Think About It

### Q28. What does it mean to write beyond the end of a buffer?

It means attempting to store more data than the allocated capacity of an array or buffer. The additional data is written outside the intended memory boundary, potentially overwriting adjacent memory.

### Q29. Why is memory corruption a security problem?

Memory corruption can modify data or program state that should not have been changed. This can cause crashes, incorrect program behavior, data corruption and potentially security vulnerabilities.

### Q30. Why is a bounds-checked operation safer?

A bounds-checked operation verifies that the amount of data being copied does not exceed the destination buffer's capacity. This prevents out-of-bounds writes and reduces the risk of memory corruption.

## Conclusion

This experiment demonstrated how an unchecked memory copy can write beyond a buffer's allocated capacity. GCC detected the problem and issued a compiler warning. However, the program still completed at runtime, demonstrating that an overflow does not necessarily cause an immediate crash.

The experiment highlights the importance of bounds checking and secure memory management in C programming.

## Safety Note

This experiment was performed only inside the SECURELAB Ubuntu virtual machine for educational observation. No exploit payloads or attacks against other systems were developed.
