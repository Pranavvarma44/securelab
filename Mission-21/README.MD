# Mission 21: Who Protects the VM?
**Course:** Secure Operating Systems (CS5101)  
**Syllabus Connection:** Virtualization, Hypervisors, Trusted Computing Base (TCB), Trusted Execution Concepts  
**Objective:** Map the trust boundaries from user-space application to physical hardware.

---

## Architecture & Trust Boundary Stack

The diagram below illustrates the execution layers and trust boundaries in a Type-2 (hosted) virtualization setup (such as running a VM on macOS):

```text
+-------------------------------------------------------------------+

|  Application (User Space in VM)                                   |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+  <-- Guest OS Internal

|  Guest OS Kernel (e.g., Linux / Windows)                          |      Trust Boundary (TCB)
+-------------------------------------------------------------------+
=====================================================================  === Virtualization Boundary
+-------------------------------------------------------------------+

|  Virtual Hardware (vCPU, vRAM, vDisk, vNIC)                       |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+

|  Hypervisor / VMM (e.g., Apple Hypervisor.framework / QEMU)       |
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+  <-- Underlying Host /

|  Host OS Kernel (macOS XNU Kernel)                                |      VMM Trust Boundary
+-------------------------------------------------------------------+
                                  |
                                  v
+-------------------------------------------------------------------+

|  Physical Hardware (CPU, RAM, SSD, NIC)                           |
+-------------------------------------------------------------------+
```

> **Note:** In a Type-1 (bare-metal) hypervisor setup (e.g., VMware ESXi, Xen), the **Host OS Kernel** layer is omitted and the **Hypervisor** runs directly on top of the **Physical Hardware**.

---

## Reflection Questions (Think About It)

### 65. Which components are inside the guest OS trust boundary?
Inside the guest OS's internal trust boundary (its own Trusted Computing Base, or TCB), the **guest kernel**, **privileged system services/daemons**, and **guest device drivers** are trusted to enforce isolation between user-space applications running inside that VM. However, because a VM cannot execute independently, the guest OS must also implicitly trust everything beneath it—specifically the **virtual hardware**, the **hypervisor**, the **host OS** (in a Type-2 setup), and the **physical hardware**.

### 66. Which component controls the virtual hardware presented to the guest?
The **hypervisor** (Virtual Machine Monitor / VMM)—often working alongside a **device emulator** (such as QEMU)—controls and presents the virtual hardware (vCPU, virtual RAM, virtual NICs, virtual disks) to the guest OS.

### 67. Why does virtualization create another layer of security dependencies?
In a traditional non-virtualized machine, an application only depends on the host OS kernel and physical hardware for security and isolation. Virtualization inserts an additional software stack—the **hypervisor** (and potentially a **host OS**)—between the guest OS and the physical hardware. Because this lower layer operates at a higher privilege level (e.g., Ring -1 or root mode), any vulnerability, misconfiguration, or compromise in the hypervisor or host OS breaks the isolation of every guest VM running on top of it.

---

## VM Additional Attack Surface Analysis

Introducing virtualization significantly expands a system's attack surface by adding complex software layers—specifically the hypervisor, virtual device emulators, and (in Type-2 setups) a full host operating system—directly beneath the guest OS. Each emulated hardware interface (such as virtual network cards, storage controllers, and graphics adapters) acts as a potential entry point where a malicious guest could exploit parsing bugs to trigger a **VM escape**, gaining arbitrary code execution on the host. Furthermore, sharing physical hardware resources across multiple virtual machines exposes the guest to **hardware side-channel attacks** (such as cache-timing or speculative execution leaks) from co-resident VMs, while virtual management interfaces and snapshots introduce new administrative and data-exposure risks outside the guest OS's visibility or control.
