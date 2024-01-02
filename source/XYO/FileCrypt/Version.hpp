// File Crypt
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_FILECRYPT_VERSION_HPP
#define XYO_FILECRYPT_VERSION_HPP

#ifndef XYO_FILECRYPT_DEPENDENCY_HPP
#	include <XYO/FileCrypt/Dependency.hpp>
#endif

namespace XYO::FileCrypt::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
