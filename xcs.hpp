#ifndef XCS_HPP
#define XCS_HPP

#include <ostream>
#include <optional>
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>
#include <functional>
#include <memory>

namespace xcs
{
	struct entity;
	struct universe;

	template<typename T>
	constexpr bool is_component() { return false; }

	struct unique_id
	{
		unsigned long const value;

		constexpr unique_id() noexcept : value(0) { }
		explicit constexpr unique_id(unsigned long value) noexcept : value(value) { }
		constexpr unique_id(unique_id const & other) noexcept = default;
		constexpr unique_id(unique_id && other) noexcept = default;
		~unique_id() noexcept = default;

		constexpr unique_id & operator= (unique_id const & other) noexcept
		{
			const_cast<unsigned long&>(value) = other.value;
			return *this;
		}

		bool operator==(unique_id const & other) const {
			return (this->value == other.value);
		}

		bool operator!=(unique_id const & other) const {
			return (*this == other);
		}

		bool operator<(unique_id const & other) const {
			return this->value < other.value;
		}

		struct hash
		{
			std::size_t operator()(unique_id const & id) const noexcept
			{
				return id.value;
			}
		};
	};

	inline std::ostream & operator<<(std::ostream & stream, unique_id id)
	{
		stream << "<" << id.value << ">";
		return stream;
	}

	struct entity
	{
		universe & universe;
		unique_id const id;

		explicit entity(struct universe & universe, unique_id id) : universe(universe), id(id) { }
	};

	struct component_store_base
	{
		virtual ~component_store_base() noexcept = default;

		virtual bool remove(unique_id id) = 0;
	};

	template<typename T>
	struct component_store : component_store_base
	{
		static_assert(is_component<T>, "T must be a registered component type!");
		using Storage = std::vector<std::pair<unique_id, T>>;

		universe & universe;

		Storage contents;
		std::unordered_map<unique_id, typename Storage::iterator, unique_id::hash> access;

		component_store(struct universe & universe) : universe(universe), contents(), access(100, unique_id::hash{})
		{
		}
		~component_store() noexcept override = default;
	public:
		T * get(unique_id ent)
		{
			auto pos = access.find(ent);
			if(pos != access.end())
				return &(*pos->second).second;
			else
				return nullptr;
		}

		T & add(unique_id ent)
		{
			if(auto * ptr = get(ent); ptr != nullptr)
				return *ptr;
			auto it = contents.emplace(contents.end(), ent, T { });
			access[ent] = it;
			return it->second;
		}

		bool remove(unique_id ent) override
		{
			auto pos = access.find(ent);
			if(pos == access.end())
				return false;
			contents.erase(pos->second);
			access.erase(ent);
			return true;
		}

	public:

		struct entity_component
		{
			entity const entity;
			T & component;

			entity_component(struct entity ent, T & c) : entity(ent), component(c) { }
		};

		struct iterator
		{
			component_store * const store;
			typename Storage::iterator iter;

			iterator(component_store * store, typename Storage::iterator iterator) :
				store(store),
				iter(iterator)
			{

			}

			entity_component operator* () {
				return entity_component { entity(store->universe, iter->first), iter->second };
			}

			entity_component operator -> () {
				return entity_component { entity(store->universe, iter->first), iter->second };
			}

			iterator & operator++() {
				this->iter++;
				return *this;
			}

			iterator & operator--() {
				this->iter--;
				return *this;
			}

			iterator operator++(int) {
				iterator clone = *this;
				this->iter++;
				return clone;
			}

			iterator operator--(int) {
				iterator clone = *this;
				this->iter--;
				return clone;
			}

			constexpr bool operator== (iterator const & other) const {
				return (this->store == other.store)
					and (this->iter == other.iter);
			}

			constexpr bool operator != (iterator const & other) const {
				return !(*this == other);
			}
		};

