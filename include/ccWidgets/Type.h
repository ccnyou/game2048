#ifndef type_h__
#define type_h__

#include <iostream>
#include <string>
#include <wchar.h>
#include <stddef.h>
#include <cctype>
#include <cassert>
#include "auto_tchar.h"

namespace cc
{
	typedef	std::basic_string<TCHAR>	    StdTString;
	typedef std::basic_ostream<TCHAR>	    StdTOStream;
	typedef std::basic_istream<TCHAR>		StdTIStream;
	typedef std::basic_stringstream<TCHAR>	StdTStringStream;

    typedef std::string     StdString;
    typedef std::wstring    StdWString;

	typedef	TCHAR*			PTSTR;
	typedef	char*			PSTR;
	typedef	wchar_t*		PWSTR;

	typedef	const TCHAR*	PCTSTR;
	typedef	const char*		PCSTR;
	typedef	const wchar_t*	PCWSTR;

    typedef unsigned long   ulong;
    typedef unsigned int    uint;
    typedef unsigned char   uchar;

	typedef long long   	    int64;
	typedef unsigned long long	uint64;


	struct byte
	{
		uchar ch;

		byte(){}
		byte(const int& val)
		{
			assert(val <= 0xFF);
			ch = (uchar)val;
		}

		operator uchar() const
		{
			return ch;
		}

		PCSTR ToPCSTR() const
		{
			return (PCSTR)this;
		}

		PCWSTR ToPCWSTR() const
		{
			return (PCWSTR)this;
		}

		PCTSTR ToPCTSTR() const
		{
			return (PCTSTR)this;
		}

		PSTR ToPSTR() const
		{
			return (PSTR)this;
		}

		PWSTR ToPWSTR() const
		{
			return (PWSTR)this;
		}

		PTSTR ToPTSTR() const
		{
			return (PTSTR)this;
		}
	};

    class Object;
    class id
    {
    private:
        Object*	m_obj;

    public:

		id() : m_obj(NULL){}
        id(Object* obj) : m_obj(obj){}

        template<class Type>
        operator Type*()
        {
            return dynamic_cast<Type*>(m_obj);
        }

        template<class Type>
        operator const Type*() const
        {
            return dynamic_cast<const Type*>(m_obj);
        }

		id& operator =(Object* obj)
		{
			m_obj = obj;
			return *this;
		}
    };

	const id nil = id(NULL);

	class RCObject;
	template<typename Type>
	class rcptr
	{
	private:
		typedef rcptr<Type>	MyType;
		typedef MyType*		TypePtr;

	public:
		rcptr(Type* obj = NULL)
		{
			m_ptr = obj;
		}

		rcptr(rcptr& rhs)
		{
			if (m_ptr != rhs.m_ptr)
			{
				Release();
				m_ptr = rhs.Retain();
			}
		}

		Type* operator->() const
		{
			return m_ptr;
		}

		operator const Type*() const
		{
			return m_ptr;
		}

		operator Type*()
		{
			return m_ptr;
		}

	protected:
		void Release()
		{
			if (m_ptr != NULL)
			{
				m_ptr->Release();
			}
		}

		Type* Retain()
		{
			Type* obj = NULL;
			if (m_ptr != NULL)
			{
				obj = m_ptr->Retain();
			}
			return obj;
		}

	private:
		Type*	m_ptr;
	};
}
#endif // type_h__
