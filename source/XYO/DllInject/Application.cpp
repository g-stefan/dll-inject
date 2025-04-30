// Dll Inject
// Copyright (c) 2008-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2008-2025 Grigore Stefan <g_stefan@yahoo.com>
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

	int Application::main(int cmdN, char *cmdS[]) {
		STARTUPINFO sInfo;
		PROCESS_INFORMATION pInfo;
		DWORD cwFlags;
		bool retVal;

		char *dllToInject;
		int index;
		String applicationToExecute;
		bool doWait;
		bool doHidden;
		bool doRealtime;

		doWait = false;
		doHidden = false;
		doRealtime = false;

		if (cmdN == 2) {
			if (strcmp(cmdS[1], "--license") == 0) {
				printf("%s", DllInject::License::license().c_str());
				return 0;
			};
			return 1;
		};

		if (cmdN < 3) {
			showUsage();
			return 1;
		};
		index = 1;
		if (strcmp(cmdS[index], "--wait") == 0) {
			if (cmdN < 4) {
				showUsage();
				return 1;
			};
			index++;
			doWait = true;
		};
		if (strcmp(cmdS[index], "--wait-hidden-realtime") == 0) {
			if (cmdN < 4) {
				showUsage();
				return 1;
			};
			index++;
			doWait = true;
			doHidden = true;
			doRealtime = true;
		};
		if (strcmp(cmdS[index], "--hidden-realtime") == 0) {
			if (cmdN < 4) {
				showUsage();
				return 1;
			};
			index++;
			doHidden = true;
			doRealtime = true;
		};
		if (strcmp(cmdS[index], "--hidden") == 0) {
			if (cmdN < 4) {
				showUsage();
				return 1;
			};
			index++;
			doHidden = true;
		};
		if (strcmp(cmdS[index], "--realtime") == 0) {
			if (cmdN < 4) {
				showUsage();
				return 1;
			};
			index++;
			doRealtime = true;
		};

		dllToInject = cmdS[index++];
		applicationToExecute = "";
		for (; index < cmdN; index++) {
			applicationToExecute << "\"" << cmdS[index] << "\"";
			if (index < cmdN) {
				applicationToExecute << " ";
			};
		};

		ZeroMemory(&sInfo, sizeof(sInfo));
		ZeroMemory(&pInfo, sizeof(pInfo));
		sInfo.cb = sizeof(STARTUPINFO);
		cwFlags = 0;

		if (doHidden) {
			sInfo.dwFlags |= STARTF_USESHOWWINDOW;
			sInfo.wShowWindow = SW_HIDE;
		} else {
			sInfo.dwFlags |= STARTF_USESHOWWINDOW;
			sInfo.wShowWindow = SW_SHOW;
		};

		if (doRealtime) {
			cwFlags = REALTIME_PRIORITY_CLASS;
		};

		retVal = XYO::Win::Inject::Process::createProcessA(0, (char *)applicationToExecute.value(), 0, 0, FALSE, cwFlags, 0, 0, &sInfo, &pInfo, dllToInject);
		if (retVal) {
			if (doWait) {
				WaitForSingleObject(pInfo.hThread, INFINITE);
			};
		} else {
			return 1;
		};

		return 0;
	};
};

#ifndef XYO_DLLINJECT_LIBRARY
XYO_APPLICATION_MAIN(XYO::DllInject::Application);
#endif
