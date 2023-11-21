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

	template<class T, class... Classes>
	concept AllSame = (std::is_same_v<T, Classes> && ...);

	template <int a, int b>
	concept SameInts = a == b;

	template <class TDerived, class TBase, bool allowSame = false>
	concept ChildOf = std::is_base_of_v<TBase, TDerived> &&
		(allowSame || !std::is_same_v<TBase, TDerived>);

	template <typename TDerived, template<typename> typename TBase>
	struct is_derived_from_any
	{
		template<typename TParam>
		static constexpr std::true_type is_derived(const TBase<TParam>&);
		static constexpr std::false_type is_derived(...);
		using type = decltype(is_derived(std::declval<TDerived&>()));
	};

	template <class TDerived, template<typename> typename TBase>
	using is_derived_from_any_t = is_derived_from_any<TDerived, TBase>::type;

	template <class TDerived, template<typename> typename TBase>
	inline constexpr bool is_derived_from_any_v{ is_derived_from_any_t<TDerived, TBase>::value };

	template <class TDerived, template<typename> typename TBase>
	concept ChildOfAny = is_derived_from_any_v<TDerived, TBase>;
};