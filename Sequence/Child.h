#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H


namespace Sequence
{
	class Parent;
	class Child
	{
	public:
		virtual ~Child() {}; // 단순 가상 함수
		virtual void update(Parent* ) = 0; // 순수 가상 함수
	};
}
#endif