#include "../include/xcs"

namespace
{
	std::size_t next_component_type_id = 1;
	xcs::meta_component_base const * first_meta_component = nullptr;
}

xcs::meta_component_base::meta_component_base(std::string const & name) noexcept :
	name(name),
	id(++next_component_type_id),
	next(first_meta_component)
{
	first_meta_component = this;
}

xcs::meta_component_base::~meta_component_base() noexcept
{

}

xcs::meta_component_base const * xcs::meta_component_base::find(std::string const & name)
{
	for(auto const * it = ::first_meta_component; it != nullptr; it = it->next)
	{
		if(it->name == name)
			return it;
	}
	return nullptr;
}

xcs::meta_component_base const & xcs::meta_component_base::by_name(std::string const & name)
{
	if(auto const * mc = find(name); mc != nullptr)
		return *mc;
	throw std::invalid_argument("component " + name + "does not exist");
}
