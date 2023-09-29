#ifndef FILE_DIALOG_HPP
#define FILE_DIALOG_HPP


#include <string>


class FileDialog
{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
};


#endif /*FILE_DIALOG_HPP*/