# Mission 9: Too Much Power (Capabilities & Privilege Separation)

**Role:** Role 3 — Access Control & Privilege Engineer
**Syllabus Connection:** Least privilege, capabilities, privilege separation, attack surface reduction
**Concepts:** Linux capabilities (`capsh`, `getcap`), broad vs. narrow privilege, privilege separation, least privilege

## 1. Broad Privilege vs. Narrow Privilege Diagram

```
+-----------------------------------------------------------------------+
|                       BROAD PRIVILEGE (Full Root)                     |
|                                                                       |
|   [ Process / User with root or sudo (ALL : ALL) ALL ]                |
|                             |                                         |
|                             v                                         |
|   Full kernel & system-wide authority (all capabilities at once)      |
|   - Can modify users, kernel modules, raw sockets, files, logs, etc.  |
|   - If compromised -> Full system takeover (blast radius is huge)     |
+-----------------------------------------------------------------------+
                                  vs.
+-----------------------------------------------------------------------+
|                NARROW PRIVILEGE (Capabilities & Separation)           |
|                                                                       |
|   +-------------------------------+   +-----------------------------+ |
|   | Scoped File Capability        |   | Privilege-Separated Service | |
|   | e.g., /usr/bin/ping           |   | e.g., Broker (privileged)   | |
|   | Only: cap_net_raw=ep          |   |       Worker (unprivileged) | |
|   +---------------+---------------+   +--------------+--------------+ |
|                   |                                  |                |
|                   v                                  v                |
|   Only raw network socket access;   Worker cannot touch sensitive     |
|   cannot alter unrelated files or   system resources even if popped;  |
|   escalate to arbitrary root.       blast radius stays contained.     |
+-----------------------------------------------------------------------+

```

## 2. "Think About It" Questions & Answers

### Question 25: Why is giving every program root privileges dangerous?

* **Answer:** Giving every program full `root` privileges violates the **principle of least privilege** and creates an unnecessarily large **attack surface** and **blast radius**. A single vulnerability—such as a memory corruption bug, command injection flaw, or compromised dependency—in even a minor or network-facing program running as `root` gives an attacker full control over the entire machine, including the ability to read sensitive secrets, alter system binaries or logs, load kernel modules, and persist across reboots.

### Question 26: What is the idea behind a capability?

* **Answer:** Linux **capabilities** break the traditional all-or-nothing power of `root` into distinct, fine-grained units of privilege (for example, `cap_net_raw` for raw network sockets, `cap_chown` for changing file ownership, or `cap_setuid` for changing UIDs). Instead of granting a binary or process blanket superuser authority over everything on the system, a capability grants only the specific privileged operation that component actually needs to perform its job.

### Question 27: How can privilege separation reduce the impact of a compromised component?

* **Answer:** **Privilege separation** splits an application or workflow into smaller components with distinct trust boundaries and minimal privileges—for example, keeping parsing or network-handling code in a low-privilege (`nobody` or dedicated service) process while isolating sensitive or privileged operations in a small, tightly controlled component. If a parsing or network-facing component is compromised, the attacker only lands in a restricted context and still has to bypass the privilege boundary to reach sensitive data or full system control, containing the damage.

## 3. SECURELAB Evidence Template

* **Question:** How does Linux represent non-root/current process capabilities, existing file capabilities, and privilege boundaries compared to broad `root` / `sudo` access?

* **Prediction:** Running `id` and `sudo -l` will show user `monish` (`uid=1000`) with `sudo` access (`(ALL : ALL) ALL`), `capsh --print | head -40` will show an empty current capability set (`Current: =`) for the unprivileged shell alongside the system bounding set, and `getcap -r /usr/bin 2>/dev/null | head -20` will list specific binaries assigned targeted capabilities (such as `/usr/bin/ping cap_net_raw=ep`).

* **Experiment:**

  ```
  sudo apt update
  sudo apt install -y libcap2-bin
  id
  sudo -l
  capsh --print | head -40
  getcap -r /usr/bin 2>/dev/null | head -20
  
  ```

* **Observation:**

  * `id` showed `uid=1000(monish)`, `gid=1000(monish)`, and membership in `sudo` (`group 27`).

  * `sudo -l` showed `User monish may run the following commands on DESKTOP-QM2E9LB: (ALL : ALL) ALL`.

  * `capsh --print | head -40` showed `Current: =` (no active capabilities in the current unprivileged process) while listing the full `Bounding set` of kernel capabilities.

  * `getcap -r /usr/bin 2>/dev/null | head -20` showed `/usr/bin/ping cap_net_raw=ep`.

* **Concept:** **Capabilities & Privilege Separation** — decomposing superuser privileges into scoped capabilities and isolating components so no single component holds unnecessary system-wide power.

* **Defense:** Enforcing **Least Privilege** via scoped capabilities and privilege separation rather than running services or scripts as full `root` or granting unrestricted `sudo` rules.

* **Verification:** Inspected runtime identity (`id`), `sudo` policy (`sudo -l`), current/bounding capability sets (`capsh --print`), and file capability assignments (`getcap`) without modifying any system binaries.

* **Reflection:** Broad privilege (`root` or `(ALL : ALL) ALL` in `sudo`) makes lateral movement and full host compromise trivial once any privileged process or `sudo`-enabled account is abused; scoping privileges with capabilities and splitting duties across privilege boundaries shrinks what an attacker can reach after an initial foothold.