# Mission 7: Alice vs Bob (Discretionary Access Control)

**Role:** Role 3 — Access Control & Privilege Engineer  
**Syllabus Connection:** Authentication, authorization, DAC, ownership and permissions  
**Concepts:** Discretionary Access Control (DAC), owner/group/other, least privilege, authorization  

---

## 1. Explanation of Owner, Group, and Other

In Linux, standard Discretionary Access Control (DAC) evaluates file access using three permission classes shown in `ls -l` (for example, `-rw-r--r--` or `-rw-------`):

1. **Owner (`u` — User):** The specific user account that owns the file (in this experiment, `alice`). The first three permission characters (`rw-`) apply exclusively when `alice` accesses the file, granting read and write permissions.
2. **Group (`g` — Group):** The primary or assigned group associated with the file (here, the `alice` group). The middle three characters (`r--` before `chmod 600`, and `---` after) apply to members of that group who are not the file owner.
3. **Other (`o` — Others):** All remaining users on the system who are neither the owner nor members of the file's group (such as `bob`). The final three characters (`r--` before `chmod 600`, and `---` after) govern their access.

Linux checks these classes in order (**Owner $\rightarrow$ Group $\rightarrow$ Other**) and applies the first matching category.

---

## 2. "Think About It" Questions & Answers

### Question 19: Why did Bob's access change after `chmod 600`?
* **Answer:** Initially, `/tmp/alice_secret.txt` had permissions `-rw-r--r--` (`644`), which gave the **other** category read (`r`) permission. Because `bob` is neither the owner (`alice`) nor a member of the `alice` group, Linux evaluated `bob` under the **other** category and allowed `cat` to read the file. Running `chmod 600` changed the permissions to `-rw-------`, stripping read permission from both **group** and **other**. When `bob` tried reading the file afterward, the kernel checked the **other** permission bits (`---`), found no read permission, and returned `Permission denied`.

### Question 20: Who controls the discretionary permission on this file?
* **Answer:** The **file owner (`alice`)** (along with the `root` superuser) controls the discretionary permissions on `/tmp/alice_secret.txt`. Under Discretionary Access Control (DAC), the owner of a resource has the discretion to decide which users, groups, or others may read, write, or execute that resource using commands such as `chmod` and `chown`.

### Question 21: How does this experiment demonstrate least privilege?
* **Answer:** The **principle of least privilege** states that users and processes should only be granted the minimum access required to perform their intended tasks. By default (`644`), every user on the system (`other`) had unnecessary read access to Alice's private file, violating confidentiality. Restricting the file to `600` (`-rw-------`) removed unneeded read privileges from everyone except `alice`, ensuring that unauthorized users like `bob` cannot access sensitive data they do not need.

---

## 3. SECURELAB Evidence Template

* **Question:** How does Linux Discretionary Access Control (DAC) use file ownership and permission bits (`owner`, `group`, `other`) to allow or deny file access between different local users?
* **Prediction:** Before changing permissions, default file creation (`644`) will allow `bob` to read `alice`'s file because `other` has read permission. After setting `chmod 600`, `bob` will be denied access because only `alice` (the owner) retains read/write permissions.
* **Experiment:**
  ```bash
  sudo adduser alice
  sudo adduser bob
  sudo -u alice bash -c 'echo "Alice secret" > /tmp/alice_secret.txt'
  sudo chown alice:alice /tmp/alice_secret.txt
  ls -l /tmp/alice_secret.txt
  sudo -u bob cat /tmp/alice_secret.txt || true
  sudo chmod 600 /tmp/alice_secret.txt
  ls -l /tmp/alice_secret.txt
  sudo -u bob cat /tmp/alice_secret.txt || true
  ```
* **Observation:**
  * Before `chmod 600`: `ls -l` showed `-rw-r--r-- 1 alice alice`, and `sudo -u bob cat /tmp/alice_secret.txt` printed `Alice secret`.
  * After `chmod 600`: `ls -l` showed `-rw------- 1 alice alice`, and `sudo -u bob cat /tmp/alice_secret.txt` failed with `cat: /tmp/alice_secret.txt: Permission denied`.
* **Concept:** **Discretionary Access Control (DAC)** and **Authorization** — once users are authenticated (`alice` vs. `bob`), the operating system kernel mediates file access based on resource ownership (`UID`/`GID`) and permission bits.
* **Defense:** Restrictive file permissions (`chmod 600`) enforcing **Least Privilege** so private files are not world-readable.
* **Verification:** Confirmed both visually via `ls -l` (verifying `r--` was removed from `other`) and functionally by re-running `cat` as `bob` and observing `Permission denied`.
* **Reflection:** If restrictive DAC permissions are omitted (or overly permissive `umask` defaults are left unchanged on sensitive directories/files), any local user or compromised low-privilege service account on the machine can read confidential files, directly violating data confidentiality.