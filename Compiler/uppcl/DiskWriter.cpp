#include "Dispatcher.h"
using namespace uppcl;


DiskWriter::DiskWriter(): Dispatcher()
{
	
}

DiskWriter::~DiskWriter()
{
	if (writer.is_open())
	{
		writer.flush();
		writer.close();
	}
}

bool DiskWriter::Open(const std::string& path)
{
	writer.open(path, std::ios::out);
	return writer.is_open();
}

void DiskWriter::SendError(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{
	writer << project << '>' << file << '(' << line << ',' << column << "): error::" <<  msg << std::endl;
}

void DiskWriter::SendError(const std::string& file, int line, int column, const std::string& msg)
{
	writer << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
}

void DiskWriter::SendTip(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{
	writer << project << '>' << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
}

void DiskWriter::SendTip(const std::string& file, int line, int column, const std::string& msg)
{
	if(!slience_) writer << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
}

void DiskWriter::SendWarning(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{
	if (!slience_) writer << project << '>' << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
}

void DiskWriter::SendWarning(const std::string& file, int line, int column, const std::string& msg)
{
	if (!slience_) writer << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
}

void DiskWriter::SendOutput(const std::string& msg, const std::string& project)
{
	writer << project << '>' << msg << std::endl;
}

void DiskWriter::SendOutput(const std::string& msg)
{
	writer << msg << std::endl;
}
