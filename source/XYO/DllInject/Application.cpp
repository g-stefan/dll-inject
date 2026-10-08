// Dll Inject
// Copyright (c) 2008-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2008-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SECURITY_WIN32
#	define SECURITY_WIN32
#endif

#include <windows.h>
#include <objbase.h>
#include <iads.h>
#include <adshlp.h>
#include <wininet.h>
#include <Iptypes.h>
#include <Iphlpapi.h>
#include <Security.h>
#include <ole2.h>
#include <shlobj.h>
#include <Shobjidl.h>
#include <docobj.h>
#include <mshtml.h>
#include <MsHtmHst.h>
#include <exdisp.h>
#include <exdispid.h>
#include <servprov.h>

#include <XYO/WinInject.hpp>
#include <XYO/DllInject/Application.hpp>
#include <XYO/DllInject/Copyright.hpp>
#include <XYO/DllInject/License.hpp>
#include <XYO/DllInject/Version.hpp>

namespace XYO::DllInject {

	void Application::showUsage() {
		printf("dll-inject - Inject dll in executable\n");
		printf("version %s build %s [%s]\n", DllInject::Version::version(), DllInject::Version::build(), DllInject::Version::datetime());
		printf("%s\n\n", DllInject::Copyright::copyright());
		printf("\n");
		printf("Usage:\n");
		printf("\tdll-inject --help\n");
		printf("\tdll-inject --version\n");
		printf("\tdll-inject --license\n");
		printf("\tdll-inject [--wait] dll app [app.cmd]\n");
		printf("\tdll-inject [--wait-hidden-realtime] dll app [app.cmd]\n");
		printf("\tdll-inject [--hidden-realtime] dll app [app.cmd]\n");
		printf("\tdll-inject [--hidden] dll app [app.cmd]\n");
		printf("\tdll-inject [--realtime] dll app [app.cmd]\n\n");
	};

	void Application::showLicense() {
		printf("%s", DllInject::License::license().c_str());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", DllInject::Version::version(), DllInject::Version::build(), DllInject::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
		TDynamicArray<String>::initMemory();
	};

	String Application::buildCommandLine(int cmdN, char *cmdS[], int from) {
		String commandLine;
		int index;
		for (index = from; index < cmdN; ++index) {
			if (index > from) {
				commandLine << " ";
			};
			commandLine << "\"" << cmdS[index] << "\"";
		};
		return commandLine;
	};

	bool Application::parseArguments(int cmdN, char *cmdS[], Arguments &arguments) {
		int index;

		arguments.action = Action::Run;
		arguments.doWait = false;
		arguments.doHidden = false;
		arguments.doRealtime = false;
		arguments.dllToInject = nullptr;
		arguments.applicationToExecute = "";

		index = 1;
		if (index >= cmdN) {
			// no arguments
			return false;
		};

		// Informational options, each used on its own.
		if (strcmp(cmdS[index], "--license") == 0) {
			arguments.action = Action::ShowLicense;
			return true;
		};
		if (strcmp(cmdS[index], "--version") == 0) {
			arguments.action = Action::ShowVersion;
			return true;
		};
		if ((strcmp(cmdS[index], "--help") == 0) || (strcmp(cmdS[index], "-h") == 0) || (strcmp(cmdS[index], "-?") == 0)) {
			arguments.action = Action::ShowUsage;
			return true;
		};

		// Optional mode flag; the forms are mutually exclusive.
		if (strcmp(cmdS[index], "--wait") == 0) {
			arguments.doWait = true;
			++index;
		} else if (strcmp(cmdS[index], "--wait-hidden-realtime") == 0) {
			arguments.doWait = true;
			arguments.doHidden = true;
			arguments.doRealtime = true;
			++index;
		} else if (strcmp(cmdS[index], "--hidden-realtime") == 0) {
			arguments.doHidden = true;
			arguments.doRealtime = true;
			++index;
		} else if (strcmp(cmdS[index], "--hidden") == 0) {
			arguments.doHidden = true;
			++index;
		} else if (strcmp(cmdS[index], "--realtime") == 0) {
			arguments.doRealtime = true;
			++index;
		} else if (strncmp(cmdS[index], "--", 2) == 0) {
			// Unknown option: reject instead of treating it as a dll path.
			return false;
		};

		// A dll and an application (at least one token) are both required.
		if ((cmdN - index) < 2) {
			return false;
		};

		arguments.dllToInject = cmdS[index];
		++index;

		arguments.applicationToExecute = buildCommandLine(cmdN, cmdS, index);
		return true;
	};

	int Application::main(int cmdN, char *cmdS[]) {
		STARTUPINFO sInfo;
		PROCESS_INFORMATION pInfo;
		DWORD cwFlags;
		BOOL retVal;
		Arguments arguments;

		if (!parseArguments(cmdN, cmdS, arguments)) {
			showUsage();
			return 1;
		};

		switch (arguments.action) {
			case Action::ShowUsage:
				showUsage();
				return 0;
			case Action::ShowLicense:
				showLicense();
				return 0;
			case Action::ShowVersion:
				showVersion();
				return 0;
			case Action::Run:
				break;
		};

		ZeroMemory(&sInfo, sizeof(sInfo));
		ZeroMemory(&pInfo, sizeof(pInfo));
		sInfo.cb = sizeof(STARTUPINFO);
		cwFlags = 0;

		sInfo.dwFlags |= STARTF_USESHOWWINDOW;
		if (arguments.doHidden) {
			sInfo.wShowWindow = SW_HIDE;
		} else {
			sInfo.wShowWindow = SW_SHOW;
		};

		if (arguments.doRealtime) {
			cwFlags = REALTIME_PRIORITY_CLASS;
		};

		retVal = XYO::Win::Inject::Process::createProcessA(0, (char *)arguments.applicationToExecute.value(), 0, 0, FALSE, cwFlags, 0, 0, &sInfo, &pInfo, arguments.dllToInject);
		if (!retVal) {
			return 1;
		};

		if (arguments.doWait) {
			// Wait for the injected application to exit, not just its primary
			// thread.
			WaitForSingleObject(pInfo.hProcess, INFINITE);
		};

		CloseHandle(pInfo.hThread);
		CloseHandle(pInfo.hProcess);

		return 0;
	};
};

#ifndef XYO_DLLINJECT_LIBRARY
XYO_APPLICATION_MAIN(XYO::DllInject::Application);
#endif
