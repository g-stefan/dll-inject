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

#define DLL_INJECT_VERSION_ABCD                1,13,0,25
#define DLL_INJECT_VERSION_STR                 "1.13.0"
#define DLL_INJECT_VERSION_STR_BUILD           "25"
#define DLL_INJECT_VERSION_STR_DATETIME        "2022-01-09 01:13:16"

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

