// File Crypt
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/FileCrypt/Application.hpp>
#include <XYO/FileCrypt/Copyright.hpp>
#include <XYO/FileCrypt/License.hpp>
#include <XYO/FileCrypt/Version.hpp>

namespace XYO::FileCrypt {

	void Application::showUsage() {
		printf("file-crypt - Encrypt/Decrypt file with key\n");
		showVersion();
		printf("Usage:\n\n");
		printf("\tfile-crypt --usage\n");
		printf("\tfile-crypt --license\n");
		printf("\tfile-crypt --version\n");
		printf("\tfile-crypt --encrypt --key str [input] [output]\n");
		printf("\t\t- Encrypt file [input] into [output] with key\n");
		printf("\tfile-crypt --decrypt --key str [input] [output]\n");
		printf("\t\t- Decrypt file [input] into [output] with key\n");
		printf("\tfile-crypt --encrypt --key-sha512 key [input] [output]\n");
		printf("\t\t- Encrypt file [input] into [output] with SHA512(key)\n");
		printf("\tfile-crypt --decrypt --key-sha512 key [input] [output]\n");
		printf("\t\t- Decrypt file [input] into [output] with SHA512(key)\n");
		printf("\tfile-crypt --encrypt --key-hex hex [input] [output]\n");
		printf("\t\t- Encrypt file [input] into [output] with un-hex(key)\n");
		printf("\tfile-crypt --decrypt --key-hex hex [input] [output]\n");
		printf("\t\t- Decrypt file [input] into [output] with un-hex(key)\n");
		printf("\tfile-crypt --gen-key-encrypt [key] [input] [output]\n");
		printf("\t\t- Encrypt file [input] into [output] with generated [key]\n");
		printf("\tfile-crypt --gen-key-decrypt [key] [input] [output]\n");
		printf("\t\t- Decrypt file [input] into [output] with generated [key]\n");
		printf("\tfile-crypt --encrypt --key-sha512-write key [key] [input] [output]\n");
		printf("\t\t- Encrypt file [input] into [output] with SHA512(key) and save key to [key]\n");
		printf("\tfile-crypt --encrypt --key-read [key] [input] [output]\n");
		printf("\t\t- Encrypt file [input] into [output] with key read from [key]\n");
		printf("\tfile-crypt --gen-key-sha512-write key [key]\n");
		printf("\t\t- Save SHA512(key) to [key]\n");
		printf("\tfile-crypt --extract-integrity [file] [integrity]\n");
		printf("\t\t- Save integrity check from encrypted [file] to [integrity]\n");
	};

