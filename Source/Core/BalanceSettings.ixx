// by Dmitry Kolontay

export module BalanceSettings;

import Resources;

import <limits>;
import <tuple>;

export struct BalanceSettingsInfo {
private:
	BuildingResPack hullInfo_;
	BuildingResPack reactorInfo_;

	BasicResPack hullHealthInit_;
	BasicResPack reactorHealthInit_;
	BasicResPack storage_;

	int solarPanels_;
	int cryochambers_;
	int batteries_;
	int workers_;

	BasicResPack reimburseCoef_;

	float mineAmount_;
public:
	constexpr BalanceSettingsInfo() noexcept :
		hullInfo_{ 900.f, 500.f, 100.f }, reactorInfo_{ 900.f, 500.f, 100.f },
		hullHealthInit_{ 100.f, 50.f }, reactorHealthInit_{ 100.f, 50.f },
		storage_{ 0.f, 1000.f }, solarPanels_{ 2 }, cryochambers_{ 5 },
		batteries_{ 1 }, workers_{ 1 }, reimburseCoef_{ 0.25f, 0.5f },
		mineAmount_{ std::numeric_limits<float>::max() } {}
};

export class BalanceSettings {
	BalanceSettingsInfo info_;

	~BalanceSettings() = default;
	constexpr BalanceSettings() = default;

	/*[[nodiscard]] static BalanceSettingsInfo GetInfo() noexcept {
		return Get().info_;
	}*/

public:
	static BalanceSettings& Get() {
		static BalanceSettings instance;
		return instance;
	}

	static void Set(const BalanceSettingsInfo& info) {
		Get().info_ = info;
	}

	template<ResourceType Type>
	static float GetReimburseCoef() {
		return Get().info_reimburseCoef_.GetRes<Type>();
	}
};