// by Dmitry Kolontay

export module Resources;
export import Resources.Types;

import <atomic>;
import <utility>;
import <tuple>;
import <algorithm>;
import <cassert>;
import <stdexcept>;

export template<ResourceType Type>
struct ResourceInfo {
	const float amount_;
	constexpr ResourceInfo(float amount = 0.f) :
		amount_{ amount } 
	{

	};
};

export class ResourceContainer{
	std::atomic<float> amount_;

protected:

	constexpr ResourceContainer(float range, float amount = 0.f) noexcept(false) :
		amount_{ amount }, range_{ range }
	{
		if (range < amount || range <= 0.f) {
			throw std::invalid_argument{"range must be positive and not less then amount"};
		}
	};

public:
	const float range_;

	ResourceContainer() = delete;
	virtual ~ResourceContainer() = default;

	virtual ResourceType GetType() const& = 0;

	float GetAmount() const& {
		return amount_.load();
	};

	float GetFreeSpace() const& {
		return range_ - amount_.load();
	};

	float RequestResource(float amount, bool acceptLower = false)&
	{

	};

	float ChangeAmount(float amount)& {

		float oldVal{ amount_.load() };
		float newVal{ std::clamp(oldVal + amount, 0.f, range_) };

		while (!amount_.compare_exchange_weak(oldVal, newVal))
		{
			newVal = std::clamp(oldVal + amount, 0.f, range_);
		}

		return newVal - oldVal;
	};

};

export template<ResourceType Type>
class ResourceContainerT : public ResourceContainer
{
public:

	ResourceContainerT() = delete;

	constexpr ResourceContainerT(float range, float amount = 0.f) :
		ResourceContainer(range, amount)
	{
		static_assert(range >= amount && range > 0.f);
	};

	virtual ResourceType GetType() const& override
	{
		return Type;
	};
};

export class ResourceContainerD : public ResourceContainer
{
	std::atomic<ResourceType> _type;	
public:

	ResourceContainerD() = delete;

	template<ResourceType Type>
	ResourceContainerD(const ResourceContainerT<Type>& container) :
		ResourceContainer(container._range, container.GetAmount()), _type{ Type }
	{

	};

	template<ResourceType Type>
	ResourceContainerD(const ResourceInfo<Type>& info, float range) :
		ResourceContainer(range, info.amount_), _type{ Type }
	{

	};

	virtual ResourceType GetType() const & override
	{
		return _type.load();
	};
};


//using ResourceInfo = std::pair<ResourceType, float>;

/*struct ResourceInfo {
	const ResourceType _type;
	const float amount_;
	constexpr ResourceInfo(ResourceType type, float amount) :
		_type{ type }, amount_{ amount } {};
};*/

template <typename... Args>
concept NonEmpty = sizeof...(Args) > 0;

template <int a, int b>
concept SameInts = a == b;

export template<ResourceType... Types> requires NonEmpty<ResourceType>
class ResoursePack {
	//using ResourcePackType = decltype(std::tuple(ResourceInfo{ Types, 0.f } ...));
	using ResourcePackType = decltype(std::tuple<decltype(ResourceInfo<Types>{}) ...>());

public:
	const ResourcePackType amounts_;

	ResoursePack() = delete;

	template<class... Values> requires SameInts<sizeof...(Values), sizeof...(Types)>
	//explicit ResoursePack(Values... amounts) : amounts_ { {Types, amounts}...} {
	constexpr explicit ResoursePack(Values... amounts) : 
		amounts_ { (ResourceInfo<Types>{amounts})...} {
	}

	template<ResourceType Type>
	constexpr float GetResourceAmount() const {
		return std::get<ResourceInfo<Type>>(amounts_).amount_;
	}
};