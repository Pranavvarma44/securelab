# Mission 6: Two Processes Need to Talk

## Objective

Compare IPC mechanisms and identify the security properties of the communication endpoint.

## Concepts

- Inter-Process Communication (IPC)
- Pipe / message passing
- Shared memory
- Permissions
- Trust boundary
- Synchronization

## Experiment 1: Pipe-based IPC

The pipe program creates a pipe and then creates a child process with `fork()`.

- `pipe(fd)` creates a communication channel.
- `fd[0]` is the read end and `fd[1]` is the write end.
- The child closes the write end and reads from the pipe.
- The parent closes the read end and writes a message.
- `waitpid()` makes the parent wait for the child to finish.

Compile:

```bash
gcc -Wall -Wextra pipe_ipc.c -o pipe_ipc
```

Run:

```bash
./pipe_ipc
```

Expected output:

```text
Child received: Hello from the parent
```

## Experiment 2: Shared-memory demonstration

The handout says to compare the pipe program with the shared-memory design supplied by the instructor. The handout itself includes the pipe starter code, but does not include a separate shared-memory implementation. For this project, a POSIX shared-memory demonstration was created in `shared_memory.c` to make the comparison practical.

The demonstration uses:

- `shm_open()` to create/open a POSIX shared-memory object.
- `ftruncate()` to set its size.
- `mmap()` to map the object into the process address space.
- `strcpy()` to write the message into the mapped region.
- `fork()` to create the child process.
- `munmap()` and `shm_unlink()` to clean up.

Compile:

```bash
gcc -Wall -Wextra shared_memory.c -o shared_memory -lrt
```

Run:

```bash
./shared_memory
```

Expected output:

```text
Parent sent: Hello through shared memory
Child received: Hello through shared memory
```

**Implementation note:** In this demonstration, the parent writes the message before calling `fork()`. The child then reads the already-written data from the inherited shared mapping. This demonstrates that both processes can access the shared region. It does not demonstrate simultaneous read/write access or a synchronization mechanism. In a program where processes access or modify shared data concurrently, synchronization (for example, a process-shared mutex or semaphore) is needed to coordinate access.

## Pipe vs Shared Memory

| Feature | Pipe | Shared memory |
|---|---|---|
| Communication model | Message passing / byte stream | Processes access a common memory region |
| Data access | Sender writes and receiver reads | Processes read/write the mapped region |
| Typical use | Simple communication between related processes | Efficient exchange of larger amounts of data |
| Synchronization | The pipe provides the communication channel; application protocol may still need coordination | Separate synchronization is needed when processes concurrently access shared data |
| Access control | Protect access to the pipe/file descriptors and related process boundary | Protect shared-memory object permissions and access to the mapped region |

A pipe transfers bytes through a kernel-managed channel. Shared memory lets processes access the same mapped memory region, avoiding repeated copying through a pipe interface. Shared memory can be efficient for larger data exchanges, but concurrent access must be coordinated.

## Proposed Secure-IPC Control

**Control: Access control on IPC endpoints**

Restrict access to IPC resources so that only authorized processes can use them.

Possible measures:

1. Set restrictive permissions when creating shared-memory objects. In this demonstration, `shm_open()` uses mode `0600`, which grants read/write access to the owner only, subject to the process umask and system permissions.
2. Ensure only intended processes receive or retain access to IPC file descriptors and shared-memory objects.
3. Validate data received from another process before using it.
4. Use appropriate synchronization when multiple processes access shared data concurrently.

The security objective is to protect both the communicated data and the IPC endpoint from unauthorized access or modification.

## Theory Questions

### Q16. Why is a pipe different from shared memory?

A pipe is a message-passing mechanism: one process writes bytes to the pipe and another process reads them. Shared memory maps a common memory region into multiple processes, allowing them to access the same data directly. A pipe provides a communication channel, while shared memory provides a shared data area.

### Q17. Why does shared memory normally require synchronization?

Multiple processes may read or write the same shared memory region at the same time. Without coordination, their operations can overlap and produce race conditions, lost updates, or inconsistent data. A process-shared mutex or semaphore can coordinate access when concurrent operations need protection.

### Q18. What must be protected in secure IPC: the data, the endpoint, or both?

Both must be protected. The data must be protected against unauthorized reading or modification. The endpoint (such as a pipe or shared-memory object) must be protected so that only authorized processes can access or use it. Access permissions and appropriate validation help enforce these protections.

## Evidence to Submit

The handout asks for:

1. Working IPC output.
2. A message-passing vs shared-memory comparison.
3. One proposed secure-IPC control.

Additional evidence for the shared-memory demonstration:

- `Mission6_Shared_Memory_Output.png`

Suggested screenshot filenames:

- `Mission6_Pipe_Output.png`
- `Mission6_IPC_Comparison.png`
- `Mission6_Secure_IPC_Control.png`
- `Mission6_Shared_Memory_Output.png`

## Files

- `pipe_ipc.c` — pipe-based IPC source code.
- `pipe_ipc` — compiled pipe executable.
- `shared_memory.c` — POSIX shared-memory demonstration source code.
- `shared_memory` — compiled shared-memory executable.
- `mission6_comparison.txt` — comparison notes.
- `mission6_security.txt` — proposed secure-IPC control.
