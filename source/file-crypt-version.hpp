//
// File Crypt
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef FILE_CRYPT_VERSION_HPP
#define FILE_CRYPT_VERSION_HPP

#define FILE_CRYPT_VERSION_ABCD                1,1,0,3
#define FILE_CRYPT_VERSION_STR                 "1.1.0"
#define FILE_CRYPT_VERSION_STR_BUILD           "3"
#define FILE_CRYPT_VERSION_STR_DATETIME        "2020-09-14 00:18:09"

#ifndef XYO_RC

namespace FileCrypt {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif

