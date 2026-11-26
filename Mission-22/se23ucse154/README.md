# SECURELAB — Mission 22
## Linux System Security Assessment and Remediation Report

**Student ID:** se23ucse154  
**Operating System:** Ubuntu 24.04.5 LTS  
**Hostname:** VulnerableVM  
**Virtualization:** UTM / QEMU  
**Assessment date:** 27 November 2026

---

## 1. Executive Summary

This assessment examined the security configuration of an Ubuntu Linux virtual machine, identified five security weaknesses, and applied operating-system-level controls where possible.

The findings were:

1. Address Space Layout Randomization (ASLR) disabled.
2. Host firewall inactive.
3. User password configured never to expire.
4. No explicit PAM failed-login lockout policy.
5. No file integrity monitoring solution detected.

ASLR was enabled, UFW was activated with a default-deny incoming policy, password aging was configured, and PAM-based failed-login protection was introduced. AIDE was installed, but its initial database creation did not complete during the assessment period. Therefore, file integrity monitoring is recorded as partially remediated and not yet verified.

## 2. System Profile

| Component | Details |
|---|---|
| Operating System | Ubuntu 24.04.5 LTS |
| Architecture | x86_64 |
| Kernel | Linux 7.0.0-42-generic |
| Virtualization | UTM / QEMU |
| Hostname | VulnerableVM |
| Primary User | vboxuser |
| CPU Allocation | 4 cores |
| Memory Allocation | 4 GB |
| Firewall | UFW |
| Mandatory Access Control | AppArmor |
| Integrity Monitoring | AIDE (installed; initialization incomplete) |

## 3. Threat Model

The assessment considered threats relevant to a Linux workstation or server:

- Exploitation of memory-corruption vulnerabilities.
- Unauthorized inbound network access.
- Credential compromise and long-lived compromised passwords.
- Repeated password-guessing attempts.
- Unauthorized modification of critical system files.

The objective was to reduce the likelihood and potential impact of these threats using built-in Linux security mechanisms and established security tools.

## 4. Findings and Security Classification

### 4.1 Finding 1 — Address Space Layout Randomization Disabled

**Description and evidence**

The initial command:

`sysctl kernel.randomize_va_space`

reported:

`kernel.randomize_va_space = 0`

A value of zero indicates that ASLR was disabled. ASLR randomizes memory locations used by processes, making memory addresses less predictable to an attacker.

**Potential impact**

Disabling ASLR can make exploitation of memory-corruption vulnerabilities easier by making relevant memory locations more predictable.

**Security classification**

- Isolation
- Confidentiality
- Integrity

**Remediation**

ASLR was enabled with:

`sudo sysctl -w kernel.randomize_va_space=2`

Persistent configuration was added under `/etc/sysctl.d/`. An existing lab configuration that explicitly set ASLR to zero was corrected, and the sysctl settings were reloaded.

**Verification**

The active setting was subsequently observed as:

`kernel.randomize_va_space = 2`

**Status:** Remediation applied; active setting verified.

### 4.2 Finding 2 — Host Firewall Inactive

**Description and evidence**

The initial command:

`sudo ufw status verbose`

reported:

`Status: inactive`

The initial inspection also found no active filtering rules in the inspected nftables/iptables rulesets.

**Potential impact**

Without an active host firewall policy, inbound network traffic may reach listening services without UFW filtering, increasing the system's network attack surface.

**Security classification**

- Attack Surface
- Confidentiality
- Integrity
- Availability

**Remediation**

The firewall was configured and enabled:

`sudo ufw default deny incoming`

`sudo ufw default allow outgoing`

`sudo ufw enable`

**Verification**

The resulting UFW status reported:

- Status: active
- Logging: on (low)
- Default incoming policy: deny
- Default outgoing policy: allow
- Routed traffic: disabled

**Status:** Remediation applied; firewall activation and policy verified.

### 4.3 Finding 3 — User Password Never Expires

**Description and evidence**

The initial account policy was inspected using:

`sudo chage -l vboxuser`

It reported that the password expires never, with a maximum password age of 99999 days and a minimum age of zero days.

**Potential impact**

If credentials are compromised, a password with no expiration limit may remain usable for an extended period unless the compromise is otherwise detected and the credentials are changed. Password expiration is a policy choice and should be aligned with the organization's security requirements.

**Security classification**

- Authentication
- Confidentiality

**Remediation**

Password aging was configured using:

`sudo chage -M 90 -m 1 -W 7 vboxuser`

This sets a maximum password age of 90 days, a minimum age of one day, and a seven-day expiration warning.

**Verification**

The account's updated expiration date was displayed using `chage -l`.

**Status:** Remediation applied; updated expiration date observed.

### 4.4 Finding 4 — No Explicit PAM Failed-Login Lockout Policy

**Description and evidence**

The PAM configuration was searched for common failed-login lockout mechanisms:

`grep -RniE 'pam_faillock|pam_tally2|deny=|unlock_time=' /etc/pam.d/`

No explicit failed-login lockout configuration was found in the inspected PAM files.

This finding is scoped to the inspected PAM configuration; it does not establish that every authentication service was exposed to unlimited attempts.

