# Dll Inject — Documentation

`dll-inject` is a small Windows command-line tool that **starts an executable
with a DLL already loaded inside it**. The DLL is injected before the target's
entry point runs, so the DLL's `DllMain` executes at the very start of the
program's life.

It is the application front end of the XYO C++ stack's injection library
(`xyo-win-inject`): `dll-inject` parses the command line and `xyo-win-inject`
does the actual process creation and injection.

```
dll-inject            <-- this tool (command line front end)
xyo-win-inject        (createProcessA/W + injectDllDirect: the injection engine)
xyo-win               (Windows umbrella library)
xyo-system / xyo-encoding / ...   (the rest of the XYO C++ stack)
```

## What it does, in one line

```
dll-inject [mode] <dll> <app> [app-arguments...]
```

Launch `<app>` (with the given arguments), force `<dll>` to load into it at
startup, optionally hidden, at realtime priority, and/or waiting for the app to
finish.

## Why use it

- **Load a DLL into a program you do not control**, without modifying the
  program on disk or changing the system `AppInit_DLLs` / search path. Useful
  for hooking, instrumentation, tracing, mods, or testing a DLL's `DllMain`
  behaviour under a real host process.
- **The DLL is present before the first instruction** of the target runs, so a
  hooking DLL can install its hooks before the application initialises.
- **No separate loader** to write: a single command does create-suspended,
  inject, and resume.

`dll-inject-sample` (a sibling repository) is an example payload DLL that hooks
Win32 API functions once injected, and is a good companion for trying this tool.

## Contents

| Document | What it covers |
|----------|----------------|
| [Usage](usage.md) | Command line, every option, exit codes, worked examples |
| [How it works](how-it-works.md) | The create-suspended / inject / resume mechanism, requirements and limitations |
| [Building and testing](building.md) | Build, run the tests, and install with fabricare |

## Requirements and caveats at a glance

- **Windows only.** The tool is built for `win64-msvc-2026` on this machine.
- **Runs elevated.** The executable's manifest requests
  `requireAdministrator`; Windows prompts for elevation when it starts.
- **Architecture must match.** A 64-bit `dll-inject` injects into 64-bit
  targets only, and the injected DLL must be 64-bit as well (and vice versa for
  a 32-bit build).
- **`<dll>` is passed to `LoadLibraryA` inside the target**, so prefer an
  absolute path; a bare name is resolved by the *target's* DLL search order.
- Injecting code into another process is a powerful, easily-misused operation.
  Use it only on software and systems you are authorised to modify.

## License

Copyright (c) 2008-2026 Grigore Stefan. Licensed under the
[MIT](../LICENSE) license.
