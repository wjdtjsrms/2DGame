#ifndef INCLUDED_SEQUENCE_GAME_CHILD_H
#define INCLUDED_SEQUENCE_GAME_CHILD_H

#include "Sequence\Base.h"

namespace Sequence
{
	namespace Game {
		class Parent;
		class Child : public Base
		{
		public:
			virtual ~Child(); // 단순 가상 함수
			Base* update(Base*); 
			virtual Base* update(Parent*) = 0;
		};
	}
}
#endif