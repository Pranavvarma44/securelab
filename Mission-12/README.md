# Mission 12: The Secret File

## Objective
Design a small directory hierarchy with deliberate access rules and verify access using test accounts.

## Concepts
- Ownership
- Groups
- File and directory permissions
- Directory traversal
- Least privilege

## Environment
- OS: Ubuntu ARM64
- Test accounts: Alice and Bob
- Group: `securelab`
- Directory: `/opt/securelab`

## 1. User and Group Configuration

Created the `securelab` group and added Alice to it.

Bob was kept outside the group to test unauthorized access.

Verified using:

```bash
id alice
id bob
getent group securelab
```

Results:
- Alice belongs to `securelab`.
- Bob does not belong to `securelab`.

## 2. Directory Hierarchy

Created the following structure:

```text
/opt/securelab/
├── public/
├── employees/
└── secrets/
    └── incident.txt
```

## 3. Ownership Configuration

Applied ownership:

```bash
sudo chown -R root:securelab /opt/securelab
```

All directories and the incident file are owned by `root` and associated with the `securelab` group.

## 4. Permission Design

| Resource | Owner:Group | Permission | Purpose |
|---|---|---|---|
| `/opt/securelab` | root:securelab | 755 | Allow directory traversal |
| `public` | root:securelab | 755 | Public access |
| `employees` | root:securelab | 750 | Restrict access to owner and group |
| `secrets` | root:securelab | 750 | Restrict access to owner and group |
| `incident.txt` | root:securelab | 640 | Owner can read/write; group can read |

### Permission explanation

- `755`: Owner has read, write and execute; group and others have read and execute.
- `750`: Owner has full permissions; group has read and execute; others have no access.
- `640`: Owner has read and write; group has read-only access; others have no access.

## 5. Access Control Experiments

Tested access using:

```bash
sudo -u alice cat /opt/securelab/secrets/incident.txt
sudo -u bob cat /opt/securelab/secrets/incident.txt
```

Incident file content:

```text
Confidential: Security incident report
```

### Results

| User | Group membership | Access result |
|---|---|---|
| Alice | securelab | Allowed |
| Bob | Not a member | Denied |

Alice successfully read the incident report.

Bob received:

```text
cat: /opt/securelab/secrets/incident.txt: Permission denied
```

The current user, Pranav, also received permission denied when attempting to access the file without elevated privileges.

Root successfully inspected the file permissions.

## 6. Permission Verification

Verified using:

```bash
sudo ls -ld /opt/securelab
sudo ls -ld /opt/securelab/public
sudo ls -ld /opt/securelab/employees
sudo ls -ld /opt/securelab/secrets
sudo ls -l /opt/securelab/secrets/incident.txt
```

Observed permissions:

```text
drwxr-xr-x root securelab /opt/securelab
drwxr-xr-x root securelab /opt/securelab/public
drwxr-x--- root securelab /opt/securelab/employees
drwxr-x--- root securelab /opt/securelab/secrets
-rw-r----- root securelab /opt/securelab/secrets/incident.txt
```

## 7. Think About It

### Q35. Why does directory permission matter as well as file permission?

Directory permissions control whether a user can list or traverse a directory. Even if a file has read permission, a user needs execute permission on its parent directories to access it through its path.

### Q36. Who should read the incident file?

Only root and authorized members of the `securelab` group should read it. Alice can read the file because she belongs to the group, while Bob cannot.

### Q37. What would happen if the file were world-readable?

Anyone who can traverse the directory hierarchy could read the confidential incident report. This would expose sensitive information to unauthorized users.

## 8. Least Privilege Explanation

The principle of least privilege means giving each user only the permissions required to perform their responsibilities.

In this experiment:
- Root owns and controls the directory hierarchy.
- Alice receives access through the `securelab` group.
- Bob is excluded from the group and cannot access confidential information.
- The incident file uses permission `640`, preventing access by other users.

This reduces the risk of unauthorized disclosure and accidental modification of sensitive information.

## Conclusion

The experiment successfully demonstrated Linux ownership, group-based access control, directory traversal permissions and the principle of least privilege.

Alice was able to read the confidential incident file, while Bob was denied access. The configured permissions behaved as expected.
