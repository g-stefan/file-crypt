//
// File Crypt
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef FILE_CRYPT_COPYRIGHT_HPP
#define FILE_CRYPT_COPYRIGHT_HPP

#define FILE_CRYPT_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define FILE_CRYPT_PUBLISHER            "Grigore Stefan"
#define FILE_CRYPT_COMPANY              FILE_CRYPT_PUBLISHER
#define FILE_CRYPT_CONTACT              "g_stefan@yahoo.com"
#define FILE_CRYPT_FULL_COPYRIGHT       FILE_CRYPT_COPYRIGHT " <" FILE_CRYPT_CONTACT ">"

#ifndef XYO_RC

namespace FileCrypt {
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
