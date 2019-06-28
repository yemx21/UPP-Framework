#include "Async.h"

using namespace System::Threading;


void IAsyncState::Wait_impl()
{

}
void IAsyncState::WaitFor_impl(const TimeSpan& timeout)
{

}

AsyncOperationStatus IAsyncState::GetStatus_impl() const
{
	return AsyncOperationStatus::Pending;
}


void AsyncOperation::Wait()
{

}
void AsyncOperation::WaitFor(const TimeSpan& timeout)
{

}

AsyncOperationStatus AsyncOperation::GetStatus() const
{
	return AsyncOperationStatus::Pending;
}

Any AsyncOperation::GetResult()
{
	return Any();
}
