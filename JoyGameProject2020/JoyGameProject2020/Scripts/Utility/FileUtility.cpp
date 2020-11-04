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
	/* 呼び出し1回目(文字列数を得る) */
	auto num1 = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(), -1, nullptr, 0);

	std::wstring wstr;/* stringのwchar_t版 */
	wstr.resize(num1);/* 得られた文字列数でリサイズ */

	/* 呼び出し2回目(確保済みのwstrに変換文字列をコピー) */
	auto num2 = MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED | MB_ERR_INVALID_CHARS,
		str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);/* 一応チェック */
	return wstr;
}

std::string FileUtility::GetDirectory(const std::string& path) {
	auto idx = path.rfind(L'/');
	return path.substr(0, idx + 1);
}
