#define LZH_EXPORTS
#include "pointer.h"

lzh::RefCountPointer::RefCountPointer() :refcount(nullptr)
{
}

lzh::RefCountPointer::RefCountPointer(const RefCountPointer & p) : refcount(nullptr)
{
	*this = p;
}

lzh::RefCountPointer & lzh::RefCountPointer::operator=(const RefCountPointer & p)
{
	if (this == &p)return *this;
	assign(p.refcount);
	return *this;
}

lzh::RefCountPointer::~RefCountPointer()
{
	release();
}

void lzh::RefCountPointer::create()
{
	release();
	refcount = new int(1);
}

void lzh::RefCountPointer::addref()
{
	if (refcount)*refcount += 1;
}

void lzh::RefCountPointer::release()
{
	if (refcount) {
		if (*refcount == 1) {
			delete refcount;
		}
		else {
			*refcount -= 1;
		}
		refcount = nullptr;
	}
}
void lzh::RefCountPointer::assign(int *refcount)
{
	release();
	if (refcount) {
		this->refcount = refcount;
		addref();
	}
}
bool lzh::RefCountPointer::last() const
{
        return refcount != nullptr ? *refcount == 1 : false;
}
