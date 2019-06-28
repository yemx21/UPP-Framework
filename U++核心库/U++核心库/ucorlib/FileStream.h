#pragma once
#include "Config.h"
#include <string>



namespace System
{
	namespace IO
	{
		typedef void* FileHandle;
		typedef void* FileSecurityHandle;

		ENUM_FLAGSEX(FileAdvancedAccess, ULong)
			/// <summary>
			/// 高级文件访问权限
			/// </summary>
		enum class FileAdvancedAccess : ULong
		{
			//
			// Standart Section
			//

			AccessSystemSecurity = 0x1000000,   // AccessSystemAcl access type
			MaximumAllowed = 0x2000000,     // MaximumAllowed access type

			Delete = 0x10000,
			ReadControl = 0x20000,
			WriteDAC = 0x40000,
			WriteOwner = 0x80000,
			Synchronize = 0x100000,

			StandardRightsRequired = 0xF0000,
			StandardRightsRead = ReadControl,
			StandardRightsWrite = ReadControl,
			StandardRightsExecute = ReadControl,
			StandardRightsAll = 0x1F0000,
			SpecificRightsAll = 0xFFFF,

			File_Read_Data = 0x0001,        // file & pipe
			File_List_Directory = 0x0001,       // directory
			File_Write_Data = 0x0002,       // file & pipe
			File_Add_File = 0x0002,         // directory
			File_Append_Data = 0x0004,      // file
			File_Add_SubDirectory = 0x0004,     // directory
			File_Create_Pipe_Instance = 0x0004, // named pipe
			File_Read_EA = 0x0008,          // file & directory
			File_Write_EA = 0x0010,         // file & directory
			File_Execute = 0x0020,          // file
			File_Traverse = 0x0020,         // directory
			File_Delete_Child = 0x0040,     // directory
			File_Read_Attributes = 0x0080,      // all
			File_Write_Attributes = 0x0100,     // all

			//
			// Generic Section
			//

			GenericRead = 0x80000000,
			GenericWrite = 0x40000000,
			GenericExecute = 0x20000000,
			GenericAll = 0x10000000,

			Specific_Rights_All = 0x00FFFF,
			File_All_Access = StandardRightsRequired | Synchronize | 0x1FF,

			File_Generic__Read = StandardRightsRead | File_Read_Data | File_Read_Attributes | File_Read_EA | Synchronize,

			File_Generic__Write = StandardRightsWrite | File_Write_Data | File_Write_Attributes | File_Write_EA | File_Append_Data | Synchronize,

			File_Generic__ReadWrite = ReadControl | File_Read_Data | File_Write_Data | File_Read_Attributes | File_Write_Attributes | File_Read_EA | File_Write_EA | File_Append_Data | Synchronize,

			File_Generic_Execute = StandardRightsExecute | File_Read_Attributes | File_Execute | Synchronize
		};


		ENUM_OPERATORS(FileAccess)
			/// <summary>
			/// 文件访问权限
			/// </summary>
		enum class FileAccess
		{
			Read,
			Write,
			All,
		};

		ENUM_FLAGSEX(FileShare, ULong)
			/// <summary>
			/// 文件共享权限
			/// </summary>
		enum class FileShare :ULong
		{
			/// <summary>
			/// 
			/// </summary>
			None = 0x00000000,
			/// <summary>
			/// Enables subsequent open operations on an object to request read access. 
			/// Otherwise, other processes cannot open the object if they request read access. 
			/// If this flag is not specified, but the object has been opened for read access, the function fails.
			/// </summary>
			Read = 0x00000001,
			/// <summary>
			/// Enables subsequent open operations on an object to request write access. 
			/// Otherwise, other processes cannot open the object if they request write access. 
			/// If this flag is not specified, but the object has been opened for write access, the function fails.
			/// </summary>
			Write = 0x00000002,

			/// <summary>
			/// Enables subsequent open and write operations on an object.
			/// </summary>
			ReadWrite = 0x00000003,

			/// <summary>
			/// Enables subsequent open operations on an object to request delete access. 
			/// Otherwise, other processes cannot open the object if they request delete access.
			/// If this flag is not specified, but the object has been opened for delete access, the function fails.
			/// </summary>
			Delete = 0x00000004
		};

