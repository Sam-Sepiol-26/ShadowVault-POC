# ShadowVault: String Obfuscation PoC

A lightweight C implementation demonstrating **XOR-based string encryption** and **pointer-driven memory modification**. This project serves as a foundational study in how malware hides static strings (like C2 IP addresses) from static analysis tools like `strings` or basic disassemblers.

## Technical Overview
The program utilizes several concepts common in malware development:
* **XOR Encoding:** Obfuscates a hardcoded IP address in memory.
* **Pointer Arithmetic:** Uses `uint32_t*` to pass and modify keys across function boundaries.
* **Shadow Copying:** Implements a local "shadow" key to demonstrate how scope affects variable values during execution.
* **Bitwise Operations:** Uses left-shifts (`<<`) to dynamically generate the decryption key at runtime.

## Reverse Engineering Perspective
From a reverse engineer's view, this code presents a few "speed bumps":
1. **Dynamic Key Generation:** The key is modified in `modify_value()` before the XOR operation, preventing a simple static extraction of the key.
2. **Stack vs Global:** The difference between the global `master_key` and the local `master_key` in `main()` is a classic trap for beginners.

## Getting Started
### Compilation
Using GCC:
```bash
gcc src/main.c -o shadowvault.exe
