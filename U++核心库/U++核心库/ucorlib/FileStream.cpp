#include "FileStream.h"
#include <exception>
#include <string>
#include "OSHelper.h"
#include "WindowsEnvironment.h"

using namespace System::IO;
using namespace System::OS;

FileStream::FileStream() : _fileHandle(INVALID_API_PTR), _autoClose(false), _buffer(nullptr)
{
	_autoClose = false;
}

FileStream::FileStream(FileHandle hFile, bool autoClose) : _buffer(nullptr)
{
	Attach(hFile, autoClose);
}

FileStream::~FileStream()
{
	Detach();
}

FileStream::operator FileHandle()
{
	return _fileHandle;
}

bool FileStream::Open(const wchar_t* filePath, FileAdvancedAccess desiredAccess, FileShare shareMode, FileCreationDisposition creationDistribution, int bufferSize, bool nothrow, FileSecurityHandle lpSecurityAttributes, FileAttributes attributes, FileHandle templateFileHandle)
{
	FileHandle hFile = Microsoft::winapi_CreateFile(filePath, (API_UINT32)desiredAccess, (API_UINT32)shareMode, lpSecurityAttributes, (API_UINT32)creationDistribution, (API_UINT32)attributes, templateFileHandle);

	bool bSuccess = false;
	if (hFile != INVALID_API_PTR)
	{
		bSuccess = true;
		_fileHandle = hFile;
		_path = filePath;
		_autoClose = true;
		_type =(FileType)Microsoft::winapi_GetFileType(_fileHandle);


		_canRead = Enums::Contains(desiredAccess, FileAdvancedAccess::File_Read_Data) || desiredAccess == FileAdvancedAccess::GenericRead || desiredAccess == FileAdvancedAccess::GenericAll;
		_canWrite = Enums::Contains(desiredAccess, FileAdvancedAccess::File_Write_Data) || desiredAccess == FileAdvancedAccess::GenericWrite || desiredAccess == FileAdvancedAccess::GenericAll;
		_canSeek = _type == FileType::Disk;
		_bufferSize = bufferSize;
		_readPos = 0;
		_readLen = 0;
		_writePos = 0; 
		_isPipe = _type == FileType::Pipe; 
		_pos = 0; 
		_appendStart = -1LL;
	}
	else
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
	}
	return bSuccess;
}

bool FileStream::Open(const wchar_t* filePath, FileAccess desiredAccess, FileShare shareMode, FileMode mode, int bufferSize, bool nothrow, FileSecurityHandle lpSecurityAttributes, FileAttributes attributes, FileHandle templateFileHandle)
{
	FileAdvancedAccess desiredAccessEx = FileAdvancedAccess::File_Generic__ReadWrite;
	switch (desiredAccess)
	{
		case FileAccess::Read:
		{
			desiredAccessEx = FileAdvancedAccess::File_Generic__Read;
			_canRead = true;
			_canWrite = false;
			break;
		}
		case FileAccess::Write:
		{
			desiredAccessEx = FileAdvancedAccess::File_Generic__Write; 
		    _canRead = false;
			_canWrite = true;
			break;
		}
		case FileAccess::All:
		default:
		{
			_canRead = true; 
			_canWrite = true;
			break;
		}	
	}
	bool flag = false;
	if (mode == FileMode::Append)
	{
		mode = FileMode::OpenOrCreate;
		flag = true;
	}

	FileHandle hFile = Microsoft::winapi_CreateFile(filePath, (API_UINT32)desiredAccessEx, (API_UINT32)shareMode, lpSecurityAttributes, (API_UINT32)mode, (API_UINT32)attributes, templateFileHandle);

	bool bSuccess = false;
	if (hFile != INVALID_API_PTR)
	{
		bSuccess = true;
		_fileHandle = hFile;
		_path = filePath;
		_autoClose = true;
		_type = (FileType)Microsoft::winapi_GetFileType(_fileHandle);

		_canSeek = _type == FileType::Disk;
		_bufferSize = bufferSize;
		_readPos = 0;
		_readLen = 0;
		_writePos = 0;
		_isPipe = _type == FileType::Pipe;
		_pos = 0;
		if (!flag)
		{
			_appendStart = -1LL;
			return true;
		}
		_appendStart = SeekCore(0LL, SeekOrigin::End, nothrow);
	}
	else
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
	}
	return bSuccess;
}

inline bool FileStream::IsOpen() const
{
	return (_fileHandle != INVALID_API_PTR);
}

inline bool FileStream::IsClosed() const
{
	return (_fileHandle == INVALID_API_PTR);
}

bool FileStream::CanRead() const
{
	return _canRead;
}

