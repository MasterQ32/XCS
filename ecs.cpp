#include "ecs.hpp"

using namespace ecs;

static MetaComponent * first = nullptr;

static inline unsigned long get_id(MetaComponent * ref)
{
    if(ref != nullptr)
        return ref->id + 1;
    else
        return 1;
}

MetaComponent::MetaComponent(char const * name) noexcept :
    next(::first),
    name(name),
    id(get_id(::first))
{
	::first = this;
}

MetaComponent const * MetaComponent::first()
{
	return ::first;
}
