// Dll Inject
// Copyright (c) 2008-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2008-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/DllInject/Copyright.hpp>
#include <XYO/DllInject/Copyright.rh>

namespace XYO::DllInject::Copyright {

	static const char *copyright_ = XYO_DLLINJECT_COPYRIGHT;
	static const char *publisher_ = XYO_DLLINJECT_PUBLISHER;
	static const char *company_ = XYO_DLLINJECT_COMPANY;
	static const char *contact_ = XYO_DLLINJECT_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
