#include "ArrayType.h"
#include <string>
#include <unordered_map>
#include <mutex>
#include "InertiaString.h"

using namespace System;

namespace System
{
	class ArrayType_impl
	{
	private:
		static std::mutex locker;
		static std::unordered_map<size_t, ArrayType*> caches;
		static std::hash<std::wstring> hash_fn;
		static bool CreateName(std::wstring& name, _type ktype, _type khashType, _type kequalType, _type kcompareType, _type kallocatorType, _type itype, _type iallocatorType, _type icontainerType, _type icompareType, ContainerTypes ctype, UInt8 dimension, size_t fixedSize1, size_t fixedSize2, size_t fixedSize3, size_t fixedSize4, size_t fixedSize5)
		{
			switch (ctype)
			{
			case ContainerTypes::Array:
			default:
			{
				switch (dimension)
				{
				case 1U:
					if (fixedSize1 == 0) return false;
					if (!itype) return false;
					name.append(itype->Name);
					name.push_back('[');
					name.append(std::to_wstring(fixedSize1));
					name.push_back(']');
					break;
				case 2U:
					if (fixedSize1 == 0 || fixedSize2==0) return false;
					if (!itype) return false;
					name.append(itype->Name);
					name.push_back('[');
					name.append(std::to_wstring(fixedSize1));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize2));
					name.push_back(']');
					break;
				case 3U:
					if (fixedSize1 == 0 || fixedSize2 == 0 || fixedSize3 == 0) return false;
					if (!itype) return false;
					name.append(itype->Name);
					name.push_back('[');
					name.append(std::to_wstring(fixedSize1));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize2));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize3));
					name.push_back(']');
					break;
				case 4U:
					if (fixedSize1 == 0 || fixedSize2 == 0 || fixedSize3 == 0 || fixedSize4 == 0) return false;
					if (!itype) return false;
					name.append(itype->Name);
					name.push_back('[');
					name.append(std::to_wstring(fixedSize1));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize2));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize3));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize4));
					name.push_back(']');
					break;
				case 5U:
					if (fixedSize1 == 0 || fixedSize2 == 0 || fixedSize3 == 0 || fixedSize4 == 0 || fixedSize5 == 0) return false;
					if (!itype) return false;
					name.append(itype->Name);
					name.push_back('[');
					name.append(std::to_wstring(fixedSize1));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize2));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize3));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize4));
					name.push_back(']');
					name.push_back('[');
					name.append(std::to_wstring(fixedSize5));
					name.push_back(']');
					break;
				default:
					return false;
				}
			}
				break;
			case ContainerTypes::STL_array:
			{
												if (fixedSize1 <= 0) return false;
												if (!itype) return false;
												name.append(L"std::array<"); 
												name.append(itype->Name);
												name.push_back(',');
												name.append(std::to_wstring(fixedSize1));
												name.push_back('>');
			}
				break;
			case ContainerTypes::STL_vector:
			{
												if (!itype) return nullptr;
												name.append(L"std::vector<");
												name.append(itype->Name);
												if (iallocatorType)
												{
													name.push_back(',');
													name.append(iallocatorType->Name);
												}
												else
												{
													name.push_back(',');
													name.append(L"std::allocator<");
													name.append(itype->Name);
													name.push_back('>');
												}
												name.push_back('>');
			}
				break;
			case ContainerTypes::STL_deque:
			{
												if (!itype) return nullptr;
												name.append(L"std::deque<");
												name.append(itype->Name);
												if (iallocatorType)
												{
													name.push_back(',');
													name.append(iallocatorType->Name);
												}
												else
												{
													name.push_back(',');
													name.append(L"std::allocator<");
													name.append(itype->Name);
													name.push_back('>');
												}
												name.push_back('>');
			}
				break;
			case ContainerTypes::STL_forward_list:
			{
														if (!itype) return nullptr;
														name.append(L"std::forward_list<");
														name.append(itype->Name);
														if (iallocatorType)
														{
															name.push_back(',');
															name.append(iallocatorType->Name);
														}
														else
														{
															name.push_back(',');
															name.append(L"std::allocator<");
															name.append(itype->Name);
															name.push_back('>');
														}
														name.push_back('>');
			}
				break;
			case ContainerTypes::STL_list:
			{
												if (!itype) return nullptr;
												name.append(L"std::list<");
												name.append(itype->Name);
												name.push_back(',');
												if (iallocatorType)
												{
													name.append(iallocatorType->Name);
												}
												else
												{					
													name.append(L"std::allocator<");
													name.append(itype->Name);
													name.push_back('>');
												}
												name.push_back('>');
			}
				break;
			case ContainerTypes::STL_set:
			{
											if (!ktype) return nullptr;
											name.append(L"std::set<");
											name.append(ktype->Name);
											name.push_back(',');
											if (kcompareType)
											{						
												name.append(kcompareType->Name);
											}
											else
											{
												name.append(L"std::less<");
												name.append(ktype->Name);
												name.push_back('>');
											}
											name.push_back(',');
											if (kallocatorType)
											{
												name.append(kallocatorType->Name);
											}
											else
											{
												name.append(L"std::allocator<");
												name.append(ktype->Name);
												name.push_back('>');
											}
											name.push_back('>');
			}
				break;
			case ContainerTypes::STL_multiset:
			{
													if (!ktype) return nullptr;
													name.append(L"std::multiset<");
													name.append(ktype->Name);
													name.push_back(',');
													if (kcompareType)
													{
														name.append(kcompareType->Name);
													}
													else
													{
														name.append(L"std::less<");
														name.append(ktype->Name);
														name.push_back('>');
													}
													name.push_back(',');
													if (kallocatorType)
													{
														name.append(kallocatorType->Name);
													}
													else
													{
														name.append(L"std::allocator<");
														name.append(ktype->Name);
														name.push_back('>');
													}
													name.push_back('>');
			}
				break;
			case ContainerTypes::STL_map:
			{
											if (!ktype) return nullptr;
											if (!itype) return nullptr;
											name.append(L"std::map<");
											name.append(ktype->Name);
											name.push_back(',');
											name.append(itype->Name);
											name.push_back(',');
											if (kcompareType)
											{
												name.append(kcompareType->Name);
											}
											else
											{
												name.append(L"std::less<");
												name.append(ktype->Name);
												name.push_back('>');
											}
											name.push_back(',');
											if (kallocatorType)
											{
												name.append(kallocatorType->Name);
											}
											else
											{
												name.append(L"std::allocator<std::pair<");
												name.append(ktype->Name);
												name.push_back(',');
												name.append(itype->Name);
												name.push_back('>'); 
												name.push_back('>');
											}
											name.push_back('>');
			}
				break;
			case ContainerTypes::STL_multimap:
			{
													if (!ktype) return nullptr;
													if (!itype) return nullptr;
													name.append(L"std::multimap<");
													name.append(ktype->Name);
													name.push_back(',');
													name.append(itype->Name);
													name.push_back(',');
													if (kcompareType)
													{
														name.append(kcompareType->Name);
													}
													else
													{
														name.append(L"std::less<");
														name.append(ktype->Name);
														name.push_back('>');
													}
													name.push_back(',');
													if (kallocatorType)
													{
														name.append(kallocatorType->Name);
													}
													else
													{
														name.append(L"std::allocator<std::pair<");
														name.append(ktype->Name);
														name.push_back(',');
														name.append(itype->Name);
														name.push_back('>');
														name.push_back('>');
													}
													name.push_back('>');
			}
				break;
			case ContainerTypes::STL_unordered_set:
			{
														if (!ktype) return nullptr;
														name.append(L"std::unordered_set<");
														name.append(ktype->Name);
														name.push_back(',');
														if (khashType)
														{
															name.append(khashType->Name);
														}
														else
														{
															name.append(L"std::hash<");
															name.append(ktype->Name);
															name.push_back('>');
														} 
														name.push_back(',');
														if (kequalType)
														{
															name.append(kequalType->Name);
														}
														else
														{
															name.append(L"std::equal_to<");
															name.append(ktype->Name);
															name.push_back('>');
														}
														name.push_back(',');
														if (kallocatorType)
														{
															name.append(kallocatorType->Name);
														}
														else
														{
															name.append(L"std::allocator<");
															name.append(ktype->Name);
															name.push_back('>');
														}
														name.push_back('>');
			}
				break;
			case ContainerTypes::STL_unordered_multiset:
			{
															if (!ktype) return nullptr;
															name.append(L"std::unordered_multiset<");
															name.append(ktype->Name);
															name.push_back(',');
															if (khashType)
															{
																name.append(khashType->Name);
															}
															else
															{
																name.append(L"std::hash<");
																name.append(ktype->Name);
																name.push_back('>');
															}
															name.push_back(',');
															if (kequalType)
															{
																name.append(kequalType->Name);
															}
															else
															{
																name.append(L"std::equal_to<");
																name.append(ktype->Name);
																name.push_back('>');
															}
															name.push_back(',');
															if (kallocatorType)
															{
																name.append(kallocatorType->Name);
															}
															else
															{
																name.append(L"std::allocator<");
																name.append(ktype->Name);
																name.push_back('>');
															}
															name.push_back('>');
			}
				break;
			case ContainerTypes::STL_unordered_map:
			{
														if (!ktype) return nullptr;
														if (!itype) return nullptr;
														name.append(L"std::map<");
														name.append(ktype->Name);
														name.push_back(',');
														name.append(itype->Name);
														name.push_back(',');
														if (khashType)
														{
															name.append(khashType->Name);
														}
														else
														{
															name.append(L"std::hash<");
															name.append(ktype->Name);
															name.push_back('>');
														} 
														name.push_back(',');
														if (kequalType)
														{
															name.append(kequalType->Name);
														}
														else
														{
															name.append(L"std::equal_to<");
															name.append(ktype->Name);
															name.push_back('>');
														}
														name.push_back(',');
														if (kallocatorType)
														{
															name.append(kallocatorType->Name);
														}
														else
														{
															name.append(L"std::allocator<std::pair<");
															name.append(ktype->Name);
															name.push_back(',');
															name.append(itype->Name);
															name.push_back('>');
															name.push_back('>');
														}
														name.push_back('>');
			}
				break;
			case ContainerTypes::STL_unordered_multimap:
			{
															if (!ktype) return nullptr;
															if (!itype) return nullptr;
															name.append(L"std::unordered_multimap<");
															name.append(ktype->Name);
															name.push_back(',');
															name.append(itype->Name);
															name.push_back(',');
															if (khashType)
															{
																name.append(khashType->Name);
															}
															else
															{
																name.append(L"std::hash<");
																name.append(ktype->Name);
																name.push_back('>');
															}
															name.push_back(',');
															if (kequalType)
															{
																name.append(kequalType->Name);
															}
															else
															{
																name.append(L"std::equal_to<");
																name.append(ktype->Name);
																name.push_back('>');
															}
															name.push_back(',');
															if (kallocatorType)
															{
																name.append(kallocatorType->Name);
															}
															else
															{
																name.append(L"std::allocator<std::pair<");
																name.append(ktype->Name);
																name.push_back(',');
																name.append(itype->Name);
																name.push_back('>');
																name.push_back('>');
															}
															name.push_back('>');
			}
				break;
			case ContainerTypes::STL_stack:
			{
												if (!itype) return nullptr;
												name.append(L"std::stack<");
												name.append(itype->Name);
												name.push_back(',');
												if (icontainerType)
												{
													name.append(icontainerType->Name);
												}
												else
												{
													name.append(L"std::deque<");
													name.append(itype->Name);
													name.push_back('>');
												}
												name.push_back('>');
			}
				break;
			case ContainerTypes::STL_queue:
			{
												if (!itype) return nullptr;
												name.append(L"std::queue<");
												name.append(itype->Name);
												name.push_back(',');
												if (icontainerType)
												{
													name.append(icontainerType->Name);
												}
												else
												{
													name.append(L"std::deque<");
													name.append(itype->Name);
													name.push_back('>');
												}
												name.push_back('>');
			}
				break;
			case ContainerTypes::STL_priority_queue:
			{
														if (!itype) return nullptr;
														name.append(L"std::priority_queue<");
														name.append(itype->Name);
														name.push_back(',');
														if (icontainerType)
														{
															name.append(icontainerType->Name);
														}
														else
														{
															name.append(L"std::vector<");
															name.append(itype->Name);
															name.push_back('>');
														}
														name.push_back(',');
														if (icompareType)
														{
															name.append(icompareType->Name);
														}
														else
														{
															name.append(L"std::less<typename Container::value_type>");
														}
														name.push_back('>');
			}
				break;
			case ContainerTypes::ObservableCollection:
			{
				if (!itype) return nullptr;
				name.append(L"System::ObservableCollection<");
				name.append(itype->Name);
				name.push_back('>');
			}
				break;
			}
			return true;
		}
	public:
		_type keyType;
		_type keyHashType;
		_type keyEqualType;
		_type keyCompareType;
		_type keyAllocatorType;
		_type itemType;
		_type itemAllocatorType;
		_type itemContainerType;
		_type itemCompareType;

		InertiaString name;
		ContainerTypes cttype; 
		UInt8 dm;
		size_t fsize1;
		size_t fsize2;
		size_t fsize3;
		size_t fsize4;
		size_t fsize5;
		bool isfixed;
		static ArrayType* Create(_type ktype, _type khashType, _type kequalType, _type kcompareType, _type kallocatorType, _type itype, _type iallocatorType, _type icontainerType, _type icompareType, ContainerTypes ctype, UInt8 dimension, size_t fixedSize1, size_t fixedSize2, size_t fixedSize3, size_t fixedSize4, size_t fixedSize5)
		{
			std::wstring str;
			if (CreateName(str, ktype, khashType, kequalType, kcompareType, kallocatorType, itype, iallocatorType, icontainerType, icompareType, ctype, dimension, fixedSize1, fixedSize2, fixedSize3, fixedSize4, fixedSize5))
			{
				size_t hash = hash_fn(str);
				std::lock_guard<std::mutex> lock(locker);
				auto iter = caches.find(hash);
				if (iter != caches.end())
					return iter->second;
				else
				{
					ArrayType* type = new ArrayType(ktype, khashType, kequalType, kcompareType, kallocatorType, itype, iallocatorType, icontainerType, icompareType, ctype, dimension, fixedSize1, fixedSize2, fixedSize3, fixedSize4, fixedSize5);
					type->_hashCode = hash;
					type->impl->name = InertiaString(str.c_str());
					caches.insert(std::make_pair(hash, type));
					return type;
				}
			}
			return false;
		}

		ArrayType_impl(_type ktype, _type khashType, _type kequalType, _type kcompareType, _type kallocatorType, _type itype, _type iallocatorType, _type icontainerType, _type icompareType, ContainerTypes ctype, UInt8 dimension, size_t fixedSize1, size_t fixedSize2, size_t fixedSize3, size_t fixedSize4, size_t fixedSize5) : keyType(ktype), keyHashType(khashType), keyEqualType(kequalType), keyCompareType(kcompareType), keyAllocatorType(kallocatorType), itemType(itype), itemAllocatorType(iallocatorType), itemContainerType(icontainerType), itemCompareType(icompareType), cttype(ctype), dm(dimension), fsize1(fixedSize1), fsize2(fixedSize2), fsize3(fixedSize3), fsize4(fixedSize4), fsize5(fixedSize5)
		{
			isfixed = ctype == ContainerTypes::Array || ctype == ContainerTypes::STL_array;
		};
	
		size_t dimension(UInt8 d) const
		{
			switch (d)
			{
			case 0U:
			default:
				return fsize1;
			case 1U:
				return fsize2;
			case 2U:
				return fsize3;
			case 3U:
				return fsize4;
			case 4U:
				return fsize5;
			}
			return 0;
		}

		static void Remove(ArrayType* at)
		{
			std::lock_guard<std::mutex> lock(locker);
			ArrayType* att = at;
			auto iter = caches.find(*at->_hashCode);
			if (iter != caches.end())
			{
				caches.erase(iter);
			}
		}

	};

	std::mutex ArrayType_impl::locker;
	std::unordered_map<size_t, ArrayType*> ArrayType_impl::caches;
	std::hash<std::wstring> ArrayType_impl::hash_fn;
}

