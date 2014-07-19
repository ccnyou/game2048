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
		static RCObject*	Alloc();	//����ʵ��
		virtual bool		Init();		//�������
		virtual id			Retain();
		virtual void		Release();
		virtual void		AutoRelease();	//���뱣֤��ǰ�����г�ʼ����һ�� AutoReleasePool
		virtual void		Dealloc();		//��������������������������Լ����������󣬱�����û����Dealloc

		int					RetainCount() const;
	};

// 	class Test : public RCObject
// 	{
// 		DECLARE_CC_RCOBJECT(Test, RCObject);
// 	};

}

