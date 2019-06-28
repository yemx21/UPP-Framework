#include "Dispatcher.h"
#include <iostream>
#include <sstream>
using namespace uppcl;

StdWriter::StdWriter()
{

}

StdWriter::~StdWriter()
{
}

void StdWriter::SendError(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{
	std::stringstream ss;
	ss << " 101 " << project << '>' << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
	std::cout << ss.str();
}

void StdWriter::SendError(const std::string& file, int line, int column, const std::string& msg)
{
	std::stringstream ss;
	ss << " 102 " << file << '(' << line << ',' << column << "): error::" << msg << std::endl;
	std::cout << ss.str();
}

void StdWriter::SendTip(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{
	std::stringstream ss;
	ss << " 103 " << project << '>' << file << '(' << line << ',' << column << "): tip::" << msg << std::endl;
	std::cout << ss.str();
}

void StdWriter::SendTip(const std::string& file, int line, int column, const std::string& msg)
{
	std::stringstream ss;
	ss << " 104 " << file << '(' << line << ',' << column << "): tip::" << msg << std::endl;
	std::cout << ss.str();
}

void StdWriter::SendWarning(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{
	if (!slience_)
	{
		std::stringstream ss;
		ss << " 105 " << project << '>' << file << '(' << line << ',' << column << "): warning::" << msg << std::endl;
		std::cout << ss.str();
	}
}

void StdWriter::SendWarning(const std::string& file, int line, int column, const std::string& msg)
{
	if (!slience_)
	{
		std::stringstream ss;
		ss << " 106 " << file << '(' << line << ',' << column << "): warning::" << msg << std::endl;
		std::cout << ss.str();
	}
}

void StdWriter::SendOutput(const std::string& msg, const std::string& project)
{
	std::stringstream ss;
	ss << " 107 " << project << '>' << msg << std::endl;
	std::cout << ss.str();
}

void StdWriter::SendOutput(const std::string& msg)
{
	std::stringstream ss;
	ss << " 108 " << msg << std::endl;
	std::cout << ss.str();
}
