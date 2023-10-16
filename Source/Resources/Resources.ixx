// by Dmitry Kolontay

export module Resources;
export import Resources.Types;

import <atomic>;
import <utility>;
import <tuple>;

export template<ResourceType Type>
struct ResourceInfo {
	const float _amount;
	constexpr ResourceInfo(float amount = 0.f) :
		_amount{ amount } 
	{
		static_assert(_amount > 0.f);
	};
};

export template<ResourceType Type>
class ResourceContainerT {

	std::atomic<float> _amount;
	
public:
	const float _range;

	ResourceContainerT() = delete;

	constexpr ResourceContainerT(float range, float amount = 0.f) :
		_amount{ amount }, _range{ range }
	{
		static_assert(range >= amount && range > 0.f);
	};
	//ResourceContainerD(const std::pair<float, float>& amountRange, float amount = 0.f);

	//flag as param?
	ResourceType GetType() const&
	{
		return Type;
	};

	float GetAmount() const& {
		return _amount.load();
	}

	float RequestResource(float amount, bool acceptLower = false)&
	{

	};
};

//https://ru.wikipedia.org/wiki/Curiously_recurring_template_pattern base?
export class ResourceContainerD {

	std::atomic<ResourceType> _type;
	std::atomic<float> _amount;
	
public:
	const float _range;

	ResourceContainerD() = delete;

	template<ResourceType Type>
	ResourceContainerD(const ResourceContainerT<Type>& container) :
		_type{ Type }, _amount{ container.GetAmount() }, _range{ container._range }
	{

	};

	template<ResourceType Type>
	ResourceContainerD(const ResourceInfo<Type>& info, float range) :
		_type{ Type }, _amount{ info._amount }, _range{ range }
	{

	};
	//ResourceContainerD(const std::pair<float, float>& amountRange, float amount = 0.f);
	
	//flag as param?
	ResourceType GetType() const &
	{
		return _type.load();
	};


	float RequestResource(float amount, bool acceptLower = false) & 
	{

	};
};



//using ResourceInfo = std::pair<ResourceType, float>;

/*struct ResourceInfo {
	const ResourceType _type;
	const float _amount;
	constexpr ResourceInfo(ResourceType type, float amount) :
		_type{ type }, _amount{ amount } {};
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
	const ResourcePackType _amounts;

	ResoursePack() = delete;

	template<class... Values> requires SameInts<sizeof...(Values), sizeof...(Types)>
	//explicit ResoursePack(Values... amounts) : _amounts { {Types, amounts}...} {
	constexpr explicit ResoursePack(Values... amounts) : 
		_amounts { (ResourceInfo<Types>{amounts})...} {
	}

	template<ResourceType Type>
	constexpr float GetResourceAmount() const {
		return std::get<ResourceInfo<Type>>(_amounts)._amount;
	}
};