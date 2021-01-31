#ifndef INCLUDED_SEQUENCE_BASE_H
#define INCLUDED_SEQUENCE_BASE_H

class Base
{
public:
	virtual ~Base() {};
	virtual Base* update(Base*) = 0;
};
#endif