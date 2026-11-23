# Mission 14: What Happened While I Was Away?

## 1. Mission Objective

Use Linux logs to reconstruct a small timeline of events.

### Concepts
- Journald
- Authentication events
- Timestamps
- Audit trail
- Incident logging

## 2. Environment

- Operating System: Ubuntu ARM64
- Logging system: systemd-journald
- Commands used: `logger`, `journalctl`, `sudo`, `last`
- Working directory: `Mission-14`

## 3. Experiment

The experiment generated harmless events in the virtual machine and inspected the system journal to understand how Linux records system activity.

### Step 1: Generate test events

Three normal test events were generated using:

```bash
logger -t SECURELAB "Mission 14: Test event 1"
logger -t SECURELAB "Mission 14: Test event 2"
logger -t SECURELAB "Mission 14: Test event 3"
```

A privileged test event was generated using:

```bash
sudo logger -t SECURELAB "Mission 14: Privileged test event"
```

### Step 2: Inspect system logs

The following commands were used:

```bash
journalctl -n 30 --no-pager
journalctl --since "15 minutes ago" --no-pager
journalctl -t SECURELAB --no-pager
```

These commands display recent system logs, logs from a specified time period, and messages associated with the `SECURELAB` identifier.

## 4. Event Timeline

The following timeline was reconstructed from the actual journal output collected during the experiment on November 23, 2026.

| Time | Event | Process ID |
|---|---|---:|
| 16:21:14 | Test event 1 recorded | 5046 |
| 16:21:14 | Test event 2 recorded | 5047 |
| 16:21:14 | Test event 3 recorded | 5048 |
| 16:21:20 | Sudo session opened for root | 5050 |
| 16:21:20 | Privileged logger command executed | 5050 |
| 16:21:20 | Privileged test event recorded | 5057 |
| 16:21:20 | Sudo session closed | 5050 |

### Timeline Analysis

- Three normal test events were recorded at 16:21:14.
- A privileged command was executed six seconds later.
- The privileged event was recorded in the system journal.
- The sudo session subsequently closed.

This demonstrates how timestamps and process identifiers can help reconstruct a sequence of system activities.

## 5. Think About It

### Q41. Which events would be useful during an investigation?

Authentication attempts, successful and failed logins, sudo commands, user session creation and termination, service activity, and unexpected application events are useful during an investigation.

These events can help identify suspicious activity and reconstruct what happened on a system.

### Q42. Why are timestamps important?

Timestamps establish when events occurred and allow investigators to arrange them chronologically.

They also help correlate events from different services and identify the sequence of activities during an incident.

### Q43. What can logs fail to tell you?

Logs may not capture every activity performed on a system. They can be incomplete, disabled, deleted, modified, or lack sufficient context.

Logs also cannot always establish who was physically responsible for an action. Therefore, logs should be combined with other evidence during an investigation.

## 6. Evidence Submitted

The following evidence is required for this mission:

1. **A 5–10 event timeline:** Seven events reconstructed from the system journal.
2. **Identification of at least one limitation of logs:** Logs may be incomplete or modified and cannot independently prove who performed an action.

## 7. Conclusion

This experiment demonstrated how Linux system logs can be used to monitor activity and reconstruct a sequence of events.

Using `logger`, harmless test events were generated and recorded through systemd-journald. The `journalctl` command was used to inspect these events and construct a timeline.

The experiment also highlighted the limitations of logs, demonstrating why they are useful for incident investigation but should not be treated as complete or unquestionable evidence.
