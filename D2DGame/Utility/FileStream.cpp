#include "stdafx.h"
#include "FileStream.h"

FileStream::FileStream(const std::string & path, const uint & file_stream_flags)
	: flags(file_stream_flags)
{
	int ios_flags = std::ios::binary;
	ios_flags |= (flags & FILE_STREAM_READ) ? std::ios::in : 0U;
	ios_flags |= (flags & FILE_STREAM_WRITE) ? std::ios::out : 0U;
	ios_flags |= (flags & FILE_STREAM_APPEND) ? std::ios::app : 0U;

	if (flags & FILE_STREAM_READ)
	{
		f_in.open(path, ios_flags);
		if (f_in.fail())
		{
			LOG_ERROR_F("Failed to open \"%s\" for reading", path.c_str());
			return;
		}
	}
	else if (flags & FILE_STREAM_WRITE)
	{
		f_out.open(path, ios_flags);
		if (f_out.fail())
		{
			LOG_ERROR_F("Failed to open \"%s\" for writing", path.c_str());
			return;
		}
	}

	is_open = true;
}

FileStream::~FileStream()
{
	Close();
}

void FileStream::Close()
{
	if (flags & FILE_STREAM_READ)
	{
		f_in.clear();
		f_in.close();
	}
	else if (flags & FILE_STREAM_WRITE)
	{
		f_out.flush();
		f_out.close();
	}
}

void FileStream::Skip(const uint & n)
{
	if (flags & FILE_STREAM_READ)
		f_in.ignore(n, std::ios::cur);
	else if (flags & FILE_STREAM_WRITE)
		f_out.seekp(n, std::ios::cur);
}

void FileStream::Write(const void * value, const uint & size)
{
	f_out.write(reinterpret_cast<const char*>(value), size);
}

void FileStream::Write(const std::string & value)
{
	const auto length = static_cast<uint>(value.length());
	Write(length);

	f_out.write(value.c_str(), length);
}

void FileStream::Write(const std::vector<std::byte>& value)
{
	const auto length = static_cast<uint>(value.size());
	Write(length);

	f_out.write(reinterpret_cast<const char*>(value.data()), length);
}

void FileStream::Read(std::string & value)
{
	auto length = Read<uint>();

	value.clear();
	value.shrink_to_fit();

	value.reserve(length);
	value.resize(length);

	f_in.read(value.data(), length);
}

void FileStream::Read(std::vector<std::byte>& value)
{
	auto length = Read<uint>();

	value.clear();
	value.shrink_to_fit();

	value.reserve(length);
	value.resize(length);

	f_in.read(reinterpret_cast<char*>(value.data()), length);
}