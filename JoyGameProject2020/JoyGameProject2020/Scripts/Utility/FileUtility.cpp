#include "FileUtility.h"
#include <assert.h>
#include <Windows.h>

std::string FileUtility::GetExtension(const std::string& path) {
	auto idx = path.rfind('.');
	return path.substr(idx + 1, path.length() - idx - 1);
}

std::wstring FileUtility::GetExtension(const std::wstring& path) {
	auto idx = path.rfind(L'.');
	return path.substr(idx + 1, path.length() - idx - 1);
}

std::wstring FileUtility::GetWideStringFromString(const std::string& str) {
	/* �Ăяo��1���(�����񐔂𓾂�) */
	auto num1 = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(), -1, nullptr, 0);

	std::wstring wstr;/* string��wchar_t�� */
	wstr.resize(num1);/* ����ꂽ�����񐔂Ń��T�C�Y */

	/* �Ăяo��2���(�m�ۍς݂�wstr�ɕϊ���������R�s�[) */
	auto num2 = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);/* �ꉞ�`�F�b�N */
	return wstr;
}

std::string FileUtility::GetDirectory(const std::string& path) {
	auto idx = path.rfind(L'/');
	return path.substr(0, idx + 1);
}
