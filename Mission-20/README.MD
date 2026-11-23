# Mission 20 – Secure Remote Access

## Objective
Understand the security boundary created when someone connects remotely.

## Concepts
- SSH (Secure Shell)
- Authentication
- Authorization
- SSH keys and passwords
- Login logging

## Environment
- OS: Ubuntu 26.04.1 LTS
- Architecture: ARM64 (aarch64)
- Kernel: 7.0.0-43-generic
- Virtualization: UTM / QEMU

## 1. Install OpenSSH Server

Installed the OpenSSH server using:

```bash
sudo apt install -y openssh-server
```

The following packages were installed:
- openssh-server
- openssh-sftp-server

## 2. Verify SSH Service

Checked the SSH socket:

```bash
sudo systemctl status ssh.socket --no-pager
```

Observed:
- SSH socket: active (listening)
- Socket enabled at startup.
- IPv4 listener: `0.0.0.0:22`
- IPv6 listener: `[::]:22`

Verified listening ports:

```bash
ss -lnt | grep ':22' || true
```

The SSH server uses systemd socket activation. Therefore, `ssh.service` can appear inactive until a connection triggers it.

## 3. Local SSH Authentication Test

Connected to the SSH server locally:

```bash
ssh localhost
```

During the first connection:
- SSH displayed a host authenticity warning.
- The ED25519 host key fingerprint was displayed.
- Accepted the host key by entering `yes`.
- The host key was saved in the local `known_hosts` file.
- Entered the Ubuntu account password.

The connection succeeded and displayed the Ubuntu welcome message:

```text
Welcome to Ubuntu 26.04.1 LTS
```

The terminal provided an interactive shell as:

```text
pranav-rudraraju@localhost
```

Exited the SSH session using:

```bash
exit
```

## 4. Authentication vs Authorization

| Authentication | Authorization |
|---|---|
| Verifies the identity of a user. | Determines what an authenticated user is allowed to do. |
| Confirms who is connecting. | Defines which resources and operations the user can access. |
| Example: SSH password or public-key authentication. | Example: Linux file permissions and sudo privileges. |

Authentication happens before authorization. Successfully logging in does not automatically grant administrative privileges.

## 5. Think About It

### Q62. What does SSH authenticate?

SSH authenticates the remote user's identity using configured authentication methods, such as passwords or public-key authentication. It also verifies the server's host key to help the client confirm the server's identity.

### Q63. After authentication, what determines what the user can do?

The user's account permissions, group memberships, file permissions, access-control policies, and sudo privileges determine what actions the authenticated user can perform.

### Q64. Where would you look for login evidence?

SSH authentication and login events can be inspected using:

```bash
sudo journalctl -u ssh
```

On systems that maintain traditional authentication logs, they can also be inspected in:

```bash
sudo less /var/log/auth.log
```

These logs can contain successful logins, failed authentication attempts, and SSH-related events.

## 6. Security Considerations

The handout instructs that SSH must remain inside the VM/lab network and must not be exposed to the public internet.

During this mission:
- SSH was tested locally using `ssh localhost`.
- The SSH socket was listening on port 22.
- UFW remained configured with a default-deny incoming policy.
- No SSH firewall allow rule was added.

## Conclusion

Successfully installed and verified OpenSSH Server and performed a local SSH login using password authentication.

The experiment demonstrated host-key verification, user authentication, and the distinction between authentication and authorization. SSH login activity can be investigated through system authentication logs.

The experiment was performed locally inside the VM, following the handout's security instructions.
