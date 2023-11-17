// by Dmitry Kolontay

export module BalanceSettings;

import Resources;
import Structures.Types;

import <tuple>;
import <array>;
import <limits>;
import <utility>;
import <stdexcept>;

export struct BalanceInfo {
	BuildingResPack hullInfo_{ 900.f, 500.f, 100.f };
	BuildingResPack reactorInfo_{ 900.f, 500.f, 100.f };

	BasicResPack hullHealthInit_{ 100.f, 50.f };
	BasicResPack reactorHealthInit_{ 100.f, 50.f };
	BasicResPack storageInitState_{ 0.f, 1000.f };

	int solarPanels_{ 2 };
	int cryochambers_{ 5 };
	int batteries_{ 1 };
	int workers_{ 1 };

	BasicResPack reimburseCoef_{ 0.25f, 0.5f };

	float mineAmount_{ std::numeric_limits<float>::max() };

	using BuildInfoPair = std::pair<BuildingResPack, BasicResPack>;

	std::array<BuildInfoPair, static_cast<size_t>(ST::MAX)> buildInfos{
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} },
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} },
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} },
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} },
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} },
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} },
	};

	constexpr BalanceInfo() noexcept = default;
};

/*constexpr BalanceInfo::BalanceInfo() noexcept :
	hullInfo_{ 900.f, 500.f, 100.f }, reactorInfo_{ 900.f, 500.f, 100.f },
	hullHealthInit_{ 100.f, 50.f }, reactorHealthInit_{ 100.f, 50.f },
	storageInitState_{ 0.f, 1000.f }, solarPanels_{ 2 }, cryochambers_{ 5 },
	batteries_{ 1 }, workers_{ 1 }, reimburseCoef_{ 0.25f, 0.5f },
	mineAmount_{ std::numeric_limits<float>::max() } {}*/

export class BalanceSettings {
	BalanceInfo info_;

	constexpr BalanceSettings() noexcept = default;
	~BalanceSettings() = default;

	/*[[nodiscard]] static BalanceInfo GetInfo() noexcept {
		return Get().info_;
	}*/

	[[nodiscard]] static const BalanceInfo& GetInfo() noexcept {
		return Get().info_;
	}

public:
	[[nodiscard]] static BalanceSettings& Get() noexcept {
		static BalanceSettings instance;
		return instance;
	}

	[[nodiscard]] static constexpr bool Verify(const BalanceInfo& info);
	[[nodiscard]] static bool Set(const BalanceInfo& info);

	template<ResourceType Type>
	[[nodiscard]] static constexpr float GetReimburseCoef() {
		return GetInfo().reimburseCoef_.GetRes<Type>();
	} 
	//are requireses required?
	template<StructureType StT> requires
		(static_cast<int>(StT) < static_cast<int>(ST::MAX))
	[[nodiscard]] static constexpr BuildingResPack MaxBuildRes() {
		return GetInfo().buildInfos[static_cast<size_t>(StT)].first;
	}

	template<StructureType StT, ResourceType RsT> requires 
		(static_cast<int>(StT) < static_cast<int>(ST::MAX))
	[[nodiscard]] static constexpr float MaxBuildRes() {
		return MaxBuildRes<StT>().GetRes<RsT>();
	}

	template<StructureType StT> requires
		(static_cast<int>(StT) < static_cast<int>(ST::MAX))
	[[nodiscard]] static constexpr BasicResPack InitRes() {
		return GetInfo().buildInfos[static_cast<size_t>(StT)].second;
	}

	template<StructureType StT, ResourceType RsT> requires
		(static_cast<int>(StT) < static_cast<int>(ST::MAX))
	[[nodiscard]] static constexpr float InitRes() {
		return InitRes<StT>().GetRes<RsT>();
	}
};

constexpr bool BalanceSettings::Verify(const BalanceInfo& info)
{
	bool isValid{ true };

	auto reimburseCoefChecker = [] (auto&&... resInfos) constexpr noexcept -> bool
		{ return ((resInfos.amount_ >= -0.f && resInfos.amount_ <= 1.f) && ...); };

	isValid = std::apply(reimburseCoefChecker, info.reimburseCoef_.Get());

	//redo
	auto buildInfosCheckerHelper = []
		(const BalanceInfo::BuildInfoPair& pair) constexpr noexcept -> bool { 
			return pair.second.GetRes<RT::Composite>() >= 0.f &&
			pair.first.GetRes<RT::Composite>() >= pair.second.GetRes<RT::Composite>() &&
			pair.second.GetRes<RT::Conductor>() >= 0.f &&
			pair.first.GetRes<RT::Conductor>() >= pair.second.GetRes<RT::Conductor>() &&
			pair.first.GetRes<RT::Time>() >= 0.f; 
		};

	auto buildInfosChecker = [&buildInfosCheckerHelper]
		(auto&&... buildInfos) constexpr noexcept -> bool { 
			return (buildInfosCheckerHelper(buildInfos) && ...); 
		};

	isValid = std::apply(buildInfosChecker, info.buildInfos);

	return isValid;
}
static_assert(BalanceSettings::Verify({}), 
	"default-constructed BalanceSettingsInfo is invalid");

bool BalanceSettings::Set(const BalanceInfo& info) {
	if (Verify(info)) {
		Get().info_ = info;
		return true;
	}
	else {
		return false;
	}
}
