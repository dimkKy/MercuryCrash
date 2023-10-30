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
	
	constexpr ResourceInfo(float amount = 0.f) noexcept:
		amount_{ amount } 
	{
	};
};

//ADD NON-ATOMIC?
//DYNAMIC NON ATOMIC +
//TYPED NON ATOMIC +
//!!!!!!

export class Container{
	std::atomic<float> amount_;

protected:

	constexpr Container(float range, float amount = 0.f) noexcept(false) :
		amount_{ amount }, range_{ range }
	{
		if (range < amount || range <= 0.f) {
			throw std::invalid_argument{"range must be positive and not less then amount"};
		}
	};

public:
	using AmountType = decltype(amount_)::value_type;
	const float range_;

	Container() = delete;
	virtual ~Container() = default;

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
class ContainerT : public Container
{
public:
	using Container::AmountType;

	ContainerT() = delete;

	constexpr ContainerT(float range, float amount = 0.f) :
		Container(range, amount)
	{
		//static_assert(range >= amount && range > 0.f);
	};

	virtual ResourceType GetType() const& override
	{
		return Type;
	};
};

export class ContainerD : public Container
{
	std::atomic<ResourceType> _type;	
public:
	using Container::AmountType;
	ContainerD() = delete;

	template<ResourceType Type>
	ContainerD(const ContainerT<Type>& container) :
		Container(container._range, container.GetAmount()), _type{ Type }
	{

	};

	template<ResourceType Type>
	ContainerD(const ResourceInfo<Type>& info, float range) :
		Container(range, info.amount_), _type{ Type }
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
	constexpr float GetAmount() const {
		return std::get<ResourceInfo<Type>>(amounts_).amount_;
	}
};