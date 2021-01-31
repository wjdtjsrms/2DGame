#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H

#include "Sequence\Base.h"

namespace Sequence
{
	class Parent;
	class Child : public Base
	{
	public:
		virtual ~Child(); // 단순 가상 함수
		Base* update(Base*); 
		virtual Base* update(Parent*) = 0;
	};
}
#endif