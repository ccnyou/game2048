#pragma once

#include <stdio.h>
#include "Type.h"

// #define DECLARE_CC_RCOBJECT(type, super) \
// public:\
// 	static cc::rcptr<type> Alloc()\
// { return cc::rcptr<type>(new type()); }


namespace cc
{
	class String;
	class Object
	{
	public:
		Object(void);
		virtual ~Object(void);

		virtual String	ToString() const;
		virtual bool	IsEqual(const Object& theObj) const;
	};

	class RCObject : public Object
	{
	private:
		int m_retainCount;

	protected:
		RCObject();
		virtual ~RCObject();

	public:
		static RCObject*	Alloc();	//必须实现
		virtual bool		Init();		//必须调用
		virtual id			Retain();
		virtual void		Release();
		virtual void		AutoRelease();	//必须保证当前环境有初始化了一个 AutoReleasePool
		virtual void		Dealloc();		//如果子类重载了这个方法，完成自己的清理工作后，必须调用基类的Dealloc

		int					RetainCount() const;
	};

// 	class Test : public RCObject
// 	{
// 		DECLARE_CC_RCOBJECT(Test, RCObject);
// 	};

}

