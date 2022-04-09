//
// Dll Inject
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef DLL_INJECT_VERSION_HPP
#define DLL_INJECT_VERSION_HPP

#define DLL_INJECT_VERSION_ABCD 1,15,0,27
#define DLL_INJECT_VERSION_STR "1.15.0"
#define DLL_INJECT_VERSION_STR_BUILD "27"
#define DLL_INJECT_VERSION_STR_DATETIME "2022-04-04 01:45:00"

#ifndef XYO_RC

namespace DllInject {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif
