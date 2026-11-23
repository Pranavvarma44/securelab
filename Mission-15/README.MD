# Mission 15: The Suspicious Application

## 1. Mission Objective

Identify what resources an application can access and decide what it actually needs.

### Concepts
- Sandboxing
- Application attack surface
- Least privilege
- Process resource inspection

## 2. Environment

- Operating System: Ubuntu ARM64
- Application inspected: `sleep 300`
- Process ID (PID): 5169
- Tools used: `ps`, `/proc`, `ls`, `ss`

## 3. Experiment

A harmless Linux utility was selected to inspect its process information, open file descriptors, resource usage and network activity.

### Step 1: Start the application

Command:

```bash
sleep 300 &
```

The process was started in the background.

Output:

```text
[1] 5169
```

The process ID was retrieved using:

```bash
echo $!
```

Output:

```text
5169
```

### Step 2: Inspect process information

Command:

```bash
ps -p 5169 -o pid,ppid,uid,euid,nlwp,%cpu,%mem,cmd
```

Actual output:

```text
PID    PPID   UID   EUID  NLWP  %CPU  %MEM  CMD
5169   3635   1000  1000  1     1.0   0.1   sleep 300
```

Observations:

- PID: 5169
- Parent PID: 3635
- UID: 1000
- Effective UID: 1000
- Number of threads: 1
- CPU usage: 1.0%
- Memory usage: 0.1%

The process was running under a normal user account rather than root.

### Step 3: Inspect open file descriptors

Command:

```bash
ls -l /proc/5169/fd
```

Observed file descriptors:

| File descriptor | Target |
|---|---|
| 0 | `/dev/pts/0` |
| 1 | `/dev/pts/0` |
| 2 | `/dev/pts/0` |

These represent:

- FD 0: Standard input
- FD 1: Standard output
- FD 2: Standard error

The process had three open file descriptors, all connected to the terminal.

### Step 4: Inspect process status

Command:

```bash
cat /proc/5169/status | grep -E 'Name|Uid|Gid|Threads'
```

Actual output:

```text
Name:   sleep
Uid:    1000    1000    1000    1000
Gid:    1000    1000    1000    1000
Threads:        1
```

This confirmed the process identity and thread count.

### Step 5: Inspect network activity

Command:

```bash
ss -lntup
```

The output displayed several UDP and TCP sockets belonging to other system processes, including a `python3` process with PID 4702.

No socket attributed to the inspected `sleep` process (PID 5169) was observed.

This indicates that the selected application did not have an associated network socket during the inspection.

## 4. Application Resource Table

| Resource | Observed usage | Requirement |
|---|---|---|
| CPU | 1.0% | Required |
| Memory | 0.1% | Required |
| Threads | 1 | Required |
| Terminal access | Three standard file descriptors | Only if interactive I/O is needed |
| Additional file access | None observed | Not required for basic operation |
| Network | No socket attributed to PID 5169 | Not required |
| Root privileges | UID 1000 | Not required |

The `sleep` utility only needs basic system resources to wait for the specified duration.

## 5. Proposed Sandbox Policy

The following restrictions are proposed according to the principle of least privilege.

| Resource | Proposed policy | Reason |
|---|---|---|
| Network access | Deny | `sleep` does not require network communication |
| Sensitive files | Deny access | The application does not need private system or user files |
| Filesystem writes | Restrict unnecessary writes | Prevent unintended modification of files |
| Root privileges | Deny privilege escalation | Normal user privileges are sufficient |
| Process creation | Restrict unnecessary child processes | The utility does not need to launch other applications |

### At Least Three Resources to Restrict

1. **Network access:** Prevent unnecessary communication with external systems.
2. **Sensitive file access:** Prevent access to confidential system and user data.
3. **Filesystem write permissions:** Prevent modification of files that are not required by the application.
4. **Root privileges:** Prevent unnecessary administrative access.

These are proposed sandbox restrictions, not restrictions enforced during this experiment.

## 6. Think About It

### Q44. Which resources does your chosen application actually need?

The `sleep` utility requires CPU time, a small amount of memory and access to system timing facilities to wait for the specified duration.

It does not require network access or access to sensitive files.

### Q45. Which resources could be denied without breaking its purpose?

Network access, access to sensitive files, unnecessary filesystem write permissions, root privileges and unnecessary process creation could be restricted without affecting its basic purpose.

### Q46. How does least privilege reduce attack surface?

Least privilege gives an application only the permissions and resources required to perform its task.

If an application is compromised, the attacker's access is limited. This reduces the possibility of unauthorized file access, system modification and other security damage.

## 7. Conclusion

This experiment demonstrated how Linux process information and the `/proc` filesystem can be used to inspect an application's resource usage.

The `sleep` process was inspected using `ps`, `/proc/5169/fd`, `/proc/5169/status` and `ss`.

The investigation showed that the application ran with normal user privileges, used one thread, had three terminal file descriptors and had no network socket attributed to it.

A sandbox policy was proposed to restrict unnecessary network access, sensitive file access, filesystem writes and administrative privileges.

The experiment demonstrated how the principle of least privilege can reduce an application's attack surface.