		ENUM_OPERATORS(FileCreationDisposition)
			/// <summary>
			/// 文件创建配置
			/// </summary>
		enum class FileCreationDisposition
		{
			/// <summary>
			/// Creates a new file. The function fails if a specified file exists.
			/// </summary>
			New = 1,
			/// <summary>
			/// Creates a new file, always. 
			/// If a file exists, the function overwrites the file, clears the existing attributes, combines the specified file attributes, 
			/// and flags with FILE_ATTRIBUTE_ARCHIVE, but does not set the security descriptor that the SECURITY_ATTRIBUTES structure specifies.
			/// </summary>
			CreateAlways = 2,
			/// <summary>
			/// Opens a file. The function fails if the file does not exist. 
			/// </summary>
			OpenExisting = 3,
			/// <summary>
			/// Opens a file, always. 
			/// If a file does not exist, the function creates a file as if dwCreationDisposition is CREATE_NEW.
			/// </summary>
			OpenAlways = 4,
			/// <summary>
			/// Opens a file and truncates it so that its size is 0 (zero) bytes. The function fails if the file does not exist.
			/// The calling process must open the file with the GENERIC_WRITE access right. 
			/// </summary>
			TruncateExisting = 5
		};

		ENUM_OPERATORS(FileMode)
			/// <summary>
			/// 文件模式
			/// </summary>
		enum class FileMode
		{
			CreateNew = 1,
			/// <summary>
			/// Creates a new file, always. 
			/// If a file exists, the function overwrites the file, clears the existing attributes, combines the specified file attributes, 
			/// and flags with FILE_ATTRIBUTE_ARCHIVE, but does not set the security descriptor that the SECURITY_ATTRIBUTES structure specifies.
			/// </summary>
			Create = 2,
			/// <summary>
			/// Opens a file. The function fails if the file does not exist. 
			/// </summary>
			Open = 3,
			/// <summary>
			/// Opens a file, always. 
			/// If a file does not exist, the function creates a file as if dwCreationDisposition is CREATE_NEW.
			/// </summary>
			OpenOrCreate = 4,
			/// <summary>
			/// Opens a file and truncates it so that its size is 0 (zero) bytes. The function fails if the file does not exist.
			/// The calling process must open the file with the GENERIC_WRITE access right. 
			/// </summary>
			Truncate = 5,
			/// <summary>
			/// Opens a file, always. 
			/// If a file does not exist, the function creates a file as if dwCreationDisposition is CREATE_NEW.
			/// Then the file will be seeked to the end.
			/// </summary>
			Append = 6
		};

		ENUM_FLAGSEX(FileAttributes, ULong)
			/// <summary>
			/// 文件属性
			/// </summary>
		enum class FileAttributes :ULong
		{
			None = 0x0000000,
			Readonly = 0x00000001,
			Hidden = 0x00000002,
			System = 0x00000004,
			Directory = 0x00000010,
			Archive = 0x00000020,
			Device = 0x00000040,
			Normal = 0x00000080,
			Temporary = 0x00000100,
			SparseFile = 0x00000200,
			ReparsePoint = 0x00000400,
			Compressed = 0x00000800,
			Offline = 0x00001000,
			NotContentIndexed = 0x00002000,
			Encrypted = 0x00004000,
			Write_Through = 0x80000000,
			Overlapped = 0x40000000,
			NoBuffering = 0x20000000,
			RandomAccess = 0x10000000,
			SequentialScan = 0x08000000,
			DeleteOnClose = 0x04000000,
			BackupSemantics = 0x02000000,
			PosixSemantics = 0x01000000,
			OpenReparsePoint = 0x00200000,
			OpenNoRecall = 0x00100000,
			FirstPipeInstance = 0x00080000
		};

		/// <summary>
		/// 查找起始位置
		/// </summary>
		enum class SeekOrigin
		{ 
			Begin,
			Current,
			End,
		};

		/// <summary>
		/// 文件类型
		/// </summary>
		enum class FileType
		{
			/// <summary>			
			/// The specified file is a character file, typically an LPT device or a console.
			/// </summary>
			Char =0x0002, 
			/// <summary>			
			/// The specified file is a disk file.
			/// </summary>
			Disk=0x0001,
			/// <summary>			
			/// The specified file is a socket, a named pipe, or an anonymous pipe.
			/// </summary>
			Pipe=0x0003,
			/// <summary>			
			/// Unused.
			/// </summary>
			Remote=0x8000,
			Unknown=0x0000,
		};

