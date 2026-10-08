# Dll Inject

Inject dll in executable

`dll-inject` is a Windows command-line tool that starts a program with a DLL
already loaded inside it, before the program's entry point runs.

```
dll-inject [mode] <dll> <app> [app-arguments...]
```

Example — inject `hook.dll` into Notepad:

```
dll-inject "C:\tools\hook.dll" "C:\Windows\System32\notepad.exe"
```

Run `dll-inject --help` for the full option list.

## Documentation

See [`docs/`](docs/README.md) for the purpose, usage, the injection mechanism,
and how to build and test. An AI-assistant skill describing the tool lives in
[`.claude/skills/dll-inject/`](.claude/skills/dll-inject/SKILL.md).

## License

Copyright (c) 2008-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
