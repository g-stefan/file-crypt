// File Crypt
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileCrypt/Copyright.hpp>
#include <XYO/FileCrypt/Copyright.rh>

namespace XYO::FileCrypt::Copyright {

	static const char *copyright_ = XYO_FILECRYPT_COPYRIGHT;
	static const char *publisher_ = XYO_FILECRYPT_PUBLISHER;
	static const char *company_ = XYO_FILECRYPT_COMPANY;
	static const char *contact_ = XYO_FILECRYPT_CONTACT;
	static const char *fullCopyright_ = XYO_FILECRYPT_FULL_COPYRIGHT;

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

	const char *fullCopyright() {
		return fullCopyright_;
	};

};