		/// <summary>
		/// 文件流类
		/// </summary>
		class RUNTIME_API FileStream
		{
		private:
			FileHandle  _fileHandle;
			bool    _autoClose;
			std::wstring _path;
			char* _buffer; 
			int _readPos;
			Int64 _readLen;
			Int64 _writePos;
			int _bufferSize;
			bool _canRead;
			bool _canWrite;
			bool _canSeek;
			bool _isPipe;
			UInt64 _pos;
			Int64 _appendStart;
			FileType _type;

		protected:
			/// <summary>
			/// 读取字节
			/// </summary>
			/// <param name="buffer">字节</param>
			/// <param name="size">字节大小</param>
			/// <param name="offset">字节偏移</param>
			/// <param name="count">字节数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			ULong ReadCore(char* buffer, ULong size, ULong offset, ULong count, bool nothrow = true);

			/// <summary>
			/// 写入字节
			/// </summary>
			/// <param name="buffer">字节</param>
			/// <param name="size">字节大小</param>
			/// <param name="offset">字节偏移</param>
			/// <param name="count">字节数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool WriteCore(char* buffer, ULong size, ULong offset, ULong count, bool nothrow = true);

			/// <summary>
			/// 刷新写入
			/// </summary>
			/// <param name="nothrow">The nothrow.</param>
			void FlushWrite(bool nothrow = true);

			/// <summary>
			/// 刷新读取
			/// </summary>
			void FlushRead();

			/// <summary>
			/// 快进到新位置
			/// </summary>
			/// <param name="offset">位置偏移</param>
			/// <param name="origin">位置.</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>新位置</returns>
			UInt64 SeekCore(Int64 offset, SeekOrigin origin, bool nothrow = true);

			/// <summary>
			/// 设置文件流长度
			/// </summary>
			/// <param name="newLength">新长度</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool SetLengthCore(UInt64 newLength, bool nothrow = true);

			/// <summary>
			/// 刷新
			/// </summary>
			void FlushCore();
		public:
			/// <summary>
			/// 创建 <see cref="FileStream"/> 实例.
			/// </summary>
			FileStream();

			/// <summary>
			/// 创建 <see cref="FileStream"/> 实例.
			/// </summary>
			/// <param name="hFile">WindowsAPI 文件句柄</param>
			/// <param name="autoClose">自动关闭</param>
			FileStream(FileHandle hFile, bool autoClose = true);

			~FileStream();

			operator FileHandle();

			/// <summary>
			/// 打开指定文件
			/// </summary>
			/// <param name="filePath">文件路径</param>
			/// <param name="desiredAccess">期望文件访问权限</param>
			/// <param name="shareMode">文件共享模式</param>
			/// <param name="creationDistribution">文件创建模式</param>
			/// <param name="bufferSize">缓冲大小</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <param name="lpSecurityAttributes">WindowsAPI 安全属性句柄</param>
			/// <param name="attributes">文件属性</param>
			/// <param name="templateFileHandle">WindowsAPI 临时文件句柄</param>
			/// <returns>bool.</returns>
			bool Open(const wchar_t* filePath, FileAdvancedAccess desiredAccess, FileShare shareMode, FileCreationDisposition creationDistribution, int bufferSize=4096, bool nothrow = true,
				FileSecurityHandle lpSecurityAttributes = nullptr,
				FileAttributes attributes = FileAttributes::None,
				FileHandle templateFileHandle = nullptr);

			/// <summary>
			/// 打开指定文件
			/// </summary>
			/// <param name="filePath">文件路径</param>
			/// <param name="desiredAccess">期望文件访问权限</param>
			/// <param name="shareMode">文件共享模式</param>
			/// <param name="creationDistribution">文件创建模式</param>
			/// <param name="bufferSize">缓冲大小</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <param name="lpSecurityAttributes">WindowsAPI 安全属性句柄</param>
			/// <param name="attributes">文件属性</param>
			/// <param name="templateFileHandle">WindowsAPI 临时文件句柄</param>
			/// <returns>bool.</returns>
			bool Open(const wchar_t* filePath, FileAccess desiredAccess, FileShare shareMode, FileMode mode, int bufferSize = 4096, bool nothrow = true,
				FileSecurityHandle lpSecurityAttributes = nullptr,
				FileAttributes attributes = FileAttributes::None,
				FileHandle templateFileHandle = nullptr);

