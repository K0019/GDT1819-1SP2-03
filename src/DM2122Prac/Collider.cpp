#include "Collider.h"

Collider::Collider(Type type)
	: colliderType(type)
{

}

Collider::~Collider()
{

}

Collider::Type Collider::getType() const
{
	return colliderType;
}