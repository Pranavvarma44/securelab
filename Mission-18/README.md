# Mission 18: Compare Access-Control Models

## Objective

Understand and compare different access-control mechanisms used in operating systems and applications. This mission examines how permissions are assigned, enforced, and managed using DAC, MAC, RBAC, ACLs, and capabilities.

## 1. Access-Control Models Comparison

| Access-Control Model | Full Form | How It Works | Who Controls Access? |
|---|---|---|---|
| DAC | Discretionary Access Control | Resource owners decide who can access their resources through permissions. | Resource owner |
| MAC | Mandatory Access Control | Access is enforced through centrally defined security policies that users cannot freely override. | System/security administrator |
| RBAC | Role-Based Access Control | Permissions are assigned to roles, and users receive permissions through their assigned roles. | Administrator |
| ACL | Access Control List | Each resource maintains a list specifying which users or groups have particular permissions. | Resource owner or administrator |
| Capabilities | Capability-Based Access Control | Access is granted through specific capabilities or tokens that represent authority over resources. | Capability issuer or system |

## 2. Real-World Scenarios

### A. Discretionary Access Control (DAC)

**Scenario:** A developer creates a project directory and grants read and write access to selected team members using Linux file permissions.

The owner can change permissions using commands such as `chmod` and `chown`.

**Example:** A file owned by Alice is readable by members of the `securelab` group, while other users are denied access.

### B. Mandatory Access Control (MAC)

**Scenario:** A server uses AppArmor to restrict the operations that an application can perform.

Even if an application runs with sufficient Unix permissions, AppArmor can prevent it from accessing files or performing operations that violate its security profile.

**Example:** A web server may be restricted to accessing only its designated configuration and website directories.

### C. Role-Based Access Control (RBAC)

**Scenario:** A university management system assigns permissions based on user roles.

- Students can view their marks and attendance.
- Teachers can update marks and attendance.
- Administrators can manage users and system settings.

Users receive permissions based on their assigned roles rather than having permissions configured individually.

### D. Access Control Lists (ACLs)

**Scenario:** A shared Linux directory needs to provide access to specific users without changing its existing owner and group permissions.

ACLs allow administrators to grant additional permissions to individual users or groups.

**Example:** A file owner can grant Alice read access and Bob read/write access using ACL entries.

### E. Capability-Based Access Control

**Scenario:** An application receives a specific capability that allows it to access a particular resource or perform a particular operation.

Instead of giving the application unrestricted administrative privileges, it receives only the authority it needs.

**Example:** A process receives permission to access a specific file descriptor without being granted unrestricted access to the entire filesystem.

## 3. Practical Observations from Previous Missions

### Mission 12: DAC

During Mission 12, Unix ownership and file permissions were tested using `/opt/securelab/incident.txt`.

Observed results:

- Alice could read the file through membership in the `securelab` group.
- Bob was denied access.
- Root could access the file.

This demonstrated how traditional Unix ownership and permission bits control access to resources.

### Mission 17: MAC

During Mission 17, AppArmor was installed and inspected.

Observed results:

- AppArmor was active.
- 229 profiles were loaded.
- 151 profiles were in enforce mode.
- 3 profiles were in complain mode.
- The `/usr/sbin/cupsd` profile contained explicit permissions and denial rules.

This demonstrated how mandatory security policies restrict application behavior independently of ordinary Unix file permissions.

## 4. Answers to Think About It

### Q53. Which access-control model gives the resource owner the most flexibility?

**Answer:** Discretionary Access Control (DAC) gives the resource owner the most flexibility. The owner can decide who can access a resource and modify its permissions. This flexibility makes DAC convenient but can also lead to inconsistent security if permissions are not managed carefully.

### Q54. Why is MAC generally stronger than DAC in high-security environments?

**Answer:** MAC enforces centrally defined security policies that ordinary users cannot freely modify or bypass. This prevents resource owners from accidentally granting access that violates the system's security requirements. It is therefore useful in environments where strict security enforcement is required.

### Q55. How does RBAC simplify permission management in large organizations?

**Answer:** RBAC groups permissions into predefined roles instead of assigning permissions individually to every user. Administrators can assign users to roles such as student, teacher, manager, or administrator. This simplifies administration, reduces duplication, and makes permission management easier as organizations grow.

### Q56. How are ACLs different from traditional Unix permission bits?

**Answer:** Traditional Unix permissions mainly define access for the file owner, owning group, and others. ACLs provide more granular control by allowing permissions to be assigned to specific users and additional groups independently.

### Q57. Why are capabilities useful for limiting application privileges?

**Answer:** Capabilities allow applications or processes to receive only the specific privileges they need. Instead of granting broad administrative access, a process can be given limited authority over particular resources or operations. This follows the principle of least privilege and reduces the potential damage if an application is compromised.

## 5. Conclusion

This mission compared five access-control mechanisms and examined how each manages permissions and security.

DAC provides flexibility to resource owners, while MAC enforces centrally defined policies. RBAC simplifies permission management through roles, ACLs provide fine-grained resource-level permissions, and capabilities grant specific privileges to processes.

The observations from Mission 12 and Mission 17 demonstrated DAC and MAC in a practical Linux environment. Together, these mechanisms help implement layered security and the principle of least privilege.