const ArrayType* ArrayType::Create(_type keyType, _type keyHashType, _type keyEqualType, _type keyCompareType, _type keyAllocatorType, _type itemType, _type itemAllocatorType, _type itemContainerType, _type itemCompareType, ContainerTypes arrayType, UInt8 dimension, size_t fixedSize1, size_t fixedSize2, size_t fixedSize3, size_t fixedSize4, size_t fixedSize5)
{
	return ArrayType_impl::Create(keyType, keyHashType, keyEqualType, keyCompareType, keyAllocatorType, itemType, itemAllocatorType, itemContainerType, itemCompareType, arrayType, dimension, fixedSize1, fixedSize2, fixedSize3, fixedSize4, fixedSize5);
}

ArrayType::ArrayType(_type keyType, _type keyHashType, _type keyEqualType, _type keyCompareType, _type keyAllocatorType, _type itemType, _type itemAllocatorType, _type itemContainerType, _type itemCompareType, ContainerTypes arrayType, UInt8 dimension, size_t fixedSize1, size_t fixedSize2, size_t fixedSize3, size_t fixedSize4, size_t fixedSize5) :Type(TypeTag::Array)
{
	impl = new ArrayType_impl(keyType, keyHashType, keyEqualType, keyCompareType, keyAllocatorType, itemType, itemAllocatorType, itemContainerType, itemCompareType, arrayType, dimension, fixedSize1, fixedSize2, fixedSize3, fixedSize4, fixedSize5);
}

