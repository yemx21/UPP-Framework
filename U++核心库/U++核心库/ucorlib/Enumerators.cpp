#include <iterator>
#include <set>
#include "MethodInfo.h"
#include "PropertyInfo.h"
#include "OperatorInfo.h"
#include "ConstructorInfo.h"
#include "Enumerators_impl.h"
#include "Enumerators.h"

using namespace System;

MethodEnumerator::MethodEnumerator() :impl(nullptr)
{

}

MethodEnumerator::~MethodEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const MethodInfo* MethodEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int MethodEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void MethodEnumerator::Next()
{
	if (impl) impl->Next();
}

void MethodEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool MethodEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


MethodReverseEnumerator::MethodReverseEnumerator() :impl(nullptr)
{

}

MethodReverseEnumerator::~MethodReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const MethodInfo* MethodReverseEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int MethodReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void MethodReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void MethodReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool MethodReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

MethodQueryEnumerator::MethodQueryEnumerator() :impl(nullptr)
{

}

MethodQueryEnumerator::~MethodQueryEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const MethodInfo* MethodQueryEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int MethodQueryEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void MethodQueryEnumerator::Next()
{
	if (impl) impl->Next();
}

void MethodQueryEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool MethodQueryEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


PropertyEnumerator::PropertyEnumerator() :impl(nullptr)
{

}

PropertyEnumerator::~PropertyEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const PropertyInfo* PropertyEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int PropertyEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void PropertyEnumerator::Next()
{
	if (impl) impl->Next();
}

void PropertyEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool PropertyEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


PropertyReverseEnumerator::PropertyReverseEnumerator() :impl(nullptr)
{

}

PropertyReverseEnumerator::~PropertyReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const PropertyInfo* PropertyReverseEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int PropertyReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void PropertyReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void PropertyReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool PropertyReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

PropertyQueryEnumerator::PropertyQueryEnumerator() :impl(nullptr)
{

}

PropertyQueryEnumerator::~PropertyQueryEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const PropertyInfo* PropertyQueryEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int PropertyQueryEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void PropertyQueryEnumerator::Next()
{
	if (impl) impl->Next();
}

void PropertyQueryEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool PropertyQueryEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


ConstructorEnumerator::ConstructorEnumerator() :impl(nullptr)
{

}

ConstructorEnumerator::~ConstructorEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const ConstructorInfo* ConstructorEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int ConstructorEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void ConstructorEnumerator::Next()
{
	if (impl) impl->Next();
}

void ConstructorEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool ConstructorEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

ConstructorReverseEnumerator::ConstructorReverseEnumerator() :impl(nullptr)
{

}

ConstructorReverseEnumerator::~ConstructorReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const ConstructorInfo* ConstructorReverseEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int ConstructorReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void ConstructorReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void ConstructorReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool ConstructorReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

ConstructorQueryEnumerator::ConstructorQueryEnumerator() :impl(nullptr)
{

}

ConstructorQueryEnumerator::~ConstructorQueryEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const ConstructorInfo* ConstructorQueryEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int ConstructorQueryEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void ConstructorQueryEnumerator::Next()
{
	if (impl) impl->Next();
}

void ConstructorQueryEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool ConstructorQueryEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


OperatorEnumerator::OperatorEnumerator() :impl(nullptr)
{

}

OperatorEnumerator::~OperatorEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const OperatorInfo* OperatorEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int OperatorEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void OperatorEnumerator::Next()
{
	if (impl) impl->Next();
}

void OperatorEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool OperatorEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


OperatorReverseEnumerator::OperatorReverseEnumerator() :impl(nullptr)
{

}

OperatorReverseEnumerator::~OperatorReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const OperatorInfo* OperatorReverseEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int OperatorReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void OperatorReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void OperatorReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool OperatorReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

OperatorQueryEnumerator::OperatorQueryEnumerator() :impl(nullptr)
{

}

OperatorQueryEnumerator::~OperatorQueryEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const OperatorInfo* OperatorQueryEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int OperatorQueryEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void OperatorQueryEnumerator::Next()
{
	if (impl) impl->Next();
}

void OperatorQueryEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool OperatorQueryEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

AttributeEnumerator::AttributeEnumerator() :impl(nullptr)
{

}

AttributeEnumerator::~AttributeEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const Attribute* AttributeEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int AttributeEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void AttributeEnumerator::Next()
{
	if (impl) impl->Next();
}

void AttributeEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool AttributeEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


AttributeReverseEnumerator::AttributeReverseEnumerator() :impl(nullptr)
{

}

AttributeReverseEnumerator::~AttributeReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const Attribute* AttributeReverseEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int AttributeReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void AttributeReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void AttributeReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool AttributeReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

AttributeQueryEnumerator::AttributeQueryEnumerator() :impl(nullptr)
{

}

AttributeQueryEnumerator::~AttributeQueryEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const Attribute* AttributeQueryEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int AttributeQueryEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void AttributeQueryEnumerator::Next()
{
	if (impl) impl->Next();
}

void AttributeQueryEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool AttributeQueryEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

FieldEnumerator::FieldEnumerator() :impl(nullptr)
{

}

FieldEnumerator::~FieldEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const FieldInfo* FieldEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int FieldEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void FieldEnumerator::Next()
{
	if (impl) impl->Next();
}

void FieldEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool FieldEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}


FieldReverseEnumerator::FieldReverseEnumerator() :impl(nullptr)
{

}

FieldReverseEnumerator::~FieldReverseEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const FieldInfo* FieldReverseEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int FieldReverseEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void FieldReverseEnumerator::Next()
{
	if (impl) impl->Next();
}

void FieldReverseEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool FieldReverseEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}

FieldQueryEnumerator::FieldQueryEnumerator() :impl(nullptr)
{

}

FieldQueryEnumerator::~FieldQueryEnumerator()
{
	if (impl){ delete impl; impl = nullptr; }
}

const FieldInfo* FieldQueryEnumerator::GetCurrent() const
{
	if (impl) return impl->Current();
	return nullptr;
}

unsigned int FieldQueryEnumerator::GetCount() const
{
	return impl != nullptr ? impl->Count() : 0;
}

void FieldQueryEnumerator::Next()
{
	if (impl) impl->Next();
}

void FieldQueryEnumerator::Previous()
{
	if (impl) impl->Previous();
}

bool FieldQueryEnumerator::GetIsEnd() const
{
	if (impl) return impl->IsEnd();
	return true;
}
