//
// Dll Inject
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef DLL_INJECT_VERSION_HPP
#define DLL_INJECT_VERSION_HPP

#define DLL_INJECT_VERSION_ABCD                1,3,0,7
#define DLL_INJECT_VERSION_STR                 "1.3.0"
#define DLL_INJECT_VERSION_STR_BUILD           "7"
#define DLL_INJECT_VERSION_STR_DATETIME        "2021-03-15 12:30:24"

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

