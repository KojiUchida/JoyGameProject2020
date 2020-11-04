#pragma once
#include <string>

namespace FileUtility {
	/* Šg’£q‚Ìæ“¾(stringŒ^) */
	std::string GetExtension(const std::string& path);
	/* Šg’£q‚Ìæ“¾(wstringŒ^) */
	std::wstring GetExtension(const std::wstring& path);
	/* stringŒ^‚©‚çwstringŒ^‚Ö‚Ì•ÏŠ· */
	std::wstring GetWideStringFromString(const std::string& str);
	/* ƒpƒX‚©‚çƒfƒBƒŒƒNƒgƒŠ‚ğæ‚èo‚· */
	std::string GetDirectory(const std::string& path);
};