bool FileStream::CanWrite() const
{
	return _canWrite;
}

bool FileStream::CanSeek() const
{
	return _canSeek;
}

void FileStream::Attach(FileHandle hFile, bool autoClose)
{
	_fileHandle = hFile;
	_path.clear(); 
	_autoClose = autoClose;
}

void FileStream::Detach()
{
	if (IsOpen() && _autoClose)
		Close();
	else
	{
		_fileHandle = INVALID_API_PTR;
		_autoClose = false;
		_path.clear();
	}
	if (_buffer)
	{
		delete _buffer;
		_buffer = nullptr;
	}
}

void FileStream::Close(bool nothrow)
{
	if (!IsOpen()) return;

	bool bError = !Microsoft::winapi_CloseHandle(_fileHandle);

	_fileHandle = INVALID_API_PTR;
	_autoClose = false;
	_path.clear();

	if (bError && !nothrow)
		throw OSHelper::GetLastErrorException();
	if (_buffer)
	{
		delete _buffer;
		_buffer = nullptr;
	}
}

bool FileStream::Write(char* buffer, ULong size, ULong offset, ULong count, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	if (buffer == nullptr)
	{
		if (!nothrow) throw std::exception("empty buffer");
		return false;
	}

	if (offset + count > size)
	{
		if (!nothrow) throw std::exception("buffer overflow");
		return false;
	}

	if (_writePos == 0)
	{
		if (!_canWrite)
		{
			if (!nothrow) throw std::exception("can not write");
			return false;
		}
		if (_readPos < _readLen)
		{
			FlushRead();
		}
		_readPos = 0;
		_readLen = 0;
	}
	if (_writePos > 0)
	{
		long num = _bufferSize - _writePos;
		if (num > 0)
		{
			if (num > count)
			{
				num = count;
			}
			memcpy(_buffer + _writePos, buffer + offset, num);
			_writePos += num;
			if (count == num)
			{
				return true;
			}
			offset += num;
			count  -= num;
		}

		WriteCore(_buffer, _bufferSize, 0, _writePos, nothrow);
		_writePos = 0;
	}
	if (count >= _bufferSize)
	{
		WriteCore(buffer, size, offset, count);
		return true;
	}
	if (count == 0)
	{
		return true;
	}
	if (_buffer == nullptr)
	{
		_buffer = new char[_bufferSize];
	}
	memcpy(_buffer + _writePos, buffer + offset, count);
	_writePos = count;
	return true;
}

bool FileStream::WriteByte(int byte, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return 0;
	}

	if (_writePos == 0)
	{
		if (!_canWrite)
		{
			if (!nothrow) throw std::exception("can not write");
			return false;
		}
		if (_readPos < _readLen)
		{
			FlushRead();
		}
		_readPos = 0;
		_readLen = 0;
		if (_buffer == nullptr)
		{
			_buffer = new char[_bufferSize];
		}
	}
	if (_writePos == _bufferSize)
	{
		FlushWrite(nothrow);
	}
	_buffer[_writePos] = byte;
	_writePos++;
	return true;
}

ULong FileStream::Read(char* buffer, ULong size, ULong offset, ULong count, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return 0;
	}

	if (buffer == nullptr)
	{
		if (!nothrow) throw std::exception("empty buffer");
		return false;
	}

	if (offset + count > size)
	{
		if (!nothrow) throw std::exception("buffer overflow");
		return false;
	}

	bool flag = false;
	ULong num = (ULong)(_readLen - _readPos);
	if (num == 0)
	{
		if (!_canRead)
		{
			if (!nothrow) throw std::exception("can not read");
			return 0UL;
		}
		if (_writePos > 0UL)
		{
			FlushWrite(nothrow);
		}
		if (!_canSeek || count >= _bufferSize)
		{
			num = ReadCore(buffer, size, offset, count, nothrow);
			_readPos = 0;
			_readLen = 0;
			return num;
		}		
		if (_buffer == nullptr)
		{
			_buffer = new char[_bufferSize];
		}
		num = ReadCore(_buffer, _bufferSize, 0, _bufferSize, nothrow);
		if (num == 0UL)
		{
			return 0;
		}
		flag = (int)num < _bufferSize;
		_readPos = 0;
		_readLen = num;
	}
	if (num > count)
	{
		num = count;
	}
	memcpy(buffer + offset, _buffer + _readPos, num);
	_readPos += num;
	if (!_isPipe && num < count && !flag)
	{
		ULong num1 = ReadCore(buffer, size, offset + num, count - num, nothrow);
		num = num + num1;
		_readPos = 0;
		_readLen = 0;
	}
	return (ULong)num;
}

