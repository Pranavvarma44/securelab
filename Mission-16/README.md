# Mission 16: Build Your First Container Sandbox

## 1. Objective

Install Docker and run a minimal Ubuntu container to understand containerization and resource isolation.

**Syllabus connection:** Virtualization, containers, namespaces, cgroups.

## 2. Environment

| Component | Details |
|---|---|
| Physical machine | MacBook (Apple Silicon) |
| Virtualization software | UTM / QEMU |
| Host operating system | Ubuntu ARM64 |
| Host architecture | aarch64 |
| Linux kernel | 7.0.0-43-generic |
| Container runtime | Docker Engine |
| Container image | Ubuntu 26.04 |

## 3. Docker Installation and Verification

Docker Engine was installed inside the Ubuntu virtual machine using the official Docker documentation.

Installation was verified using:

```bash
sudo docker run --rm hello-world
```

The command successfully downloaded and executed the Hello World image, confirming that Docker was working correctly.

## 4. Experiment: Running an Ubuntu Container

The official Ubuntu 26.04 image was used to execute the following commands.

### A. Check system and kernel information

```bash
sudo docker run --rm ubuntu:26.04 uname -a
```

**Observed output:**

```text
Linux 9861fe2690d7 7.0.0-43-generic
#43-Ubuntu SMP PREEMPT_DYNAMIC
Fri Oct 23 13:33:03 UTC 2026
aarch64 GNU/Linux
```

### B. Check Ubuntu version

```bash
sudo docker run --rm ubuntu:26.04 cat /etc/os-release
```

**Observed output:**

```text
PRETTY_NAME="Ubuntu 26.04.1 LTS"
NAME="Ubuntu"
VERSION_ID="26.04"
VERSION="26.04.1 LTS (Resolute Raccoon)"
VERSION_CODENAME=resolute
ID=ubuntu
```

### C. Inspect running processes

```bash
sudo docker run --rm ubuntu:26.04 ps
```

**Observed output:**

```text
PID TTY          TIME CMD
  1 ?        00:00:00 ps
```

The container displayed only its own process view. The `ps` command appeared with PID 1.

## 5. Host vs Container Architecture

The Ubuntu virtual machine acts as the Docker host. Docker creates and manages containers within this environment.

```text
+--------------------------------------+
|       MacBook (Apple Silicon)        |
|            Physical Machine          |
+--------------------------------------+
                  |
                  v
+--------------------------------------+
|             UTM / QEMU               |
|                                      |
|       Ubuntu ARM64 Virtual Machine   |
|                                      |
|       Linux Kernel 7.0.0-43-generic  |
|       Architecture: aarch64          |
|                                      |
|          +------------------+        |
|          |   Docker Engine  |        |
|          +------------------+        |
|                   |                  |
|                   v                  |
|          +------------------+        |
|          | Ubuntu Container |        |
|          |                  |        |
|          | Ubuntu 26.04.1   |        |
|          | aarch64          |        |
|          | Isolated process |        |
|          | environment      |        |
|          +------------------+        |
|                                      |
+--------------------------------------+
```

### Observations

- Both the host and container use the `aarch64` architecture.
- Both report Linux kernel version `7.0.0-43-generic`.
- The container has its own isolated process environment.
- The container shares the host's Linux kernel instead of running a separate kernel.

## 6. Virtual Machine vs Container

| Feature | Virtual Machine | Container |
|---|---|---|
| Virtualization | Hardware-level virtualization | OS-level virtualization |
| Operating system | Runs a guest OS | Uses an isolated user-space environment |
| Kernel | Has its own guest kernel | Shares the host kernel |
| Resource usage | Generally higher | Generally lower |
| Startup | Generally slower | Generally faster |
| Isolation | Virtual hardware and OS isolation | Process and resource isolation |

A virtual machine provides an independent operating system environment, whereas a container shares the host kernel and isolates applications using operating-system-level mechanisms.

## 7. Think About It

### Q47. Why does the container have its own process view?

A container uses PID namespaces to isolate its process IDs and process list from the host system. This allows processes inside the container to have their own process view without exposing the complete host process list.

### Q48. Is a container the same thing as a virtual machine?

No. A virtual machine runs a separate guest operating system with its own kernel. A container shares the host's kernel while providing an isolated environment for applications and processes.

### Q49. Which resources can cgroups control?

Control groups (cgroups) can account for and control resource usage, including:

- CPU usage
- Memory usage
- Block I/O
- Process-related resource consumption

They help prevent applications from consuming excessive system resources.

## 8. Conclusion

Docker was successfully installed and verified inside the Ubuntu ARM64 virtual machine.

A minimal Ubuntu 26.04 container was executed successfully. The experiment demonstrated the container's Linux environment, architecture, kernel information and isolated process view.

The experiment helped establish the difference between virtual machines and containers and introduced the role of namespaces and cgroups in container isolation.

## 9. Evidence

The following evidence is required for this mission:

| Evidence | File |
|---|---|
| Container output (all three commands) | `screenshots/container-output.png` |
| Host vs container architecture diagram | Architecture diagram in this README |
| VM vs container comparison | Section 6 of this README |

