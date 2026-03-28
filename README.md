<div align="center">

<table border="0" cellpadding="0" cellspacing="0" align="center">
  <tr>
    <td valign="middle">
      <img src="minishell_pkg/usr/share/icons/hicolor/512x512/apps/mnsh.png" alt="" height="100"/>
    </td>
    <td valign="middle">
      <font size="8"><b style="font-weight: 600; font-family:monospace">MiniShell</b></font>
    </td>
  </tr>
</table>

<br/>

![Language](https://img.shields.io/badge/Language-C%2B%2B17-e74c3c?style=flat-square&logo=c%2B%2B&logoColor=white)
![Standard](https://img.shields.io/badge/Standard-POSIX-2ecc71?style=flat-square)
![Status](https://img.shields.io/badge/Status-Active-2ecc71?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-f39c12?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Linux-8b949e?style=flat-square&logo=linux&logoColor=white)

<br/>

*A minimal, POSIX-compliant Unix-style shell for Linux distributions.*
</div>

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Build](#build)
- [License](#license)
- [Author](#author)

---

## Overview

**MiniShell** is a lightweight Linux shell written in C++ from the ground up.
Command parsing, process forking, pipe chaining, I/O redirection, and signal handling — all in a clean, readable codebase..

```bash
mnsh:~$ echo "Hello, Shell!" | tr '[:lower:]' '[:upper:]'
"HELLO, SHELL!"
mnsh:~$ _
```

---

## Features
- Execution of external commands using `fork`, `execvp`, and `waitpid`.
- Abstract Syntax Tree (AST) – based parsing for complex command structures.
- Logical operators:
  - `&&` (AND)
  - `||` (OR)
- Sequence - `;`
- Pipelines using - `|`
- Input/output/error redirections:
  -  `<`, `>`, `>>`, `2>`
- Subshell support using parentheses `( ... )` with correct scope isolation.
- Built-in commands like `cd`, `exit`, etc.
- Accurate exit-status propagation enabling correct short-circuit logic.
- Parent/child process separation for built-ins and subshells.
- Signal handling: Supports Ctrl+C interruption for foreground commands and subshell execution, with signal isolation between parent shell and child processes.
- Line editing & history (`↑↓` navigate, `←→` cursor, `Ctrl+A/E`, `Tab` completion, `history`, `!n`)

---

## Architecture
The shell follows a modular design:
> **Tokenizer → Parser → AST → Executor**
- ### Tokenizer
  Converts raw input into tokens (commands, operators, redirections, parentheses).
- ### Parser
  Builds an AST respecting operator precedence:
  > `;`  → lowest\
  > `&&` and `||`\
  > `|`\
  > command/subshell → highest
- ### AST Nodes
  - Command
  - Pipeline
  - Logical
  - Sequence
  - Subshell
- ### Executor
  - Recursively evaluates AST nodes using post-order traversal
  - Forks processes where required
  - Preserves shell state for built-ins

---

## Build
```bash
# Ubuntu / Debian
sudo apt install build-essential
mkdir build
cd build
cmake ..
make
./mnsh  or gnome-terminal -- ./mnsh (for new tab in terminal)
```

---

## License
Distributed under the **MIT License**. See [`LICENSE`](./LICENSE) for details.

---

## Author
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0a66c2?style=flat-square&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/chiranjivi-keshav-907156232/)
[![GitHub](https://img.shields.io/badge/GitHub-181717?style=flat-square&logo=github&logoColor=white)](https://github.com/chiranjivikeshav)
[![Gmail](https://img.shields.io/badge/Gmail-ea4335?style=flat-square&logo=gmail&logoColor=white)](mailto:chiranjivikeshavjnvm@gmail.com)
[![Twitter](https://img.shields.io/badge/Twitter-1d9bf0?style=flat-square&logo=twitter&logoColor=white)](https://x.com/ChiranjiviKesh1)
[![Instagram](https://img.shields.io/badge/Instagram-e1306c?style=flat-square&logo=instagram&logoColor=white)](https://instagram.com/chiranjivikeshav)

---
<div align="center">
<a href="#table-of-contents">⬆ Back to top</a>
</div>
