
# Mission 3 — Process Detective

## Objective
Investigate process metadata and build a small process-security inspector using Linux `/proc`.

## Concepts
- PID and PPID
- UID and GID
- Threads
- CPU and memory usage
- `/proc` filesystem

## Experiment

Created `proc_inspector.sh` to inspect a process started by the current user.

### Execution

```bash
sleep 300 &
TARGET_PID=$!
./proc_inspector.sh "$TARGET_PID"
```

### Information collected

- Process ID and command
- Process state and parent PID
- User and group IDs
- Thread count
- Virtual memory size and resident memory
- Executable path
- Open file descriptor count

## Evidence 1 — Working Script

Source: `proc_inspector.sh`

The script reads process information from `/proc/PID/` and displays relevant metadata.

## Evidence 2 — Open File Descriptor Count

The script uses:

```bash
ls "/proc/$PID/fd" | wc -l
```

This counts the open file descriptors associated with the process.


### Observed Results

Test process PID: 30279

| File Descriptor | Target |
|---|---|
| 0 | /dev/pts/0 |
| 1 | /dev/pts/0 |
| 2 | /dev/pts/0 |

Total open file descriptors: **3**

The three descriptors correspond to standard input, standard output and standard error.

## Think About It

### Q7. What security-relevant information does `/proc/PID/status` reveal?

It reveals process identity (UID/GID), parent process ID, execution state, number of threads and memory usage. This helps identify suspicious processes and understand their privileges and resource consumption.

### Q8. Why can resource consumption itself be a security concern?

Excessive CPU or memory consumption can slow down or crash a system, potentially causing a Denial-of-Service (DoS) condition.

### Q9. Why is the number of threads useful when investigating a process?

An unexpectedly high number of threads may indicate unusual activity, resource abuse or suspicious behaviour.

## How `/proc` Helps Investigation

The `/proc` filesystem exposes information about running processes, including their credentials, memory usage, execution state and open resources. Security investigators can inspect this information to identify unusual process behaviour and investigate potential security incidents.

## Conclusion

The experiment demonstrated how Linux exposes process metadata through `/proc` and how a Bash script can collect useful information for process-security investigation.
