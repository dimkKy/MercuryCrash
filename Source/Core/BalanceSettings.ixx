
export module BalanceSettings;

import Resources;

import <limits>;
import <tuple>;

export struct BalanceSettings {
	const BuildingResPack hullInfo_;
	const BuildingResPack reactorInfo_;

	const BasicResPack hullHealthInit_;
	const BasicResPack reactorHealthInit_;
	const BasicResPack storage_;

	const int solarPanels_;
	const int cryochambers_;
	const int batteries_;
	const int workers_;

	const BasicResPack reimburseCoef_;

	const float mineAmount_;

	//* operators to resPack
	constexpr BalanceSettings() noexcept :
		hullInfo_{ 900.f, 500.f, 100.f }, reactorInfo_{ 900.f, 500.f, 100.f },
		hullHealthInit_{ 100.f, 50.f }, reactorHealthInit_{ 100.f, 50.f },
		storage_{ 0.f, 1000.f }, solarPanels_{ 2 }, cryochambers_{ 5 },
		batteries_{ 1 }, workers_{ 1 }, reimburseCoef_{ 0.25f, 0.5f },
		mineAmount_{ std::numeric_limits<float>::max() } {}
};