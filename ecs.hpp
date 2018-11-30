#ifndef ECS_HPP
#define ECS_HPP

#include <unordered_map>
#include <utility>
#include <memory>

namespace ecs
{
	struct Component
	{
		virtual ~Component() noexcept = default;
	};

	struct MetaComponent
	{
		MetaComponent * const next;
	    char const * const name;
	    unsigned long id;

	    explicit MetaComponent(char const * name) noexcept;
	    MetaComponent(MetaComponent const &) = delete;
	    MetaComponent(MetaComponent &&) = delete;
		virtual ~MetaComponent() noexcept = default;

	    virtual std::unique_ptr<Component> construct() const = 0;

	public:
		static MetaComponent const * first();
	};

	template<typename T>
	struct GenericMetaComponent : MetaComponent
	{
	    explicit GenericMetaComponent(char const * name) : MetaComponent(name)
	    {

	    }

		~GenericMetaComponent() noexcept override = default;

	    std::unique_ptr<Component> construct() const override
	    {
	        return std::make_unique<T>();
	    }
	};

	template<typename T>
	MetaComponent const & GetMetaComponent() = delete;

	template<typename T>
	constexpr bool IsRegisteredComponent() { return false; }

	template<typename T>
	unsigned long GetMetaComponentID()
	{
		return GetMetaComponent<T>().id;
	}

	#define ECS_REGISTER_COMPONENT(_name, _type) \
		inline ::ecs::GenericMetaComponent<_type> __##_type##_component { _name }; \
		template<> inline ::ecs::MetaComponent const & ::ecs::GetMetaComponent<_type>() { return __##_type##_component; } \
		template<> constexpr bool ::ecs::IsRegisteredComponent<_type>() { return true; }

	struct Entity
	{
	private:
		std::unordered_map<unsigned long, std::unique_ptr<Component>> components;
	public:
		template<typename T>
		T * AddComponent()
		{
			static_assert(IsRegisteredComponent<T>(), "Component type must be registered!");
			auto const & mc = GetMetaComponent<T>();
			auto it = components.emplace(mc.id, nullptr);
			if(not it.second)
				throw std::out_of_range("component already exists!");
			it.first->second = mc.construct();
			return static_cast<T*>(it.first->second.get());
		}

		template<typename T>
		T * GetComponent()
		{
			static_assert(IsRegisteredComponent<T>(), "Component type must be registered!");
			return static_cast<T*>(components[GetMetaComponentID<T>()].get());
		}

		template<typename T>
		T const * GetComponent() const
		{
			static_assert(IsRegisteredComponent<T>(), "Component type must be registered!");
			return static_cast<T*>(components[GetMetaComponentID<T>()].get());
		}

		template<typename T>
		bool RemoveComponent()
		{
			return (components.erase(GetMetaComponentID<T>()) > 0);
		}
	};
}

#endif // ECS_HPP
