#pragma once
#include <string>

namespace FileUtility {
	/* 拡張子の取得(string型) */
	std::string GetExtension(const std::string& path);
	/* 拡張子の取得(wstring型) */
	std::wstring GetExtension(const std::wstring& path);
	/* string型からwstring型への変換 */
	std::wstring GetWideStringFromString(const std::string& str);
	/* パスからディレクトリを取り出す */
	std::string GetDirectory(const std::string& path);


};