**Potential impact**

Without an explicit lockout policy in the relevant PAM authentication stack, repeated failed authentication attempts may not be restricted by that stack, increasing exposure to password-guessing attempts.

**Security classification**

- Authentication
- Confidentiality
- Availability

**Remediation**

The `pam_faillock` module was configured in the PAM authentication stack. The policy in `/etc/security/faillock.conf` was set to:

```ini
deny = 5
fail_interval = 900
unlock_time = 600
local_users_only
```

The intended policy is to lock a local account after five failed attempts within 900 seconds and automatically unlock it after 600 seconds. A backup of the original PAM configuration was retained.

**Verification**

The PAM entries were inspected after modification. A `sudo -k` followed by `sudo -v` successfully authenticated the legitimate user. The actual five-attempt lockout behavior was not independently tested.

**Status:** Configuration applied; legitimate sudo authentication verified. Full lockout behavior remains untested.

### 4.5 Finding 5 — No File Integrity Monitoring Solution Detected

**Description and evidence**

The initial package inspection found no common file integrity monitoring tools, including AIDE, Tripwire, Samhain, OSSEC, or Wazuh.

**Potential impact**

Without file integrity monitoring, unauthorized changes to important system files and configuration may go undetected.

**Security classification**

- Integrity

**Remediation attempted**

AIDE (Advanced Intrusion Detection Environment) and its common configuration package were installed successfully:

- `aide`
- `aide-common`

Initial database creation was started with:

`sudo aideinit`

However, initialization was still running at the end of the assessment. The generated `/var/lib/aide/aide.db.new` file was still zero bytes at the last inspection.

**Verification and limitation**

Package installation was confirmed. The initial database was not successfully created and activated during the assessment period. Consequently, a successful AIDE integrity check cannot be claimed.

**Recommended completion steps**

1. Complete AIDE database initialization.
2. Activate the generated baseline database.
3. Run `sudo aide --check`.
4. Configure periodic integrity checks.
5. Verify that a controlled file change is detected.

**Status:** Partially remediated. AIDE installed; database initialization and integrity verification pending.

## 5. Attack Surface Assessment

The initial assessment also examined services, listening sockets, account privileges, file permissions, AppArmor, and system logging.

| Component | Initial observation | Assessment |
|---|---|---|
| Kernel security | ASLR disabled | Finding identified |
| Host firewall | UFW inactive | Finding identified |
| Password policy | Password never expires | Finding identified |
| PAM authentication | No explicit faillock policy found | Finding identified |
| File integrity | No common monitoring package detected | Finding identified |
| SSH | SSH server inactive/not installed | No active SSH listener identified |
| AppArmor | Profiles loaded; enforcement present | Existing protection |
| Automatic updates | Unattended upgrades active | Existing protection |
| System logging | rsyslog active | Existing protection |

Other inspected areas, including sensitive file permissions and privileged executables, did not produce additional confirmed findings for this report. Potential observations were not labelled vulnerabilities without sufficient evidence.

## 6. Remediation Summary

| No. | Finding | Control applied | Status |
|---|---|---|---|
| 1 | ASLR disabled | Enabled kernel ASLR and corrected persistent configuration | Applied; active value verified |
| 2 | Firewall inactive | Enabled UFW with default-deny incoming policy | Applied; status verified |
| 3 | Password never expires | Configured password aging | Applied; expiration date observed |
| 4 | No explicit PAM lockout | Configured pam_faillock policy | Applied; lockout behavior not fully tested |
| 5 | No file integrity monitoring | Installed AIDE and started baseline initialization | Partially applied; initialization incomplete |

## 7. Evidence Collection

Terminal output and configuration inspections were collected in:

`~/Mission22-Evidence`

Evidence categories include:

- Kernel security configuration and ASLR settings.
- Initial and remediated firewall status.
- Initial and updated password-aging policy.
- PAM authentication configuration and faillock policy.
- AIDE package installation and initialization status.

The evidence directory should be retained with the assessment materials. Screenshots and raw terminal evidence should be included when available.

## 8. Conclusion

Five security weaknesses were identified in the Ubuntu virtual machine. The remediation work improved the system's security posture by enabling ASLR, activating the host firewall, establishing password-aging rules, and introducing a PAM-based failed-login lockout policy.

AIDE was installed as the foundation for file integrity monitoring, but its baseline database creation remained incomplete. This control is therefore recorded as partially remediated rather than operational.

The assessment demonstrates the importance of identifying security weaknesses, applying appropriate operating-system controls, and verifying the resulting behavior. A configuration change should not be treated as a successful remediation until the intended control has been validated.

## 9. Reflection

This mission provided practical experience with Linux system hardening and security auditing. It involved kernel security parameters, firewall configuration, account policy management, PAM authentication, and file integrity monitoring.

It also reinforced the importance of evidence-based reporting: findings should be supported by observed configuration, remediation status should be stated accurately, and incomplete verification should be clearly disclosed.

---

**Project:** SECURELAB  
**Mission:** 22 — Final Challenge  
**Student ID:** se23ucse154
