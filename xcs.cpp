#include "xcs.hpp"

namespace
{
	std::size_t next_component_type_id = 1;
	xcs::meta_component_base * first_meta_component = nullptr;
}

xcs::meta_component_base::meta_component_base(char const * name) noexcept :
	name(name),
	id(++next_component_type_id),
	next(first_meta_component)
{
	first_meta_component = this;
}

xcs::meta_component_base::~meta_component_base() noexcept
{

}
