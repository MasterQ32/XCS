#ifndef COMPONENTA_HPP
#define COMPONENTA_HPP

#include "xcs.hpp"

struct ComponentA
{
	xcs::unique_id other;
};

XCS_REGISTER_COMPONENT(ComponentA)

#endif // COMPONENTA_HPP
