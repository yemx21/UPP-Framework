#include "Dispatcher.h"
using namespace uppcl;

Dispatcher::Dispatcher() :slience_(false)
{

}

void Dispatcher::SetSlience(bool slience)
{
	slience_ = slience;
}

bool Dispatcher::GetSlience() const
{
	return slience_;
}

void Dispatcher::SendError(const std::string& file, int line, int column, const std::string& msg, const std::string& solution)
{

}

void Dispatcher::SendError(const std::string& file, int line, int column, const std::string& msg)
{

}

void Dispatcher::SendTip(const std::string& file, int line, int column, const std::string& msg)
{

}

void Dispatcher::SendWarning(const std::string& file, int line, int column, const std::string& msg)
{

}

void Dispatcher::SendOutput(const std::string& msg)
{

}

void Dispatcher::SendTip(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{

}

void Dispatcher::SendWarning(const std::string& file, int line, int column, const std::string& msg, const std::string& project)
{

}

void Dispatcher::SendOutput(const std::string& msg, const std::string& project)
{

}
