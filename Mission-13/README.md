# Mission 13: Can We Trust the File?

## 1. Mission Objective

Detect file modification with hashing and understand what hashing can and cannot prove.

### Concepts
- File integrity
- SHA-256
- Tampering
- Authenticity

## 2. Environment

- OS: Ubuntu ARM64
- Tool: `sha256sum`
- Algorithm: SHA-256
- Working directory: `Mission-13`

## 3. Experiment

The experiment demonstrates how SHA-256 hashing can detect unexpected modifications to a file.

### Step 1: Create the original configuration file

Command:

```bash
echo "Approved configuration" > config.txt
```

This created a file containing:

```text
Approved configuration
```

### Step 2: Generate the original hash

Command:

```bash
sha256sum config.txt > config.sha256
```

Display the saved checksum:

```bash
cat config.sha256
```

**Original SHA-256 hash:**

```text
7e8320091ff5b18af4c4b21db8c2bd0bbbadabe170236c86d98c433886d88dfc
```

### Step 3: Verify the original file

Command:

```bash
sha256sum -c config.sha256
```

Output:

```text
config.txt: OK
```

The verification succeeded because the current file contents matched the saved checksum.

### Step 4: Modify the configuration file

Command:

```bash
echo "Unexpected change" >> config.txt
```

This appended additional content without removing the original content.

The file now contains:

```text
Approved configuration
Unexpected change
```

### Step 5: Generate the modified hash

Command:

```bash
sha256sum config.txt
```

**Modified SHA-256 hash:**

```text
bbd7b221e620b35e51bda162276e595fb24263fb3312b327cae37403ac840016
```

The modified hash is different from the original hash.

### Step 6: Verify against the original checksum

Command:

```bash
sha256sum -c config.sha256
```

Output:

```text
config.txt: FAILED
sha256sum: WARNING: 1 computed checksum did NOT match
```

Verification failed because the contents of `config.txt` no longer matched the original saved checksum.

## 4. Results and Analysis

| Property | Original file | Modified file |
|---|---|---|
| Content | Approved configuration | Approved configuration + Unexpected change |
| SHA-256 | 7e832009... | bbd7b221... |
| Verification against original hash | Passed | Failed |

The experiment demonstrates that SHA-256 produces a different hash when the file contents change.

Even a small modification can produce a substantially different hash value.

The original checksum was preserved in `config.sha256`, allowing the modified file to be compared against the original state.

## 5. Think About It

### Q38. What changed in the hash?

The SHA-256 hash changed from:

```text
7e8320091ff5b18af4c4b21db8c2bd0bbbadabe170236c86d98c433886d88dfc
```

to:

```text
bbd7b221e620b35e51bda162276e595fb24263fb3312b327cae37403ac840016
```

This happened because additional content was appended to the configuration file. SHA-256 is sensitive to changes in the input data, so even a small modification produces a different hash.

### Q39. Does a hash prove who created the file?

No. A hash can help verify that a file's contents match a previously recorded checksum, but it does not identify who created the file.

Anyone can generate a hash for a file. Therefore, a hash alone cannot establish the identity of its creator.

### Q40. Why is authenticity a different property from integrity?

Integrity means that the file has not been unexpectedly modified.

Authenticity means that the file genuinely comes from the claimed source.

A matching hash can help establish integrity when the original checksum is trusted. However, it does not independently prove the identity of the sender or creator.

## 6. Evidence Submitted

The following evidence is included for this mission:

1. Original hash: SHA-256 checksum generated and successful initial verification.
2. Changed hash: New checksum generated after modifying the configuration file.
3. Verification failure: Failed checksum verification with an explanation of the modification.

Screenshots are stored in the `screenshots/` directory.

## 7. Conclusion

This experiment demonstrated how SHA-256 hashing can be used to detect file modifications.

The original configuration file passed checksum verification. After unexpected content was appended, its SHA-256 hash changed and verification against the original checksum failed.

The experiment also demonstrated that integrity and authenticity are different security properties. Hashing helps detect modifications, but a hash alone does not prove who created or supplied a file.
