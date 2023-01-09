// Dll Inject
// Copyright (c) 2008-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_DLLINJECT_VERSION_HPP
#define XYO_DLLINJECT_VERSION_HPP

#ifndef XYO_DLLINJECT_DEPENDENCY_HPP
#	include <XYO/DllInject/Dependency.hpp>
#endif

namespace XYO::DllInject::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
