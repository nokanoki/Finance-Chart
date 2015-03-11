#include "Object.h"
using fchart::Object;

Object::Object()
{
	this->refcount = 1;
}
Object::~Object()
{
}

void Object::Release()
{
	this->refcount--;
	if (this->refcount <= 0)
		delete this;
}
void Object::AddRef()
{
	this->refcount++;
}