		iterator begin() { return iterator(this, contents.begin()); }
		iterator end() { return iterator(this, contents.end()); }

	};


	struct meta_component_base
	{
		char const * const name;
		std::size_t const id;
		meta_component_base * const next;

		explicit meta_component_base(char const * name) noexcept;

		virtual ~meta_component_base() noexcept;

		virtual std::unique_ptr<component_store_base> create_store(universe & universe) const = 0;
	};

	template<typename T>
	struct meta_component : meta_component_base
	{
		explicit meta_component(const char * name) : meta_component_base(name) { }
		~meta_component() noexcept override = default;

		std::unique_ptr<component_store_base> create_store(universe & universe) const override
		{
			return std::make_unique<component_store<T>>(universe);
		}
	};

	template<typename T>
	meta_component<T> const & get_meta_component() = delete;

	#define XCS_REGISTER_COMPONENT(_Type) \
		template<> inline bool ::xcs::is_component<_Type>() { return true; } \
		inline ::xcs::meta_component<_Type> __##_Type##component { #_Type }; \
		template<> inline ::xcs::meta_component<_Type> const & ::xcs::get_meta_component<_Type>() { \
			return __##_Type##component; \
		}

	struct universe
	{
	private:
		std::set<unique_id> entities;
		unsigned long nextunique_id;
		mutable std::unordered_map<std::size_t, std::unique_ptr<component_store_base>> component_stores;

	public:
		universe() : entities(), nextunique_id(1), component_stores()
		{

		}
		universe(universe const & ) = delete;
		universe(universe &&) = delete;
		~universe() = default;

	public:
		entity create_entity()
		{
			std::pair<std::set<unique_id>::iterator, bool> pair;
			while(not (pair = entities.emplace(nextunique_id)).second)
				nextunique_id++;
			return entity { *this, *pair.first };
		}

		std::optional<entity> create_entity_with_id(unique_id id)
		{
			auto result = entities.emplace(id);
			if(result.second)
				return entity { *this, *result.first };
			else
				return std::nullopt;
		}

		std::optional<entity> find_entity(unique_id id)
		{
			auto it = entities.find(id);
			if(it != entities.end())
				return entity { *this, *it };
			else
				return std::nullopt;
		}

		bool destroy_entity(entity ent)
		{
			if(&ent.universe != this) // wrong universe, can't remove
				return false;
			auto const existed = (entities.erase(ent.id) > 0);
			if(existed)
			{
				for(auto & pair : this->component_stores)
					pair.second->remove(ent.id);
			}
			return existed;
		}

	public:
		template<typename T>
		component_store<T> & get_components()
		{
			auto const & meta = get_meta_component<T>();
			auto it = component_stores.find(meta.id);
			if(it != component_stores.end())
				return static_cast<component_store<T>&>(*it->second.get());
			component_stores.emplace(meta.id, meta.create_store(*this));
			return static_cast<component_store<T>&>(*component_stores.at(meta.id).get());
		}

		template<typename T>
		component_store<T> const & get_components() const
		{
			auto const & meta = get_meta_component<T>();
			auto it = component_stores.find(meta.id);
			if(it != component_stores.end())
				return static_cast<component_store<T>&>(*it->second.get());
			component_stores.emplace(meta.id, meta.create_store(*this));
			return static_cast<component_store<T>&>(*component_stores.at(meta.id).get());
		}
	};

	template<typename T>
	inline T & add_component(entity ent)
	{
		return ent.universe.get_components<T>().add(ent.id);
	}

	template<typename T>
	inline T * get_component(entity ent)
	{
		return ent.universe.get_components<T>().get(ent.id);
	}

	template<typename T>
	inline bool remove_component(entity ent)
	{
		return ent.universe.get_components<T>().remove(ent.id);
	}

	template<typename T>
	inline component_store<T> & get_components(universe & universe)
	{
		return universe.get_components<T>();
	}
}
#endif // ECS_HPP
