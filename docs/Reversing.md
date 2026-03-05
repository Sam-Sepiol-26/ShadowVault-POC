# Reversing: ShadowVault Memory Analysis

This document provides a technical walkthrough for extracting the hidden "Secret Message" from the `ShadowVault` binary using static and dynamic analysis techniques.

---

## Objective
Identify the plaintext string obfuscated within the `msg[]` buffer without utilizing the source code.

---

## Phase 1: Static Analysis (Ghidra / IDA Pro)

### 1. Identifying the Obfuscated Buffer
Navigate to the `.data` section of the binary. You will find a hex array labeled `msg`.
* **Observation:** Running `strings` on the binary only reveals the "Enter your code" prompt. The actual payload is non-human-readable in its static state.

### 2. Tracing the Logic (XREFs)
Locate where `msg` is referenced. You will find it inside a loop performing a rolling XOR operation:
`shadow_vault[i] = msg[i] ^ (key + i);`
* **The Trap:** The `key` is a pointer passed from `main`. You must analyze the caller to identify the initialized value.

### 3. Spotting the Shadow Copy
In the disassembled `main` function, two variables share the name `master_key`:
* **Global:** `0x11223344` (A "Red Herring" to mislead static analysis).
* **Local (Stack):** `0xDEADBEEF` (The actual starting point for the routine).

---

## Phase 2: Dynamic Analysis (x64dbg)

### 1. Locating Key Transformation
Set a breakpoint at the start of `main` and step into the `modify_value` function.
* **Instruction:** Look for the `SHL` (Shift Left) opcode.
* **Calculation:** `0xDEADBEEF << 1` results in `0xBD5B7DDE`.
* **Result:** This derived value is the actual key used for the initial `XOR_data` encryption.

### 2. Defeating the Input Validation
The `input_func` compares your hexadecimal input against the runtime `master_key`. 
* **The Bypass:** Set a breakpoint on the `CMP` instruction inside `input_func`.
* **Memory Inspection:** Inspect the register holding the comparison value. This reveals the "Magic Code" required to trigger the decryption path.

### 3. Dumping the Payload
Once the correct key is provided:
1. Step over the `decrypt_data` function.
2. Inspect the memory address for `shadow_vault`.
3. **Plaintext:** The hex values resolve to: `IP:172.168.278.90`.

---

## Analysis Summary
| Artifact | Value |
| :--- | :--- |
| **Static Key (Red Herring)** | `0x11223344` |
| **Initial Local Key** | `0xDEADBEEF` |
| **Runtime Derived Key** | `0xBD5B7DDE` |
| **Obfuscation Method** | Index-linked XOR Rolling Cipher |

---
