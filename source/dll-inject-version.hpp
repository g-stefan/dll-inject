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

#define DLL_INJECT_VERSION_ABCD                1,6,0,20
#define DLL_INJECT_VERSION_STR                 "1.6.0"
#define DLL_INJECT_VERSION_STR_BUILD           "20"
#define DLL_INJECT_VERSION_STR_DATETIME        "2021-08-09 17:38:07"

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

