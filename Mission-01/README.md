
# Mission 1 — Who Are You?

## Objective
Understand Linux user and group identities and how real and effective IDs influence process permissions.

## 1. Shell Identity

Command:
```bash
id
```

Observed:
- UID: 1000
- Primary GID: 1000
- Additional groups: adm, cdrom, sudo, dip, plugdev, users, lpadmin, lxd.

## 2. Process Identity Using C

Source code: `identity.c`

The program uses:
- `getuid()` — Real UID
- `geteuid()` — Effective UID
- `getgid()` — Real GID
- `getegid()` — Effective GID

### Normal Execution

| Identity | Value |
|---|---:|
| Real UID | 1000 |
| Effective UID | 1000 |
| Real GID | 1000 |
| Effective GID | 1000 |

## 3. Set-UID Experiment

Commands:

```bash
sudo chown root:root identity
sudo chmod u+s identity
./identity
```

Observed:

| Identity | Value |
|---|---:|
| Real UID | 1000 |
| Effective UID | 0 |
| Real GID | 1000 |
| Effective GID | 1000 |

### Observation

The real UID remained 1000 because the normal user launched the program. The effective UID became 0 because the Set-UID executable ran with root privileges.

The Set-UID permission was subsequently removed and ownership restored.

## 4. Theory Questions

### Q1. Difference between UID and EUID

The real UID identifies the user who started a process. The effective UID determines the user permissions the process uses when accessing protected resources.

### Q2. Authentication vs Authorization

Authentication verifies a user's identity. Authorization determines what an authenticated user is allowed to access or do.

### Q3. CIA property affected by unauthorized reading

Confidentiality is affected because sensitive information has been accessed by someone who is not authorized to see it.

## 5. Attack Surfaces

1. **SSH and network services:** Exposed remote-access services can be targeted by attackers.
2. **Web browsers:** Malicious websites, downloads, and browser vulnerabilities can compromise a workstation