	void Application::showLicense() {
		printf("%s", FileCrypt::License::license());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", FileCrypt::Version::version(), FileCrypt::Version::build(), FileCrypt::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		char *opt;

		bool doEncrypt = false;
		bool doDecrypt = false;

		bool keyIsHex = false;
		bool keyIsStr = false;
		bool keyIsSHA512 = false;
		bool keyIsGenerated = false;
		bool keyWrite = false;
		bool keyRead = false;
		bool keySHA512Write = false;
		String keyInput;

		bool extractIntegrity = false;

		char *fileIn = nullptr;
		char *fileOut = nullptr;
		char *fileKey = nullptr;

		if (cmdN < 2) {
			showUsage();
			return 1;
		};

		for (i = 1; i < cmdN; ++i) {
			if (strncmp(cmdS[i], "--", 2) == 0) {
				opt = &cmdS[i][2];
				if (strcmp(opt, "usage") == 0) {
					showLicense();
					if (cmdN == 2) {
						return 0;
					};
				};
				if (strcmp(opt, "license") == 0) {
					showLicense();
					if (cmdN == 2) {
						return 0;
					};
				};
				if (strcmp(opt, "version") == 0) {
					showLicense();
					if (cmdN == 2) {
						return 0;
					};
				};
				if (strcmp(opt, "encrypt") == 0) {
					doEncrypt = true;
				};
				if (strcmp(opt, "decrypt") == 0) {
					doDecrypt = true;
				};

				if (strcmp(opt, "key") == 0) {
					keyIsStr = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					keyInput = cmdS[i];
				};

				if (strcmp(opt, "key-hex") == 0) {
					keyIsHex = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					keyInput = cmdS[i];
				};

				if (strcmp(opt, "key-sha512") == 0) {
					keyIsSHA512 = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					keyInput = cmdS[i];
				};

				if (strcmp(opt, "gen-key-encrypt") == 0) {
					doEncrypt = true;
					keyIsGenerated = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					fileKey = cmdS[i];
					continue;
				};

				if (strcmp(opt, "gen-key-decrypt") == 0) {
					doDecrypt = true;
					keyIsGenerated = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					fileKey = cmdS[i];
					continue;
				};

				if (strcmp(opt, "key-sha512-write") == 0) {
					keyIsSHA512 = true;
					keyWrite = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					keyInput = cmdS[i];
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					fileKey = cmdS[i];
				};

				if (strcmp(opt, "key-read") == 0) {
					keyRead = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					fileKey = cmdS[i];
				};

				if (strcmp(opt, "gen-key-sha512-write") == 0) {
					keySHA512Write = true;
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					keyInput = cmdS[i];
					++i;
					if (i >= cmdN) {
						printf("Error: Invalid parameters\r\n");
						return 1;
					};
					fileKey = cmdS[i];
				};

				if (strcmp(opt, "extract-integrity") == 0) {
					extractIntegrity = true;
				};

				continue;
			};
			if (!fileIn) {
				fileIn = cmdS[i];
				continue;
			};
			if (!fileOut) {
				fileOut = cmdS[i];
				continue;
			};
		};

		if (extractIntegrity) {
			if ((fileIn == nullptr) || (fileOut == nullptr)) {
				printf("Error: Invalid parameters\r\n");
				return 1;
			};
			Buffer integrity;
			if (Shell::fileGetContents(fileIn, integrity)) {
				if (integrity.length >= 128) {
					if (Shell::filePutContents(fileOut, &integrity.buffer[64], 64)) {
						return 0;
					};
				};
			};
			return 1;
		};

		if (keySHA512Write) {

			if (fileKey) {
				Buffer key;
				key.setSize(64);
				key.length = key.size;
				SHA512::hashToU8(keyInput, key.buffer);
				if (Shell::filePutContents(fileKey, key.buffer, 64)) {
					return 0;
				};
				return 1;
			};

			printf("Error: Invalid parameters\r\n");
			return 1;
		};

		if ((fileIn == nullptr) || (fileOut == nullptr)) {
			printf("Error: Invalid parameters\r\n");
			return 1;
		};

		if ((keyIsGenerated) && (fileKey != nullptr) && (doEncrypt || doDecrypt)) {
			if (doEncrypt) {
				Buffer fileInBuffer;
				if (Shell::fileGetContents(fileIn, fileInBuffer)) {
					SHA512 randomKey;
					uint8_t randomKeyData[64];
					randomKey.processInit();
					randomKey.processU8(fileInBuffer.buffer, fileInBuffer.length);
					UConvert::u64ToU8(DateTime::timestampInMilliseconds(), randomKeyData);
					randomKey.processU8(randomKeyData, 8);
					randomKey.processDone();
					randomKey.toU8(randomKeyData);
					if (Shell::filePutContents(fileKey, randomKeyData, 64)) {
						if (Crypt::encryptFile(randomKeyData, 64, fileIn, fileOut)) {
							return 0;
						};
					};
				};
			};
			if (doDecrypt) {
				Buffer fileKeyBuffer;
				if (Shell::fileGetContents(fileKey, fileKeyBuffer)) {
					if (Crypt::decryptFile(fileKeyBuffer.buffer, fileKeyBuffer.length, fileIn, fileOut)) {
						return 0;
					};
				};
			};
			return 1;
		};

		if ((keyIsHex || keyIsStr || keyIsSHA512 || keyRead) && (keyInput.length() > 0) && (doEncrypt || doDecrypt)) {
			Buffer key;
			if (keyIsHex) {
				key.fromHex(keyInput);
			};
			if (keyIsStr) {
				key.fromString(keyInput);
			};
			if (keyIsSHA512) {
				key.setSize(64);
				key.length = key.size;
				SHA512::hashToU8(keyInput, key.buffer);
			};
			if (keyWrite) {
				if (!Shell::filePutContents(fileKey, key.buffer, 64)) {
					return 1;
				};
			};
			if (keyRead) {
				if (!Shell::fileGetContents(fileKey, key)) {
					return 1;
				};
			};

			if (doEncrypt) {
				if (Crypt::encryptFile(key.buffer, key.length, fileIn, fileOut)) {
					return 0;
				};
				return 1;
			};

			if (doDecrypt) {
				if (Crypt::decryptFile(key.buffer, key.length, fileIn, fileOut)) {
					return 0;
				};
				return 1;
			};
		};

		printf("Error: Invalid parameters\r\n");
		return 1;
	};
};

#ifndef XYO_FILECRYPT_LIBRARY
XYO_APPLICATION_MAIN(XYO::FileCrypt::Application);
#endif
