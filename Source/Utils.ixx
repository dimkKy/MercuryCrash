export module Utils;

import <cmath>;

export namespace Utils {
	constexpr float kindaSmallTime{ 0.001f };

	[[nodiscard]] float NotNegligibleTime(float value) {
		return std::fabsf(value) >= kindaSmallTime;
	}

	template <typename... Args>
	concept NonEmpty = sizeof...(Args) > 0;

	template <int a, int b>
	concept SameInts = a == b;
};