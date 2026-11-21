# Mission 8: The Permission Maze (Access Control Lists)

**Role:** Role 3 — Access Control & Privilege Engineer
**Syllabus Connection:** ACLs, DAC, fine-grained authorization
**Concepts:** Access Control Lists (ACLs), `getfacl`, `setfacl`, fine-grained authorization, least privilege

## 1. Comparison: Standard Permissions vs. Access Control Lists (ACLs)

Standard Unix Discretionary Access Control (DAC) permissions divide access into only three rigid categories: a single **owner**, a single **group**, and **other** (all remaining users). While simple and fast, this coarse-grained model cannot grant permissions to a specific second user without either making the file world-readable (`other`), changing file ownership, or asking an administrator to create and manage a new group. **Access Control Lists (ACLs)** extend standard DAC by attaching named user (`user:<name>:<perms>`) and named group (`group:<name>:<perms>`) entries directly to a file alongside an ACL `mask` that bounds the maximum effective permissions for extended entries. This enables fine-grained authorization—allowing `alice` to grant `bob` read-only access to `/tmp/acl_secret.txt` while keeping `group::---` and `other::---` completely locked down for every other user on the system.

## 2. "Think About It" Questions & Answers

### Question 22: What changed when the ACL was added?
* **Answer:** Running `sudo setfacl -m u:bob:r /tmp/acl_secret.txt` added two extended entries to the file's ACL metadata: `user:bob:r--` (granting read-only access specifically to `bob`) and `mask::r--` (defining the maximum effective permissions allowed for named users and groups). Importantly, the base owner (`user::rw-`), owning group (`group::---`), and other (`other::---`) entries remained unchanged, meaning no other non-owner user gained access.

### Question 23: Why is an ACL useful when owner/group/other is insufficient?
* **Answer:** Traditional `owner/group/other` permissions only support one owner and one group per file. When you need to share a file with a specific individual (like `bob`) or multiple distinct subsets of users with different permission levels (e.g., read-only for one user, read-write for another) without exposing the file to `other` or creating numerous single-purpose groups, ACLs solve the limitation cleanly at the file level.

### Question 24: What security principle is being exercised?
* **Answer:** The **principle of least privilege** (along with **fine-grained authorization / need-to-know**). Instead of broadening access to everyone (`other`) or giving `bob` unnecessary write permissions or group membership, `bob` is granted the exact minimum permission required (`r--`) to read the file, while all other users remain denied.

## 3. SECURELAB Evidence Template

* **Question:** How can Linux grant a single additional user (`bob`) read access to a restricted file (`600`) without weakening standard `owner/group/other` permissions for the rest of the system?
* **Prediction:** Before adding the ACL, `getfacl` will show only base `user::rw-`, `group::---`, and `other::---` entries. After running `setfacl -m u:bob:r`, `getfacl` will display `user:bob:r--` and `mask::r--`, allowing `bob` to read `/tmp/acl_secret.txt` while keeping `other::---` restricted.
* **Experiment:**
  ```bash
  sudo apt update
  sudo apt install -y acl
  sudo -u alice bash -c 'echo "ACL test" > /tmp/acl_secret.txt'
  sudo chown alice:alice /tmp/acl_secret.txt
  sudo chmod 600 /tmp/acl_secret.txt
  getfacl /tmp/acl_secret.txt
  sudo setfacl -m u:bob:r /tmp/acl_secret.txt
  getfacl /tmp/acl_secret.txt
  sudo -u bob cat /tmp/acl_secret.txt
  ```
* **Observation:**
  * Before `setfacl`: `getfacl` showed `# owner: alice`, `# group: alice`, `user::rw-`, `group::---`, `other::---`.
  * After `setfacl`: `getfacl` added `user:bob:r--` and `mask::r--`, and `sudo -u bob cat /tmp/acl_secret.txt` output `ACL test`.
* **Concept:** **Access Control Lists (ACLs)** and **Fine-Grained Authorization** extending standard Linux DAC.
* **Defense:** Named-user ACL entries (`setfacl -m u:bob:r`) paired with restrictive base permissions (`chmod 600`) to avoid overly permissive `other` bits.
* **Verification:** Inspected the updated ACL entries via `getfacl /tmp/acl_secret.txt` (`user:bob:r--`, `other::---`) and functionally verified read access by running `cat` as `bob`.
* **Reflection:** Without ACLs, users frequently fall back to insecure workarounds such as setting `chmod 644` or `chmod 666` (exposing files to `other`), which violates least privilege and allows any local user or compromised process to read or modify sensitive files.