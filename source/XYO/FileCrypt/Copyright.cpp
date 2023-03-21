// File Crypt
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileCrypt/Copyright.hpp>
#include <XYO/FileCrypt/Copyright.rh>

namespace XYO::FileCrypt::Copyright {

	static const char *copyright_ = XYO_FILECRYPT_COPYRIGHT;
	static const char *publisher_ = XYO_FILECRYPT_PUBLISHER;
	static const char *company_ = XYO_FILECRYPT_COMPANY;
	static const char *contact_ = XYO_FILECRYPT_CONTACT;

	std::string copyright() {
		return copyright_;
	};

	std::string publisher() {
		return publisher_;
	};

	std::string company() {
		return company_;
	};

	std::string contact() {
		return contact_;
	};

};