int FileStream::ReadByte(bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return -1;
	}

	if (_readLen == 0 && !_canRead)
	{
		if (!nothrow) throw std::exception("can not read");
		return -1;
	}

	if (_readPos == _readLen)
	{
		if (_writePos > 0)
		{
			FlushWrite(nothrow);
		}
		if (_buffer == nullptr)
		{
			_buffer = new char[_bufferSize];
		}
		_readLen = ReadCore(_buffer, _bufferSize, 0, _bufferSize, nothrow);
		_readPos = 0;
	}
	if (_readPos == _readLen)
	{
		return -1;
	}
	int num = (int) _buffer[_readPos];
	_readPos ++;
	return num;
}

ULong FileStream::ReadCore(char* buffer, ULong size, ULong offset, ULong count, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	if (buffer == nullptr)
	{
		if (!nothrow) throw std::exception("empty buffer");
		return false;
	}

	if (offset + count > size)
	{
		if (!nothrow) throw std::exception("buffer overflow");
		return false;
	}


	if (!OSHelper::IsValidAddress(buffer + offset, count))
	{
		if (!nothrow) throw std::overflow_error("invalid buffer or not enough space");
		return false;
	}

	ULong dwRead;
	if (!Microsoft::winapi_ReadFile(_fileHandle, buffer + offset, count, &dwRead, NULL))
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
		return false;
	}

	_pos += (UInt64)dwRead;
	return dwRead;
}

bool FileStream::WriteCore(char* buffer, ULong size, ULong offset, ULong count, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	if (buffer == nullptr)
	{
		if (!nothrow) throw std::exception("empty buffer");
		return false;
	}

	if (offset + count > size)
	{
		if (!nothrow) throw std::exception("buffer overflow");
		return false;
	}

	if (!OSHelper::IsValidAddress(buffer + offset, count, false))
	{
		if (!nothrow) throw std::overflow_error("invalid buffer or not enough space");
		return false;
	}

	API_UINT32 nWritten;
	if (!Microsoft::winapi_WriteFile(_fileHandle, buffer + offset, count, &nWritten, NULL))
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
		return false;
	}

	if (nWritten != count)
	{
		if (!nothrow) throw std::exception("disk is full");
		return false;
	}

	_pos += (UInt64)nWritten;
	return true;
}

void FileStream::FlushWrite(bool nothrow)
{
	WriteCore(_buffer, _bufferSize, 0, _writePos, nothrow);
	_writePos = 0;
}

void FileStream::FlushRead()
{
	if (_readPos - _readLen != 0)
	{
		SeekCore((long)(_readPos - _readLen), SeekOrigin::Current);
	}
	_readPos = 0;
	_readLen = 0;
}

UInt64 FileStream::SeekCore(Int64 offset, SeekOrigin origin, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	auto ret = Microsoft::winapi_SetFilePointer(_fileHandle, offset, (API_UINT32)origin);
	if (ret == -1)
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
	}
	_pos = (UInt64)ret;
	return _pos;
}

UInt64 FileStream::Seek(Int64 offset, SeekOrigin origin, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return 0UL;
	}

	if (!_canSeek)
	{
		if(!nothrow) throw std::exception("can not seek");
	}

	if (_writePos > 0)
	{
		FlushWrite(nothrow);
	}
	else if (origin == SeekOrigin::Current)
	{
		offset = offset - (Int64)(_readLen - _readPos);
	}

	Int64 num = _pos + (Int64)(_readPos - _readLen);
	UInt64 num1 = SeekCore(offset, origin);
	if (_appendStart != -1LL && (Int64)num1 < _appendStart)
	{
		SeekCore(num, SeekOrigin::Begin, nothrow);
		if (!nothrow) throw std::exception("file seek append overwrite");
		return 0UL;
	}

	if (_readLen > 0)
	{
		if (num == num1)
		{
			if (_readPos > 0)
			{
				memcpy(_buffer, _buffer + _readPos, _readLen - _readPos);
				_readLen -= _readPos;
				_readPos = 0;
			}
			if (_readLen > 0)
			{
				SeekCore((Int64)_readLen, SeekOrigin::Current, nothrow);
			}
		}
		else if (num - (Int64)_readPos >= num1 || num1 >= num + (Int64)_readLen - (Int64)_readPos)
		{
			_readPos = 0;
			_readLen = 0;
		}
		else
		{
			Int64 num2 = (Int64)(num1 - num);
			memcpy(_buffer, _buffer + _readPos + num2, _readLen - (_readPos + num2));
			_readLen = _readLen - (_readPos + num2);
			_readPos = 0;
			if (_readLen > 0)
			{
				SeekCore((Int64)_readLen, SeekOrigin::Current, nothrow);
			}
		}
	}
	return num1;
}

