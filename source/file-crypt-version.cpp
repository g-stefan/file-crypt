//
// File Crypt
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "file-crypt-version.hpp"

namespace FileCrypt {
	namespace Version {

		static const char *version_ = "1.0.0";
		static const char *build_ = "2";
		static const char *versionWithBuild_ = "1.0.0.2";
		static const char *datetime_ = "2020-02-24 03:06:44";

		const char *version() {
			return version_;
		};
		const char *build() {
			return build_;
		};
		const char *versionWithBuild() {
			return versionWithBuild_;
		};
		const char *datetime() {
			return datetime_;
		};

	};
};

