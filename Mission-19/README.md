# Mission 19 – Harden the Machine

## Objective
Apply practical Linux system-hardening measures, inspect the system's security baseline, implement at least three hardening changes, and verify their effectiveness.

## Environment
- OS: Ubuntu 26.04.1 LTS (Resolute Raccoon)
- Architecture: ARM64 (aarch64)
- Kernel: `7.0.0-43-generic`
- Virtualization: UTM / QEMU

## 1. Initial Security Baseline

Before applying hardening changes, the system was inspected.

### Package updates
- `sudo apt update` initially reported 122 upgradable packages.
- The first upgrade attempt encountered 404 errors for SSSD packages.
- The Ubuntu APT repository was switched from `archive.ubuntu.com` to the official ARM64 ports mirror: `ports.ubuntu.com/ubuntu-ports`.
- Package updates subsequently completed successfully.
- `sudo dpkg --audit` returned no output.
- `apt list --upgradable` showed no remaining package updates.

### Running services
The system had 35 running services during the initial inspection.

Examples included:
- `NetworkManager`
- `docker`
- `containerd`
- `cups`
- `avahi-daemon`
- `cron`
- `sshd` was not observed in the initial listener inspection.

### Network listeners
The initial `ss -lntup` inspection showed:
- TCP loopback listeners for CUPS and the local DNS resolver.
- UDP listeners including mDNS and WSDD.
- No non-loopback TCP listener was observed in the initial inspection.

### Firewall status
Initially, UFW was inactive.

## 2. Hardening Change 1 – Apply Package Updates

Updated the Ubuntu package lists and installed available package upgrades.

Commands:

```bash
sudo apt update
sudo apt upgrade
```

The initial upgrade encountered missing SSSD package archives. After changing the Ubuntu repository to the official ARM64 ports mirror, the upgrade completed.

Verification:

```bash
sudo dpkg --audit
apt list --upgradable
```

Results:
- No incomplete package installations were reported.
- No remaining upgradable packages were listed.

## 3. Hardening Change 2 – Configure UFW Firewall

UFW was initially inactive. Default firewall policies were configured to restrict incoming connections while allowing outgoing connections.

Commands:

```bash
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw enable
```

Verification:

```bash
sudo ufw status verbose
```

Observed result:

- Status: active
- Logging: on (low)
- Default incoming policy: deny
- Default outgoing policy: allow
- Default routed policy: deny

This configuration blocks unsolicited incoming connections by default while allowing the machine to initiate outgoing connections.

## 4. Hardening Change 3 – Enable Automatic Security Updates

The `unattended-upgrades` package was already installed.

Commands:

```bash
sudo apt install unattended-upgrades
sudo dpkg-reconfigure -plow unattended-upgrades
```

Selected **Yes** when prompted to enable automatic updates.

Verification:

```bash
systemctl status unattended-upgrades --no-pager
cat /etc/apt/apt.conf.d/20auto-upgrades
```

Observed configuration:

```text
APT::Periodic::Update-Package-Lists "1";
APT::Periodic::Unattended-Upgrade "1";
```

The unattended-upgrades service was active and enabled.

This configuration allows the system to periodically refresh package lists and install unattended upgrades.

## 5. Final Verification

| Security measure | Final result |
|---|---|
| Package updates | Completed |
| Package database audit | No issues reported |
| Remaining package updates | None listed |
| UFW firewall | Active |
| Incoming traffic | Denied by default |
| Outgoing traffic | Allowed by default |
| Automatic updates | Enabled |
| Unattended-upgrades service | Active and enabled |
| Reboot requirement | No reboot required |

## 6. Conclusion

Three practical hardening changes were implemented:

1. Updated installed system packages to obtain available security and maintenance fixes.
2. Enabled UFW with a default-deny incoming policy and default-allow outgoing policy.
3. Enabled automatic package-list updates and unattended upgrades.

The final checks confirmed that the package manager was in a clean state, the firewall was active, and automatic updates were enabled.

These measures improve the system's security posture by reducing exposure to outdated software, restricting unsolicited incoming network connections, and automating future updates.
