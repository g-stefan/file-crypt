// File Crypt
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILECRYPT_LICENSE_HPP
#define XYO_FILECRYPT_LICENSE_HPP

#ifndef XYO_FILECRYPT_DEPENDENCY_HPP
#	include <XYO/FileCrypt/Dependency.hpp>
#endif

namespace XYO::FileCrypt::License {

	const char *licenseHeader();
	const char *licenseBody();
	const char *shortLicense();

};

#endif