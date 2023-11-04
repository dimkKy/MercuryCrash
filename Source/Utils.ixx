// by Dmitry Kolontay

export module Utils;

import <cmath>;

export namespace Utils {
	constexpr float kindaSmallTime{ 0.0001f };
	constexpr float kindaSmallAmount{ 0.001f };

	[[nodiscard]] bool NotNegligibleTime(float value) {
		return std::fabsf(value) >= kindaSmallTime;
	}

	[[nodiscard]] bool NotNegligibleAmount(float value) {
		return std::fabsf(value) >= kindaSmallAmount;
	}

	template <typename... Args>
	concept NonEmpty = sizeof...(Args) > 0;

	template <int a, int b>
	concept SameInts = a == b;

	template <class TDerived, class TBase, bool allowSame = false>
	concept ChildOf = std::is_base_of<TBase, TDerived>::value &&
		(allowSame || std::negation<std::is_same<TBase, TDerived>>::value);

	template <typename TDerived, template<typename> typename TBase>
	struct is_derived_from_any
	{
		template<typename TParam>
		static constexpr std::true_type is_derived(const volatile TBase<TParam>&);
		static constexpr std::false_type is_derived(...);
		using type = decltype(is_derived(std::declval<TDerived&>()));
	};

	template <class TDerived, template<typename> typename TBase>
	concept ChildOfAny = is_derived_from_any<TDerived, TBase>::type::value;
};