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
	float amount_;
	
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
		if (range < amount || range <= 0.f) [[unlikely]] {
			throw std::invalid_argument{"range must be positive and not less then amount"};
		}
	};

public:
	using AmountType = decltype(amount_)::value_type;
	const float range_;

	Container() = delete;
	virtual ~Container() = default;

	virtual ResourceType GetType() const& = 0;

	float GetRes() const& {
		return amount_.load(std::memory_order_relaxed);
	};

	float GetFreeSpace() const& {
		return range_ - GetRes();
	};

	//returns newVal - oldVal
	float ChangeAmount(float amount)& {
		float oldVal{ GetRes() };
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

	bool SetAmountForced(float amount)& {
		if (amount >= -0.f && amount <= range_) [[likely]] {
			amount_.store(amount);
			return true;
		}
		else {
			return false;
		}
	}
};

export class ContainerD : public Container
{
	std::atomic<ResourceType> _type;	
public:
	using Container::AmountType;
	ContainerD() = delete;

	template<ResourceType Type>
	ContainerD(const ContainerT<Type>& container) :
		Container(container._range, container.GetRes()), _type{ Type }
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
template<class T, class... Classes>
concept AllSame = (std::is_same_v<T, Classes> && ...);

export template<ResourceType... Types> requires Utils::NonEmpty<ResourceType>
class ResoursePack {
	using ResourcePackType = std::tuple<decltype(ResourceInfo<Types>{}) ...>;

public:
	ResourcePackType amounts_;
	
	ResoursePack() = delete;

	template<class... Values> 
		requires Utils::SameInts<sizeof...(Values), sizeof...(Types)> && AllSame<float, Values...>
	constexpr ResoursePack(Values... amounts) noexcept: 
		amounts_ { (ResourceInfo<Types>{amounts})...} {
	}
	//add ctor from another?
	template<ResourceType Type>
	constexpr float GetRes() const {
		return std::get<ResourceInfo<Type>>(amounts_).amount_;
	}

	constexpr ResourcePackType Get() const {
		return amounts_;
	}
};

export using BuildingResPack =
	ResoursePack<RT::Composite, RT::Conductor, RT::Time>;

export using BasicResPack = ResoursePack<RT::Composite, RT::Conductor>;