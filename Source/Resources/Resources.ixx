// by Dmitry Kolontay

export module Resources;
export import Resources.Types;

import Utils;

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

//ADD NON-ATOMIC?
//DYNAMIC NON ATOMIC +
//TYPED NON ATOMIC +
//!!!!!!

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
		return amount_.load(std::memory_order_relaxed);
	};

	float GetFreeSpace() const& {
		return range_ - GetAmount();
	};

	//returns newVal - oldVal
	float ChangeAmount(float amount)& {
		float oldVal{ GetAmount() };
		float newVal{ std::clamp(oldVal + amount, 0.f, range_) };

		while (!amount_.compare_exchange_weak(oldVal, newVal, std::memory_order_acq_rel))
		{
			newVal = std::clamp(oldVal + amount, 0.f, range_);
		}

		return newVal - oldVal;
	};

	void Reset()& {
		amount_.store(0.f, std::memory_order_release);
	}

};

export template<ResourceType Type>
class ResourceContainerT : public ResourceContainer
{
public:

	ResourceContainerT() = delete;

	constexpr ResourceContainerT(float range, float amount = 0.f) :
		ResourceContainer(range, amount)
	{
		//static_assert(range >= amount && range > 0.f);
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

export template<ResourceType... Types> requires Utils::NonEmpty<ResourceType>
class ResoursePack {
	//using ResourcePackType = decltype(std::tuple(ResourceInfo{ Types, 0.f } ...));
	using ResourcePackType = decltype(std::tuple<decltype(ResourceInfo<Types>{}) ...>());

public:
	const ResourcePackType amounts_;

	ResoursePack() = delete;

	template<class... Values> requires Utils::SameInts<sizeof...(Values), sizeof...(Types)>
	//explicit ResoursePack(Values... amounts) : amounts_ { {Types, amounts}...} {
	constexpr explicit ResoursePack(Values... amounts) : 
		amounts_ { (ResourceInfo<Types>{amounts})...} {
	}

	template<ResourceType Type>
	constexpr float GetResourceAmount() const {
		return std::get<ResourceInfo<Type>>(amounts_).amount_;
	}
};