ArrayType::~ArrayType()
{
	if (impl)
	{
		ArrayType_impl::Remove(this);
		delete impl;
		impl = nullptr;
	}
}

ContainerTypes ArrayType::GetContainerTypes() const
{
	return  impl != nullptr ? impl->cttype : ContainerTypes::Unknown;
}

const wchar_t* ArrayType::GetName() const
{
	return impl != nullptr ? impl->name.get().c_str() : nullptr;
}


bool ArrayType::GetIsFixed() const
{
	return  impl != nullptr ? impl->isfixed : false;
}

size_t ArrayType::GetFixedSize(UInt8 dimension) const
{
	return  impl != nullptr ? impl->dimension(dimension) : 0;
}

UInt8 ArrayType::GetDimension()const
{
	return impl != nullptr ? impl->dm : 0;
}

_type ArrayType::GetKeyType() const
{
	return  impl != nullptr ? impl->keyType : nullptr;
}

_type ArrayType::GetKeyHashType() const
{
	return  impl != nullptr ? impl->keyHashType : nullptr;
}

_type ArrayType::GetKeyEqualType() const
{
	return  impl != nullptr ? impl->keyEqualType : nullptr;
}

_type ArrayType::GetKeyCompareType() const
{
	return  impl != nullptr ? impl->keyCompareType : nullptr;
}

_type ArrayType::GetKeyAllocatorType() const
{
	return  impl != nullptr ? impl->keyAllocatorType : nullptr;
}

_type ArrayType::GetItemType() const
{
	return  impl != nullptr ? impl->itemType : nullptr;
}

_type ArrayType::GetItemAllocatorType() const
{
	return  impl != nullptr ? impl->itemAllocatorType : nullptr;
}

_type ArrayType::GetItemContainerType() const
{
	return  impl != nullptr ? impl->itemContainerType : nullptr;
}

_type ArrayType::GetItemCompareType() const
{
	return  impl != nullptr ? impl->itemCompareType : nullptr;
}