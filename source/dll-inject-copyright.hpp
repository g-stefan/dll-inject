//
// Dll Inject
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef DLL_INJECT_COPYRIGHT_HPP
#define DLL_INJECT_COPYRIGHT_HPP

#define DLL_INJECT_COPYRIGHT "Copyright (c) Grigore Stefan"
#define DLL_INJECT_PUBLISHER "Grigore Stefan"
#define DLL_INJECT_COMPANY DLL_INJECT_PUBLISHER
#define DLL_INJECT_CONTACT "g_stefan@yahoo.com"
#define DLL_INJECT_FULL_COPYRIGHT DLL_INJECT_COPYRIGHT " <" DLL_INJECT_CONTACT ">"

#ifndef XYO_RC

namespace DllInject {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
