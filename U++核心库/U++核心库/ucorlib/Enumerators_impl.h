#pragma once
#include <set>
#include <vector>
#include "Predicates.h"
#include <mutex>


namespace System
{
	class MethodInfo;
	class ClassInfo;
	class EnumInfo;

	class MethodEnumerator;
	class MethodReverseEnumerator;
	class MethodQueryEnumerator;

	class MethodEnumerator_impl
	{
	protected:
		friend class MethodInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class MethodEnumerator;
		typedef std::set<MethodInfo*>* base;
		typedef std::set<MethodInfo*>::const_iterator base_iterator; 
		typedef std::set<MethodInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		std::mutex lock;
		MethodEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();		
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const MethodInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == iterOwner->end()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer++; 
		}

		void Previous()
		{
			if (citer == iterOwner->begin()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer--; 
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class MethodReverseEnumerator_impl
	{
	protected:
		friend class MethodInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class MethodReverseEnumerator;
		typedef std::set<MethodInfo*>* base;
		typedef std::set<MethodInfo*>::const_reverse_iterator base_iterator;
		typedef std::set<MethodInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer; 
		std::mutex lock;
		MethodReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const MethodInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == iterOwner->rend()) return; 
			std::lock_guard<std::mutex> locker(lock);
			citer++;
		}

		void Previous()
		{
			if (citer == iterOwner->rbegin()) return;
			std::lock_guard<std::mutex> locker(lock);
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class MethodQueryEnumerator_impl
	{
	protected:
		friend class MethodInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class MethodQueryEnumerator;
		typedef std::set<MethodInfo*>* base;
		std::vector<MethodInfo*> results;
		typedef std::vector<MethodInfo*>::iterator res_iter;
		res_iter citer;
		std::mutex lock;
		MethodQueryEnumerator_impl(base owner, MethodPredicate pre)
		{
			std::lock_guard<std::mutex> locker(lock);
			for (auto iter = owner->begin(); iter != owner->end(); ++iter)
			{
				auto item = *iter;
				if (item)
				{
					if (pre(item))
					{
						results.push_back(item);
					}
				}
			}
			citer = results.begin();
		}

		unsigned int Count() const
		{
			return results.size();
		}

		const MethodInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == results.end()) return;
			citer++;
		}

		void Previous()
		{
			if (citer == results.begin()) return;
			citer--;
		}

		bool IsEnd() const
		{
			return citer == results.end();
		}
	};


	class PropertyEnumerator;
	class PropertyReverseEnumerator;
	class PropertyQueryEnumerator;

	class PropertyEnumerator_impl
	{
	protected:
		friend class PropertyInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class PropertyEnumerator;
		typedef std::set<PropertyInfo*>* base;
		typedef std::set<PropertyInfo*>::const_iterator base_iterator;
		typedef std::set<PropertyInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		PropertyEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const PropertyInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class PropertyReverseEnumerator_impl
	{
	protected:
		friend class PropertyInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class PropertyReverseEnumerator;
		typedef std::set<PropertyInfo*>* base;
		typedef std::set<PropertyInfo*>::const_reverse_iterator base_iterator;
		typedef std::set<PropertyInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		PropertyReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const PropertyInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class PropertyQueryEnumerator_impl
	{
	protected:
		friend class PropertyInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class PropertyQueryEnumerator;
		typedef std::set<PropertyInfo*>* base;
		std::vector<PropertyInfo*> results;
		typedef std::vector<PropertyInfo*>::iterator res_iter;
		res_iter citer;
		std::mutex lock;
		PropertyQueryEnumerator_impl(base owner, PropertyPredicate pre)
		{
			std::lock_guard<std::mutex> locker(lock);
			for (auto iter = owner->begin(); iter != owner->end(); ++iter)
			{
				auto item = *iter;
				if (item)
				{
					if (pre(item))
					{
						results.push_back(item);
					}
				}
			}
			citer = results.begin();
		}

		unsigned int Count() const
		{
			return results.size();
		}

		const PropertyInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == results.end()) return;
			citer++;
		}

