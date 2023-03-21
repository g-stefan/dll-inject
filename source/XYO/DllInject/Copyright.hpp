// Dll Inject
// Copyright (c) 2008-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2008-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_DLLINJECT_COPYRIGHT_HPP
#define XYO_DLLINJECT_COPYRIGHT_HPP

#ifndef XYO_DLLINJECT_DEPENDENCY_HPP
#	include <XYO/DllInject/Dependency.hpp>
#endif

namespace XYO::DllInject::Copyright {
	std::string copyright();
	std::string publisher();
	std::string company();
	std::string contact();
};

#endif
