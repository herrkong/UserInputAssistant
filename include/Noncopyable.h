 
#pragma once

namespace hk
{

class Noncopyable
{
protected:
	Noncopyable();
	~Noncopyable();

	Noncopyable(const Noncopyable &) = delete;
	Noncopyable & operator=(const Noncopyable &) = delete;

};


}//end of namespace hk
