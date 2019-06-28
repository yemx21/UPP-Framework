#include "UamlReader.h"
using namespace System::Uaml;


UamlReader::UamlReader(const UamlServiceManagerPtr& serviceManager) : _service(serviceManager)
{

}

UamlReader::~UamlReader()
{
	
}


Result<bool, UamlError> UamlReader::Open(const wchar_t* path)
{
	Result<bool, UamlError> ret{ false, UamlError::Failed };

	return ret;
}

void UamlReader::Close()
{

}