		void Previous()
		{
			if (citer == results.begin()) return;
			citer--;
		}

		bool IsEnd() const
		{
			return citer == results.end();
		}
	};


	class ConstructorEnumerator;
	class ConstructorReverseEnumerator;
	class ConstructorQueryEnumerator;

	class ConstructorEnumerator_impl
	{
	protected:
		friend class ConstructorInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class ConstructorEnumerator;
		typedef std::set<ConstructorInfo*>* base;
		typedef std::set<ConstructorInfo*>::const_iterator base_iterator;
		typedef std::set<ConstructorInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		ConstructorEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const ConstructorInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class ConstructorReverseEnumerator_impl
	{
	protected:
		friend class ConstructorInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class ConstructorReverseEnumerator;
		typedef std::set<ConstructorInfo*>* base;
		typedef std::set<ConstructorInfo*>::const_reverse_iterator base_iterator;
		typedef std::set<ConstructorInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		ConstructorReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const ConstructorInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class ConstructorQueryEnumerator_impl
	{
	protected:
		friend class ConstructorInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class ConstructorQueryEnumerator;
		typedef std::set<ConstructorInfo*>* base;
		std::vector<ConstructorInfo*> results;
		typedef std::vector<ConstructorInfo*>::iterator res_iter;
		res_iter citer;
		std::mutex lock;
		ConstructorQueryEnumerator_impl(base owner, ConstructorPredicate pre)
		{
			std::lock_guard<std::mutex> locker(lock);
			for (auto iter = owner->begin(); iter != owner->end(); ++iter)
			{
				auto item = *iter;
				if (item)
				{
					if (pre(item))
					{
						results.push_back(item);
					}
				}
			}
			citer = results.begin();
		}

		unsigned int Count() const
		{
			return results.size();
		}

		const ConstructorInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == results.end()) return;
			citer++;
		}

		void Previous()
		{
			if (citer == results.begin()) return;
			citer--;
		}

		bool IsEnd() const
		{
			return citer == results.end();
		}
	};


	class OperatorEnumerator;
	class OperatorReverseEnumerator;
	class OperatorQueryEnumerator;

	class OperatorEnumerator_impl
	{
	protected:
		friend class OperatorInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class OperatorEnumerator;
		typedef std::set<OperatorInfo*>* base;
		typedef std::set<OperatorInfo*>::const_iterator base_iterator;
		typedef std::set<OperatorInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		OperatorEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const OperatorInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class OperatorReverseEnumerator_impl
	{
	protected:
		friend class OperatorInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class OperatorReverseEnumerator;
		typedef std::set<OperatorInfo*>* base;
		typedef std::set<OperatorInfo*>::const_reverse_iterator base_iterator;
		typedef std::set<OperatorInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		OperatorReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const OperatorInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class OperatorQueryEnumerator_impl
	{
	protected:
		friend class OperatorInfo;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class OperatorQueryEnumerator;
		typedef std::set<OperatorInfo*>* base;
		std::vector<OperatorInfo*> results;
		typedef std::vector<OperatorInfo*>::iterator res_iter;
		res_iter citer;
		std::mutex lock;
		OperatorQueryEnumerator_impl(base owner, OperatorPredicate pre)
		{
			std::lock_guard<std::mutex> locker(lock);
			for (auto iter = owner->begin(); iter != owner->end(); ++iter)
			{
				auto item = *iter;
				if (item)
				{
					if (pre(item))
					{
						results.push_back(item);
					}
				}
			}
			citer = results.begin();
		}

		unsigned int Count() const
		{
			return results.size();
		}

		const OperatorInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == results.end()) return;
			citer++;
		}

		void Previous()
		{
			if (citer == results.begin()) return;
			citer--;
		}

		bool IsEnd() const
		{
			return citer == results.end();
		}
	};

	class AttributeEnumerator;
	class AttributeReverseEnumerator;
	class AttributeQueryEnumerator;

