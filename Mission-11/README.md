# Mission 11 — Why Didn't the Attack Work?

## Objective

Inspect executable security properties and understand how compiler and operating-system protections help prevent memory attacks.

## Concepts

- NX / DEP
- Address Space Layout Randomization (ASLR)
- Stack canaries
- Position Independent Executables (PIE)
- Defense in depth

## Experiment

### 1. Install checksec

Installed the `checksec` utility to inspect executable security properties.

```bash
sudo apt install -y checksec
```

Installation result: Already at the newest version.

### 2. Build the protected executable

Compiled the buffer overflow demonstration with stack protection and PIE enabled.

```bash
gcc -Wall -Wextra -O0 -fstack-protector-strong -fPIE -pie memory_overflow_demo.c -o memory_protected
```

GCC issued a warning because the demonstration attempts to copy 34 bytes into a 16-byte buffer. This is the intentional unsafe operation from Mission 10.

### 3. Inspect executable protections

Command:

```bash
checksec --file=./memory_protected
```

Observed results:

| Protection | Result |
|---|---|
| RELRO | Full RELRO |
| Stack Canary | Canary found |
| NX | Enabled |
| PIE | Enabled |
| RPATH | No RPATH |
| RUNPATH | No RUNPATH |

### 4. Inspect ASLR

Command:

```bash
cat /proc/sys/kernel/randomize_va_space
```

Observed output:

```text
2
```

Interpretation: Full Address Space Layout Randomization is enabled.

### 5. Inspect stack permissions

Command:

```bash
readelf -W -l ./memory_protected | grep GNU_STACK
```

Observed output:

```text
GNU_STACK ... RW
```

Interpretation: The stack is readable and writable but does not have execute permission. This confirms NX protection for the executable's stack.

## Understanding the Results

| Defense | Threat it helps address |
|---|---|
| ASLR | Makes memory addresses harder to predict |
| NX / DEP | Prevents execution of instructions from non-executable memory regions |
| Stack Canary | Helps detect stack corruption, particularly overwriting the stack guard |
| PIE | Allows the executable's code to be loaded at randomized addresses |
| Full RELRO | Protects relocation-related memory from modification after relocation |

These protections work together as defense in depth.

## Think About It

### 31. What does ASLR make harder to predict?

ASLR makes the memory locations of important process regions, such as the stack, heap and shared libraries, harder to predict. This makes attacks that depend on known memory addresses more difficult.

### 32. What does NX prevent a writable memory region from being used for?

NX prevents a writable but non-executable memory region from also being used to execute instructions. For example, it prevents code stored on a non-executable stack from being executed.

### 33. What does a stack canary attempt to detect?

A stack canary attempts to detect stack buffer overflows that overwrite the protected stack guard value. The program can detect the changed value before returning from the affected function and terminate rather than continue with corrupted stack state.

### 34. Why is defense in depth better than relying on one protection?

Each protection addresses different risks and has limitations. ASLR makes addresses unpredictable, NX prevents execution from non-executable memory, and stack canaries help detect certain forms of stack corruption. Using multiple protections provides additional layers of security if one protection is bypassed or insufficient.

## Conclusion

The protected executable was inspected using checksec and readelf. The results confirmed Full RELRO, stack canaries, NX and PIE. The Linux ASLR setting was 2, indicating full address-space randomization.

The experiment demonstrated how compiler and operating-system protections work together to make memory-related attacks more difficult.

## Experiment Scope

The protected build and operating-system security settings were inspected. The additional build with selected protections disabled was not performed.

## Evidence

- checksec output for `memory_protected`.
- ASLR configuration output.
- GNU_STACK permissions output.
- Explanation linking security defenses to the threats they address.
