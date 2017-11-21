#include "pch.h"
#include "Binfile.h"


BinFile::BinFile(const wchar_t* fpath)
{
	std::ifstream binfile(fpath, std::ios::in | std::ios::binary);

	if (binfile.is_open()) {
		int fsize = static_cast<int>(binfile.seekg(0, std::ios::end).tellg());
		binfile.seekg(0, std::ios::beg);
		std::unique_ptr<char> code(new char[fsize]);
		binfile.read(code.get(), fsize);
		nSize = fsize;
		Bin = std::move(code);
	}
}