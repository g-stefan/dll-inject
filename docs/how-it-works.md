# How it works

`dll-inject` itself only parses the command line. The real work is done by
`XYO::Win::Inject::Process::createProcessA` in the `xyo-win-inject` library.
The sequence is **create suspended → inject → resume**:

1. **Create the target suspended.** `CreateProcessA` is called with the
   caller's flags plus `CREATE_SUSPENDED`. The new process exists but its
   primary thread has not run a single instruction yet.

2. **Allocate a page in the target.** `VirtualAllocEx` reserves one 4 KiB page
   of read/write memory inside the target process.

3. **Build a tiny stub.** A short machine-code routine is assembled in a local
   buffer and copied into that page with `WriteProcessMemory`. The stub:
   - saves registers,
   - calls `LoadLibraryA` with the path of the DLL (the path string is stored
     in the same page),
   - restores registers,
   - jumps to the **original entry point** (the thread's saved `Eip`/`Rip`).

   The page also holds, at fixed offsets, the address of `LoadLibraryA` and the
   saved original instruction pointer. The page is then switched to
   `PAGE_EXECUTE_READ` and its instruction cache flushed.

4. **Redirect the thread.** `GetThreadContext` reads the suspended thread's
   context, its instruction pointer is saved, and `SetThreadContext` points the
   thread at the stub.

5. **Resume (unless the caller asked for suspended).** `ResumeThread` starts the
   thread; it runs the stub, which loads the DLL (running its `DllMain`) and
   then jumps to the program's real entry point. From the application's point of
   view it starts normally — but with the DLL already loaded.

If any step fails, the engine calls `TerminateProcess` on the half-created
target and returns failure, so you never end up with a running process in an
inconsistent state.

## Why `LoadLibraryA`'s address is reused

The stub uses the address of `LoadLibraryA` obtained in the *injector's* own
`kernel32.dll`. This works because Windows maps `kernel32.dll` at the same base
address in every process for the life of a boot session, so the address is
valid in the target too.

## Requirements

- **Same architecture** for `dll-inject`, the target executable and the DLL.
  The 64-bit build injects a 64-bit stub and reads `Rip`; it cannot drive a
  32-bit target. The library contains both a 32-bit and a 64-bit stub; which one
  is compiled depends on the platform `dll-inject` was built for.
- **Rights to create the process and write its memory.** The tool's manifest
  requests administrator elevation, which covers launching and injecting most
  targets.
- The DLL must be loadable by `LoadLibraryA` from inside the target (correct
  path, dependencies resolvable, matching bitness).

## Limitations

- Injection happens **only at process creation**. This tool launches the target
  itself; it does not attach to an already-running process.
- The DLL's `DllMain` runs on the process's primary thread before the real entry
  point. Keep `DllMain` work minimal and loader-lock safe, as with any DLL.
- `--wait` reports only whether launch + injection succeeded, not the target's
  own exit code (see [usage](usage.md)).
- The stub and its data share a single 4 KiB page; an absurdly long DLL path
  (approaching 4 KiB) is rejected by the engine.

## Where the code lives

| Concern | File |
|---------|------|
| Command line parsing, option handling, usage | [`source/XYO/DllInject/Application.cpp`](../source/XYO/DllInject/Application.cpp) |
| The declared option/argument model | [`source/XYO/DllInject/Application.hpp`](../source/XYO/DllInject/Application.hpp) |
| Version / copyright / license strings | `source/XYO/DllInject/Version.cpp`, `Copyright.cpp`, `License.cpp` |
| The injection engine (`createProcessA`, `injectDllDirect`) | `xyo-win-inject` repository, `source/XYO/WinInject/Process.cpp` |
