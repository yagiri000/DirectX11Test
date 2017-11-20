#pragma once
#include <algorithm>
#include <memory>
#include <fstream>
#include <string>

// TODO : �w�b�_�[�œǂݍ���ł���t�@�C�����C��

struct BinFile
{
	BinFile(const wchar_t* fpath);
	const void* get() const { return Bin.get(); }
	int size() const { return nSize; }
private:
	int nSize = 0;
	std::unique_ptr<char> Bin;
};