	class AttributeEnumerator_impl
	{
	protected:
		friend class Attribute;
		friend class EnumInfo;
		friend class ClassInfo;
		friend class AttributeEnumerator;
		typedef std::vector<Attribute*>* base;
		typedef std::vector<Attribute*>::const_iterator base_iterator;
		typedef std::vector<Attribute*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		AttributeEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const Attribute* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class AttributeReverseEnumerator_impl
	{
	protected:
		friend class Attribute;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class AttributeReverseEnumerator;
		typedef std::vector<Attribute*>* base;
		typedef std::vector<Attribute*>::const_reverse_iterator base_iterator;
		typedef std::vector<Attribute*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		AttributeReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const Attribute* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class AttributeQueryEnumerator_impl
	{
	protected:
		friend class Attribute;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class AttributeQueryEnumerator;
		typedef std::vector<Attribute*>* base;
		std::vector<Attribute*> results;
		typedef std::vector<Attribute*>::iterator res_iter;
		res_iter citer;
		std::mutex lock;
		AttributeQueryEnumerator_impl(base owner, AttributePredicate pre)
		{
			std::lock_guard<std::mutex> locker(lock);
			for (auto iter = owner->begin(); iter != owner->end(); ++iter)
			{
				auto item = *iter;
				if (item)
				{
					if (pre(item))
					{
						results.push_back(item);
					}
				}
			}
			citer = results.begin();
		}

		unsigned int Count() const
		{
			return results.size();
		}

		const Attribute* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == results.end()) return;
			citer++;
		}

		void Previous()
		{
			if (citer == results.begin()) return;
			citer--;
		}

		bool IsEnd() const
		{
			return citer == results.end();
		}
	};

	class FieldEnumerator;
	class FieldReverseEnumerator;
	class FieldQueryEnumerator;

	class FieldEnumerator_impl
	{
	protected:
		friend class Field;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class FieldEnumerator;
		typedef std::set<FieldInfo*>* base;
		typedef std::set<FieldInfo*>::const_iterator base_iterator;
		typedef std::set<FieldInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		FieldEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->begin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const FieldInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->end();
		}
	};

	class FieldReverseEnumerator_impl
	{
	protected:
		friend class Field;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class FieldReverseEnumerator;
		typedef std::set<FieldInfo*>* base;
		typedef std::set<FieldInfo*>::const_reverse_iterator base_iterator;
		typedef std::set<FieldInfo*>::size_type pos_type;
		base iterOwner;
		base_iterator citer;
		FieldReverseEnumerator_impl(base owner)
		{
			iterOwner = owner;
			citer = iterOwner->rbegin();
		}

		unsigned int Count() const
		{
			return iterOwner->size();
		}

		const FieldInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			citer++;
		}

		void Previous()
		{
			citer--;
		}

		bool IsEnd() const
		{
			return citer == iterOwner->rend();
		}
	};

	class FieldQueryEnumerator_impl
	{
	protected:
		friend class Field;
		friend class ClassInfo;
		friend class EnumInfo;
		friend class FieldQueryEnumerator;
		typedef std::set<FieldInfo*>* base;
		std::vector<FieldInfo*> results;
		typedef std::vector<FieldInfo*>::iterator res_iter;
		res_iter citer;
		std::mutex lock;
		FieldQueryEnumerator_impl(base owner, FieldPredicate pre)
		{
			std::lock_guard<std::mutex> locker(lock);
			for (auto iter = owner->begin(); iter != owner->end(); ++iter)
			{
				auto item = *iter;
				if (item)
				{
					if (pre(item))
					{
						results.push_back(item);
					}
				}
			}
			citer = results.begin();
		}

		unsigned int Count() const
		{
			return results.size();
		}

		const FieldInfo* Current() const
		{
			return *citer;
		}

		void Next()
		{
			if (citer == results.end()) return;
			citer++;
		}

		void Previous()
		{
			if (citer == results.begin()) return;
			citer--;
		}

		bool IsEnd() const
		{
			return citer == results.end();
		}
	};
}