UInt64 FileStream::SeekToEnd()
{
	return Seek(0, SeekOrigin::End);
}

void FileStream::SeekToBegin()
{
	Seek(0, SeekOrigin::Begin);
}

bool FileStream::SetLengthCore(UInt64 newLength, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	UInt64 num = _pos;
	if (_pos != newLength)
	{
		Seek(newLength, SeekOrigin::Begin, nothrow);
	}
	

	if (!Microsoft::winapi_SetEndOfFile(_fileHandle))
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
		return false;
	}

	if (num != newLength)
	{
		if (num < newLength)
		{
			SeekCore(num, SeekOrigin::Begin, nothrow);
			return true;
		}
		SeekCore(0LL, SeekOrigin::End, nothrow);
	}

	return true;
}

bool FileStream::SetLength(UInt64 newLength, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return 0UL;
	}

	if (!_canSeek)
	{
		throw std::exception("can not seek");
	}

	if (!_canWrite)
	{
		throw std::exception("can not write");
	}

	if (_writePos > 0)
	{
		FlushWrite(nothrow);
	}
	else if (_readPos < _readLen)
	{
		FlushRead();
	}
	_readPos = 0;
	_readLen = 0;

	if (_appendStart != -1LL && (Int64)newLength < _appendStart)
	{
		if (!nothrow) throw std::exception("file seek append overwrite");
		return false;
	}

	return SetLengthCore(newLength, nothrow);
}

UInt64 FileStream::GetLength(bool nothrow) const
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}
	
	return (UInt64)Microsoft::winapi_GetFileSize(_fileHandle);
}

UInt64 FileStream::GetPosition(bool nothrow) const
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return 0UL;
	}

	if (!_canSeek)
	{
		if (!nothrow) throw std::exception("can not seek");
		return 0UL;
	}

	return _pos + (long)(_readPos - _readLen + _writePos);
}

bool FileStream::SetPosition(UInt64 newPos, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return 0UL;
	}

	if (_writePos > 0)
	{
		FlushWrite(nothrow);
	}
	_readPos = 0;
	_readLen = 0;
	return Seek(newPos, SeekOrigin::Begin, nothrow) !=0ull;
}

void FileStream::FlushCore()
{
	if (_writePos > 0)
	{
		FlushWrite();
		return;
	}
	if (_readPos < _readLen && _canSeek)
	{
		FlushRead();
	}
}

bool FileStream::Flush(bool nothrow)
{
	return Flush(false, nothrow);

}

bool FileStream::Flush(bool flushToDisk, bool nothrow)
{
	if (IsClosed())
	{
		if (!nothrow) throw std::exception("file is closed");
		return false;
	}
	FlushCore();
	if (flushToDisk && _canWrite)
	{
		if (!Microsoft::winapi_FlushFileBuffers(_fileHandle))
		{
			if (!nothrow) throw OSHelper::GetLastErrorException();
			return false;
		}
	}
	return true;
}

FileStream* FileStream::Duplicate(bool nothrow) const
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	FileStream* pFile = new FileStream();
	FileHandle hFile;
	FileHandle hProcess = Microsoft::winapi_GetCurrentProcess();
	if (!Microsoft::winapi_DuplicateHandle(hProcess, _fileHandle, hProcess, &hFile, 0, false, API_DUPLICATE_SAME_ACCESS))
	{
		delete pFile;
		if (!nothrow) throw OSHelper::GetLastErrorException();
		return nullptr;
	}
	pFile->_fileHandle = hFile;
	pFile->_path = _path;
	pFile->_autoClose = _autoClose;
	return pFile;
}

void FileStream::Abort()
{
	if (IsOpen())
	{
		// close but ignore errors
		Microsoft::winapi_CloseHandle(_fileHandle);
		_fileHandle = INVALID_API_PTR;
		_autoClose = false;
		_path.clear();
	}
}

bool FileStream::LockRange(const UInt64& lPos, const UInt64& lCount, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	if (!Microsoft::winapi_LockFile(_fileHandle, lPos, lCount))
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
		return false;
	}
	return true;
}

bool FileStream::UnlockRange(const UInt64& lPos, const UInt64& lCount, bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return false;
	}

	if (!Microsoft::winapi_UnlockFile(_fileHandle, lPos, lCount))
	{
		if (!nothrow) throw OSHelper::GetLastErrorException();
		return false;
	}
	return true;
}

FileType FileStream::winapi_GetFileType(bool nothrow)
{
	if (!IsOpen())
	{
		if (!nothrow) throw std::exception("file is not opened");
		return FileType::Unknown;
	}

	return _type;
}