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
		amount_{ amount } {};
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

	constexpr Container(const Container& other) noexcept :
		amount_{ other.amount_.load(std::memory_order_relaxed) }, range_{ other.range_ }
	{};

	//delete?
	Container& operator=(const Container& other) noexcept
	{
		range_ = other.range_;
		amount_.store(other.amount_.load(std::memory_order_relaxed), std::memory_order_relaxed);
		return *this;
	}

public:
	using AmountType = decltype(amount_)::value_type;
	float range_;

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

	constexpr ContainerT(const ContainerT& other) noexcept = default;

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

export template<ResourceType... Types> requires Utils::NonEmpty<ResourceType>
class ResoursePack {
	using ResourcePackType = std::tuple<decltype(ResourceInfo<Types>{}) ...>;

public:
	ResourcePackType amounts_;

	ResoursePack() = delete;

	template<class... Values> 
		requires Utils::AllSame<float, Values...> && 
			Utils::SameInts<sizeof...(Values), sizeof...(Types)>
	constexpr ResoursePack(Values... amounts) noexcept: 
		amounts_ { (ResourceInfo<Types>{amounts})...} {}

	template<ResourceType... OtherTypes>
	constexpr ResoursePack(const ResoursePack<OtherTypes...>& other) noexcept {
		(SetRes<Types>(other.GetRes<Types>()), ...);
	}

	template<ResourceType... OtherTypes>
	constexpr ResoursePack& operator=(const ResoursePack<OtherTypes...>& other) noexcept
		requires std::same_as<ResourcePackType, ResoursePack<OtherTypes...>::ResourcePackType> {
		(SetRes<Types>(other.GetRes<Types>()), ...);
		return *this;
	}

	template<ResourceType Type>
	constexpr float GetRes() const noexcept {
		return 0.f;
	}

	template<ResourceType Type>
	constexpr float GetRes() const noexcept requires
		requires{ std::get<ResourceInfo<Type>>(amounts_); } {
		return std::get<ResourceInfo<Type>>(amounts_).amount_;
	}

	constexpr ResourcePackType Get() const noexcept {
		return amounts_;
	}

protected:
	template<ResourceType Type>
	constexpr void SetRes(float amount) noexcept requires
		requires{ std::get<ResourceInfo<Type>>(amounts_); } {
		std::get<ResourceInfo<Type>>(amounts_).amount_ = amount;
	}
};

export using BuildingResPack =
	ResoursePack<RT::Composite, RT::Conductor, RT::Time>;

export using BasicResPack = ResoursePack<RT::Composite, RT::Conductor>;