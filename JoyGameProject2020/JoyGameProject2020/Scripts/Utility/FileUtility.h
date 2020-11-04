#pragma once
#include <string>

namespace FileUtility {
	/* �g���q�̎擾(string�^) */
	std::string GetExtension(const std::string& path);
	/* �g���q�̎擾(wstring�^) */
	std::wstring GetExtension(const std::wstring& path);
	/* string�^����wstring�^�ւ̕ϊ� */
	std::wstring GetWideStringFromString(const std::string& str);
	/* �p�X����f�B���N�g�������o�� */
	std::string GetDirectory(const std::string& path);
};

