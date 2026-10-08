# Usage

```
dll-inject --help
dll-inject --version
dll-inject --license
dll-inject [--wait] <dll> <app> [app-arguments...]
dll-inject [--wait-hidden-realtime] <dll> <app> [app-arguments...]
dll-inject [--hidden-realtime] <dll> <app> [app-arguments...]
dll-inject [--hidden] <dll> <app> [app-arguments...]
dll-inject [--realtime] <dll> <app> [app-arguments...]
```

Running `dll-inject` with no arguments (or with an argument it does not
understand) prints the usage and exits with code `1`.

## Positional arguments

| Argument | Meaning |
|----------|---------|
| `<dll>` | Path of the DLL to inject. It is handed to `LoadLibraryA` **inside the target process**, so an absolute path is safest; a bare file name is resolved by the target's DLL search order, not the current directory. |
| `<app>` | Path of the executable to launch. |
| `[app-arguments...]` | Everything after `<app>` becomes the launched program's command line. Each token is wrapped in double quotes, so values containing spaces survive, and they are joined with single spaces. |

Both `<dll>` and `<app>` are required. A mode flag on its own, or a `<dll>`
without an `<app>`, is rejected (usage is printed, exit code `1`).

## Mode flags

A single, optional mode flag may appear **before** `<dll>`. The forms are
mutually exclusive — pick the one that combines the behaviours you want.

| Flag | Hidden window | Realtime priority | Wait for exit |
|------|:-------------:|:-----------------:|:-------------:|
| *(none)* | no | no | no |
| `--wait` | no | no | **yes** |
| `--hidden` | **yes** | no | no |
| `--realtime` | no | **yes** | no |
| `--hidden-realtime` | **yes** | **yes** | no |
| `--wait-hidden-realtime` | **yes** | **yes** | **yes** |

- **Hidden** starts the application with `SW_HIDE` (`STARTF_USESHOWWINDOW`), so
  a GUI/console window does not pop up. Processes that create their own windows
  explicitly may still show them.
- **Realtime** creates the process in `REALTIME_PRIORITY_CLASS`. Use with care:
  a busy realtime process can starve the rest of the system.
- **Wait** makes `dll-inject` block until the launched application exits, then
  return. Without it, `dll-inject` returns as soon as the application has been
  started and injected.

## Informational options

Each is used on its own, as the only argument:

| Option | Effect |
|--------|--------|
| `--help`, `-h`, `-?` | Print the usage and exit `0`. |
| `--version` | Print the version, build number and build date, then exit `0`. |
| `--license` | Print the full MIT license text and exit `0`. |

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | Success: an informational option was shown, or the application was launched (and, with `--wait`, has since exited). |
| `1` | The command line was invalid, or the process could not be created / the DLL could not be injected. |

> Note: with `--wait` the exit code reports whether the **launch and injection**
> succeeded, not the launched application's own exit code.

## Examples

Inject `hook.dll` into Notepad and return immediately:

```
dll-inject "C:\tools\hook.dll" "C:\Windows\System32\notepad.exe"
```

Inject into an application, pass arguments to it, and wait until it closes:

```
dll-inject --wait "C:\tools\trace.dll" "C:\app\worker.exe" --config prod --verbose
```

Start a background job hidden and at realtime priority:

```
dll-inject --hidden-realtime "C:\tools\agent.dll" "C:\app\service-host.exe"
```

Show the version:

```
dll-inject --version
```

## Common pitfalls

- **Wrong architecture.** Injecting a 32-bit DLL into a 64-bit target (or
  launching a 32-bit target from a 64-bit `dll-inject`) fails. Match the
  bitness of `dll-inject`, `<dll>` and `<app>`.
- **DLL not found at load time.** `LoadLibraryA` runs inside the target; if you
  pass a bare name it is searched relative to the target's directories, not your
  shell's current directory. Pass a full path to avoid surprises.
- **A faulty `DllMain`.** If the injected DLL's `DllMain` returns `FALSE` or
  crashes, the host process is terminated by the injection engine; the tool then
  reports failure (exit `1`).
- **Elevation.** The process is launched from an elevated `dll-inject`; the
  target therefore also runs elevated.
