// Dll Inject
// Copyright (c) 2008-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2008-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_DLLINJECT_APPLICATION_HPP
#define XYO_DLLINJECT_APPLICATION_HPP

#ifndef XYO_DLLINJECT_DEPENDENCY_HPP
#	include <XYO/DllInject/Dependency.hpp>
#endif

namespace XYO::DllInject {

	class Application : public virtual IApplication {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

		public:
			// What the command line asks the application to do.
			enum class Action {
				ShowUsage,   // print the usage (explicitly requested with --help)
				ShowLicense, // print the license (--license)
				ShowVersion, // print the version (--version)
				Run          // inject the dll and start the application
			};

			// Result of parsing the command line.
			struct Arguments {
					Action action;
					bool doWait;
					bool doHidden;
					bool doRealtime;
					const char *dllToInject;
					String applicationToExecute;
			};

			inline Application(){};

			void showUsage();
			void showLicense();
			void showVersion();

			// Parse the command line into `arguments`.
			// Returns false when the command line is invalid (the caller should
			// print the usage and exit with a non-zero code).
			static bool parseArguments(int cmdN, char *cmdS[], Arguments &arguments);

			// Build a quoted command line from cmdS[from .. cmdN-1].
			// Each argument is wrapped in double quotes and separated by a single
			// space, with no trailing space.
			static String buildCommandLine(int cmdN, char *cmdS[], int from);

			int main(int cmdN, char *cmdS[]);

			static void initMemory();
	};

};

#endif
