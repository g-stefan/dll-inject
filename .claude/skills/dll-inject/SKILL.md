---
name: dll-inject
description: >-
  How to use dll-inject, the Windows command-line tool of the XYO C++ stack
  that starts an executable with a DLL already injected at process creation.
  It wraps XYO::Win::Inject::Process::createProcessA from xyo-win-inject:
  create the target suspended, write a LoadLibraryA stub into it, redirect the
  thread, resume. Covers the command line
  `dll-inject [mode] <dll> <app> [args...]`, the mutually exclusive mode flags
  --wait, --hidden, --realtime, --hidden-realtime, --wait-hidden-realtime, the
  informational options --help/-h/-?, --version, --license, exit codes, the
  requirements (Windows, administrator elevation, matching architecture for
  tool/dll/target, absolute DLL path for LoadLibraryA), and how to build and
  test it with fabricare. Use when running, scripting, documenting, reviewing
  or changing dll-inject, when injecting a DLL into a program at launch on
  Windows, or when working inside the dll-inject repository (source under
  XYO/DllInject) or with its companion payload dll-inject-sample.
---

# dll-inject

Windows command-line **DLL injector** of the XYO C++ libraries. It launches a
program with a DLL forced to load into it **before the program's entry point
runs**, so the DLL's `DllMain` executes at startup. The tool parses the command
line; the injection engine is `xyo-win-inject`
(`XYO::Win::Inject::Process::createProcessA`).

Full documentation: `docs/` in this repository
(`X:\Storage\XYO\Gitea\CPP\dll-inject\docs` on this machine): README (overview),
**usage** (every option, exit codes, examples), **how-it-works** (the
create-suspended / inject / resume mechanism, requirements, limitations),
**building** (fabricare build and tests). Read the matching page when you need
more than this summary.

## Command line

```
dll-inject --help | -h | -?          # print usage, exit 0
dll-inject --version                 # print version/build/date, exit 0
dll-inject --license                 # print MIT license, exit 0
dll-inject [mode] <dll> <app> [app-arguments...]
```

- `<dll>` — path of the DLL to inject. It is passed to `LoadLibraryA` **inside
  the target**, so use an **absolute path**; a bare name follows the target's
  DLL search order.
- `<app>` — executable to launch. Both `<dll>` and `<app>` are required.
- `[app-arguments...]` — become the launched program's command line; each token
  is wrapped in double quotes and joined with a single space.

### Mode flags (optional, one only, before `<dll>`)

| Flag | Hidden window | Realtime priority | Wait for exit |
|------|:---:|:---:|:---:|
| *(none)* | no | no | no |
| `--wait` | no | no | yes |
| `--hidden` | yes | no | no |
| `--realtime` | no | yes | no |
| `--hidden-realtime` | yes | yes | no |
| `--wait-hidden-realtime` | yes | yes | yes |

- **hidden** → `SW_HIDE`; **realtime** → `REALTIME_PRIORITY_CLASS` (use with
  care); **wait** → block until the target exits.

### Exit codes

`0` success (informational option shown, or app launched + injected; with
`--wait`, also exited). `1` invalid command line, or the process could not be
created / the DLL could not be injected. With `--wait` the code reflects
launch+injection, **not** the target's own exit code.

Examples:

```
dll-inject "C:\tools\hook.dll" "C:\Windows\System32\notepad.exe"
dll-inject --wait "C:\tools\trace.dll" "C:\app\worker.exe" --config prod
dll-inject --hidden-realtime "C:\tools\agent.dll" "C:\app\host.exe"
```

## Requirements / gotchas

- **Windows only**, built for `win64-msvc-2026` here.
- **Runs elevated** — the manifest requests `requireAdministrator`; the target
  therefore also runs elevated.
- **Architecture must match** across `dll-inject`, `<dll>` and `<app>` (the
  64-bit build injects 64-bit targets only).
- Injection is **at launch only**; the tool does not attach to a running
  process.
- A failing/crashing `DllMain` causes the engine to `TerminateProcess` the
  target and report failure.
- Injecting code into other processes is powerful and easily misused — only do
  it on software you are authorised to modify. `dll-inject-sample` (sibling
  repo) is a ready-made payload DLL for trying it.

## Source map

```
source/XYO/DllInject/Application.hpp   Application class: Action enum, Arguments struct,
                                       parseArguments, buildCommandLine, main, showUsage/License/Version
source/XYO/DllInject/Application.cpp   command line parsing + CreateProcess/inject call
source/XYO/DllInject/Version / Copyright / License   metadata strings
source/XYO/DllInject/Application.rc / .manifest / .ico   version info, requireAdministrator manifest, icon
test/test.01.cpp                       unit tests for parseArguments and buildCommandLine
fabricare.json                         build manifest (dll-inject exe + test.01)
```

`main()` flow: `parseArguments` → on `Action::ShowUsage/ShowLicense/ShowVersion`
print and return 0; on `Action::Run` set up `STARTUPINFO`
(`SW_HIDE`/`SW_SHOW`, `REALTIME_PRIORITY_CLASS`), call
`XYO::Win::Inject::Process::createProcessA(... dll)`, optionally
`WaitForSingleObject(hProcess)` when `--wait`, then close the process/thread
handles.

## Building and testing (fabricare)

From the repo root (needs `xyo-win` and `xyo-win-inject` installed in the SDK):

```
fabricare          # build output/bin/dll-inject.exe
fabricare test     # build + run test.01 (run make first)
fabricare install  # copy to ~/.fabricare/<platform>
fabricare clean
```

The test project compiles `["@test", "XYO/DllInject"]` with define
`XYO_DLLINJECT_LIBRARY` (which drops `XYO_APPLICATION_MAIN` so the test owns
`main`). It covers argument parsing and command-line building only; the actual
injection needs a real target and elevation and is verified by hand.

See the `fabricare` skill for the build tool itself, and the `xyo-system` /
`xyo-encoding` skills for `String`, `Shell` and friends used here.
