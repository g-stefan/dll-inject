# Building and testing

`dll-inject` is built with **fabricare**, the XYO C++ build tool. The manifest
is [`fabricare.json`](../fabricare.json); the tool must already be on `PATH`
(it is installed per platform under `~/.fabricare/<platform>/bin`).

## Prerequisites

The project depends on two XYO libraries that must be built and installed into
the SDK first (`fabricare install` in each):

- `xyo-win`
- `xyo-win-inject` (which itself pulls in the rest of the XYO stack)

Check `~/.fabricare/<platform>/lib` for `xyo-win.json` and
`xyo-win-inject.json` — if they are there, the dependencies are available.

## Commands

Run these from the project root (the folder with `fabricare.json`):

```
fabricare              # make (default): build output/bin/dll-inject.exe
fabricare test         # build and run the test executables
fabricare install      # copy output/ into ~/.fabricare/<platform>
fabricare clean        # remove output/ and temp/
fabricare release      # package output/ into release/*.zip + sha512 manifest
```

`fabricare test` does not rebuild the tool, so run `fabricare make` first after
changing the sources.

## The test project

`fabricare.json` defines a `test.01` project (category `test`). It compiles the
`test/` folder together with the application sources, defining
`XYO_DLLINJECT_LIBRARY` so that `Application.cpp`'s `XYO_APPLICATION_MAIN` is
left out and the test supplies its own `main`:

```json
{
    "name": "test.01",
    "make": "exe",
    "category": "test",
    "sourcePath": ["@test", "XYO/DllInject"],
    "sourcePrefix": "",
    "defines": ["XYO_DLLINJECT_LIBRARY"],
    "dependency": ["xyo-win", "xyo-win-inject"]
}
```

[`test/test.01.cpp`](../test/test.01.cpp) exercises the parts of the application
that are pure logic and safe to run anywhere:

- `Application::parseArguments` — every mode flag, the informational options
  (`--help` / `-h` / `-?`, `--version`, `--license`), rejection of an unknown
  option, and rejection of a missing `<dll>` or `<app>`.
- `Application::buildCommandLine` — that arguments are quoted, separated by a
  single space, and that there is **no trailing space** after the last one.

The injection itself (`CreateProcess` + the stub) is **not** unit-tested: it
needs a real target process and elevation, so it is verified by hand. A good
manual smoke test is to inject `dll-inject-sample`'s DLL into a throwaway
process and confirm its hooks run.

## Platform notes

- Use `win64-msvc-2026` (the default) on this machine. The 64-bit build injects
  into 64-bit targets only.
- Source files use **CRLF** line endings; keep them that way when editing.
- If `fabricare` reports that `vcvarsall.bat` or `cl.exe` is "not recognized",
  clear the `NoDefaultCurrentDirectoryInExePath` environment variable for the
  build process (see the fabricare documentation).
