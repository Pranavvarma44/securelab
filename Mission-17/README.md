# Mission 17: Linux Says No

## 1. Objective

Observe mandatory access control (MAC) policy enforcement using AppArmor and inspect loaded security profiles.

**Syllabus connection:** AppArmor, MAC, application sandboxing.

## 2. Environment

| Component | Details |
|---|---|
| Operating System | Ubuntu 26.04.1 LTS |
| Architecture | ARM64 (aarch64) |
| Kernel | Linux 7.0.0-43-generic |
| Security Framework | AppArmor |
| AppArmor Version | 5.0.2 |

## 3. Installation

Installed the required AppArmor packages:

```bash
sudo apt install -y apparmor apparmor-utils
```

AppArmor was already installed on the system. The installation successfully added the AppArmor utilities and their dependencies.

## 4. AppArmor Status Verification

Executed:

```bash
sudo aa-status
```

**Observed results:**

| Status | Count |
|---|---:|
| Loaded profiles | 229 |
| Enforce mode | 151 |
| Complain mode | 3 |
| Prompt mode | 0 |
| Kill mode | 0 |
| Unconfined mode | 75 |
| Processes in enforce mode | 9 |

### Interpretation

- **Enforce mode:** AppArmor actively enforces the security rules and blocks unauthorized operations.
- **Complain mode:** Policy violations are logged but are not blocked by the profile.
- **Unconfined mode:** Applications are not restricted by an enforcing AppArmor profile.
- **Prompt mode:** No profiles were in this mode.
- **Kill mode:** No profiles were in this mode.

The results confirm that AppArmor is loaded and actively enforcing security policies on the system.

## 5. Inspecting an AppArmor Profile

Inspected the CUPS printing service profile:

```bash
sudo cat /etc/apparmor.d/usr.sbin.cupsd
```

The profile applies security restrictions to the `/usr/sbin/cupsd` printing service.

### Examples of profile rules

| Profile rule | Purpose |
|---|---|
| `/etc/cups/** rw` | Allows reading and writing CUPS configuration files |
| `/usr/share/** r` | Allows reading shared system files |
| `/var/log/cups/** rw` | Allows access to CUPS log files |
| `/dev/lp* rw` | Allows access to printer devices |
| `deny capability block_suspend` | Explicitly denies the block-suspend capability |
| `deny /dev/tty rw` | Denies read/write access to the terminal device |
| `deny /etc/krb5.conf w` | Prevents writing to the Kerberos configuration |

The profile also defines a `third_party` subprofile for certain third-party printer backends, filters and drivers.

### Interpretation

The CUPS profile demonstrates how AppArmor restricts an application's access to system resources.

Instead of allowing unrestricted access, the profile specifies which files, directories, capabilities and operations the application can use.

The profile also contains explicit deny rules for operations that should not be permitted.

## 6. Checking Unconfined Processes

Executed:

```bash
sudo aa-unconfined 2>/dev/null
```

**Observed output included:**

```text
/usr/lib/systemd/systemd-resolved not confined
/usr/sbin/avahi-daemon not confined
/usr/sbin/chronyd confined by '/usr/sbin/chronyd (enforce)'
/usr/bin/python3.14 confined by '/usr/bin/wsdd (enforce)'
/usr/sbin/cupsd confined by '/usr/sbin/cupsd (enforce)'
```

This demonstrates that some processes run without an enforcing profile, while others operate under AppArmor restrictions.

## 7. Think About It

### Q50. What is AppArmor controlling?

AppArmor controls the resources and operations an application can access. These include files, directories, capabilities, network resources and interactions with other processes.

### Q51. How is policy enforcement different from ordinary Unix ownership?

Ordinary Unix permissions control access based on user ownership, groups and permission bits.

AppArmor provides an additional application-specific security layer through profiles. These profiles restrict application behavior independently of ordinary file ownership and permissions.

### Q52. Why can MAC provide defense in depth?

Mandatory Access Control adds another layer of security beyond traditional Unix permissions.

Even if an application is compromised, AppArmor can restrict its access to sensitive files and system resources, reducing the potential damage.

## 8. Evidence

The following evidence is included:

| Evidence | Screenshot |
|---|---|
| AppArmor status | `screenshots/apparmor-status.png` |
| CUPS profile explanation | `screenshots/cupsd-profile.png` |

**Note:** No instructor-provided denial demonstration was available, so an allowed-versus-denied behavior experiment was not performed.

## 9. Conclusion

AppArmor was successfully verified as active on the Ubuntu virtual machine.

The experiment demonstrated mandatory access control through loaded security profiles, including enforcing and complain modes.

Inspection of the CUPS profile showed how application-specific policies define permitted and denied operations.

This mission demonstrated how AppArmor provides an additional layer of security through mandatory access control and application sandboxing.
