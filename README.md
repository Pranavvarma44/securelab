# SECURELAB — Secure Operating Systems Laboratory

## Project Overview

SECURELAB is a hands-on Secure Operating Systems project focused on understanding Linux security mechanisms, process and memory management, system hardening, and practical security experiments.

The project is implemented and tested using an Ubuntu Linux virtual machine.

## Environment

- **Operating System:** Ubuntu Linux (ARM64)
- **Virtualization:** UTM
- **Architecture:** AArch64
- **Programming Languages:** C, Bash
- **Tools:** GCC, GDB, strace, checksec, readelf, Git

## Team Responsibilities

| Member | Assigned Missions |
|---|---|
| Member 1 — Process & Memory Security | 3, 4, 10, 11 |
| Member 2 | 5, 6 |
| Member 3 | 7, 8, 9 |
| Member 4 | 12–17 |
| Member 5 | 18–21 and integration coordination |

Missions 1 and 2 are introductory/shared exercises.

## Project Progress

### Completed Missions

| Mission | Topic | Status |
|---|---|---|
| Mission 1 | User and Group Identity | Completed |
| Mission 2 | System Calls and Process Tracing | Completed |
| Mission 3 | Process Detective | Completed |
| Mission 4 | Process Memory Isolation | Completed |
| Mission 10 | The Dangerous Array | Completed |

### In Progress

| Mission | Topic | Status |
|---|---|---|
| Mission 11 | Memory Protection Mechanisms | Core checks completed; comparison experiment pending |

### Other Team Missions

Missions 5–9 and 12–21 are assigned to the other team members and will be integrated into the shared repository as work progresses.

### Final Integration

- Mission 22 — Final Challenge
- Integration of security controls
- Final hardened Linux VM
- Common evidence repository
- Project report and change log

## Repository Structure

```text
SECURELAB/
│
├── README.md
│
├── Mission-01/
│   ├── README.md
│   └── identity.c
│
├── Mission-02/
│   └── README.md
│
├── Mission-03/
│   ├── README.md
│   ├── proc_inspector.sh
│   └── screenshots/
│
├── Mission-04/
│   ├── README.md
│   ├── address_space.c
│   └── screenshots/
│
├── Mission-10/
│   ├── README.md
│   ├── memory_overflow_demo.c
│   └── screenshots/
│
└── Mission-11/
    ├── README.md
    ├── memory_overflow_demo.c
    ├── memory_protected
    └── screenshots/
```

*The structure will expand as additional missions are completed and integrated.*

## Mission Documentation

Each mission has its own directory containing:

- A dedicated README with objectives, experiment details, observations and theory answers.
- Source code and scripts used in the experiment.
- Screenshots and other supporting evidence.

## Member 1 — Work Summary

The following areas have been investigated:

1. **User and Group Identity:** Linux user IDs, group IDs and account information.
2. **System Call Tracing:** Observing file operations using `strace`.
3. **Process Investigation:** Inspecting process metadata through the `/proc` filesystem.
4. **Process Memory Isolation:** Examining virtual memory mappings and process address spaces.
5. **Buffer Overflow:** Demonstrating the consequences of copying data beyond a buffer's capacity.
6. **Memory Protections:** Inspecting ASLR, NX, stack canaries, PIE and RELRO.

## Version Control

The project uses Git and GitHub for source-code management, documentation and collaboration.

Each mission is maintained independently so that team members can contribute without mixing their experiment files and documentation.

---

**Project:** SECURELAB  
**Course:** Secure Operating Systems  
**Platform:** Ubuntu Linux

# SECURELAB — Secure Operating Systems

A collaborative Linux security project covering process security, memory protection, vulnerabilities, access control, and system hardening.

## Environment
- OS: Ubuntu Linux (ARM64)
- Virtualization: UTM
- Architecture: AArch64

## Team Structure

| Member | Assigned Missions |
|---|---|
| Member 1 | 3, 4, 10, 11 |
| Member 2 | 5, 6 |
| Member 3 | 7, 8, 9 |
| Member 4 | 12–17 |
| Member 5 | 18–21 |

## Mission Progress

| Mission | Status |
|---|---|
| 01 | Completed |
| 02–22 | Pending |

## Mission Documentation

- [Mission 01](Mission-01/README.md)