			/// <summary>
			/// 判断文件流是否可读
			/// </summary>
			bool CanRead() const;

			/// <summary>
			/// 判断文件流是否可写
			/// </summary>
			bool CanWrite() const;

			/// <summary>
			/// 判断文件流是否可快进
			/// </summary>
			bool CanSeek() const;

			/// <summary>
			/// 判断文件流是否处于打开状态
			/// </summary>
			bool IsOpen() const;

			/// <summary>
			/// 判断文件流是否处于关闭状态
			/// </summary>
			bool IsClosed() const; 

			/// <summary>
			/// 附加WindowsAPI 文件句柄
			/// </summary>
			/// <param name="hFile">WindowsAPI 文件句柄</param>
			/// <param name="autoClose">是否自动关闭</param>
			void Attach(FileHandle hFile, bool autoClose = true);

			/// <summary>
			/// 分离WindowsAPI 文件句柄
			/// </summary>
			void Detach();

			/// <summary>
			/// 关闭文件流
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			void Close(bool nothrow = true);

			/// <summary>
			/// 写入字节
			/// </summary>
			/// <param name="buffer">字节</param>
			/// <param name="size">字节大小</param>
			/// <param name="offset">字节偏移</param>
			/// <param name="count">字节数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool Write(char* buffer, ULong size, ULong offset, ULong count, bool nothrow = true);

			/// <summary>
			/// 写入一个字节
			/// </summary>
			/// <param name="byte">字节</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool WriteByte(int byte, bool nothrow = true);

			/// <summary>
			/// 读取字节
			/// </summary>
			/// <param name="buffer">字节</param>
			/// <param name="size">字节大小</param>
			/// <param name="offset">字节偏移</param>
			/// <param name="count">字节数</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			ULong Read(char* buffer, ULong size, ULong offset, ULong count, bool nothrow = true);

			/// <summary>
			/// 读取下一个字节
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>单字节</returns>
			int ReadByte(bool nothrow = true);

			/// <summary>
			/// 快进到新位置
			/// </summary>
			/// <param name="offset">位置偏移</param>
			/// <param name="origin">位置.</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>新位置</returns>
			UInt64 Seek(Int64 offset, SeekOrigin origin, bool nothrow = true);

			/// <summary>
			/// 快进至结尾
			/// </summary>
			/// <returns>新位置</returns>
			UInt64 SeekToEnd();

			/// <summary>
			/// 退回至文件头
			/// </summary>
			void SeekToBegin();

			/// <summary>
			/// 设置文件流长度
			/// </summary>
			/// <param name="newLength">新长度</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool SetLength(UInt64 newLength, bool nothrow = true);

			/// <summary>
			/// 获取文件流长度
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>文件流长度</returns>
			UInt64 GetLength(bool nothrow = true) const;

			/// <summary>
			/// 获取文件流位置
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			UInt64 GetPosition(bool nothrow = true) const; 

			/// <summary>
			/// 设置文件流位置
			/// </summary>
			/// <param name="newPos">新位置</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool SetPosition(UInt64 newPos, bool nothrow=true);

			/// <summary>
			/// 刷新
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>bool.</returns>
			bool Flush(bool nothrow = true);

			/// <summary>
			/// 刷新
			/// </summary>
			/// <param name="flushToDisk">刷新到硬盘</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			virtual bool Flush(bool flushToDisk, bool nothrow = true);

			/// <summary>
			/// 复制文件流
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>文件流</returns>
			FileStream* Duplicate(bool nothrow = true) const;

			/// <summary>
			/// 中止文件流
			/// </summary>
			void Abort();

			/// <summary>
			/// 区域上锁
			/// </summary>
			/// <param name="lPos">上锁位置</param>
			/// <param name="lCount">上锁长度</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool LockRange(const UInt64& lPos, const UInt64& lCount, bool nothrow = true);

			/// <summary>
			/// 区域解锁
			/// </summary>
			/// <param name="lPos">解锁位置</param>
			/// <param name="lCount">解锁长度</param>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns></returns>
			bool UnlockRange(const UInt64& lPos, const UInt64& lCount, bool nothrow = true);

			/// <summary>
			/// 获取文件类型
			/// </summary>
			/// <param name="nothrow">无错误捕捉</param>
			/// <returns>文件类型</returns>
			FileType winapi_GetFileType(bool nothrow = true);
		};
	}
}