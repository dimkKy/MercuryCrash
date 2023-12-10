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
	BasicResPack storageInitState_{ 0.f, 1000.f };

	int solarPanels_{ 2 };
	int cryochambers_{ 5 };
	int batteries_{ 1 };
	int workers_{ 1 };

	BasicResPack reimburseCoef_{ 0.25f, 0.5f };

	float mineAmount_{ std::numeric_limits<float>::max() };

	using BuildInfoPair = std::pair<BuildingResPack, BasicResPack>;

	std::array<BuildInfoPair, static_cast<size_t>(ST::MAX)> buildInfos{
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} }, // Battery
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} }, // SolarPanel
		std::pair{ BuildingResPack{900.f, 500.f, 100.f}, BasicResPack{100.f, 50.f} }, // Reactor
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} }, // Cryochamber
		std::pair{ BuildingResPack{900.f, 500.f, 100.f}, BasicResPack{100.f, 50.f} }, // Hull
		std::pair{ BuildingResPack{1.f, 1.f, 1.f}, BasicResPack{1.f, 1.f} }, // Worker
	};

	constexpr BalanceInfo() noexcept = default;
};

export class BalanceSettings {
	BalanceInfo info_;

	constexpr BalanceSettings() noexcept = default;
	~BalanceSettings() = default;

	/*[[nodiscard]] static BalanceInfo GetInfo() noexcept {
		return Get().info_;
	}*/

	[[nodiscard]] static const BalanceInfo& GetInfo() noexcept;

public:
	[[nodiscard]] static BalanceSettings& Get() noexcept;

	[[nodiscard]] static constexpr bool Verify(const BalanceInfo& info) noexcept;
	[[nodiscard]] static bool Set(const BalanceInfo& info) noexcept;

	[[nodiscard]] static BasicResPack StorageInitState() noexcept;
	[[nodiscard]] static float MineInitState() noexcept;

	template<ResourceType Type>
	[[nodiscard]] static constexpr float GetReimburseCoef() noexcept;

	template<StructureType Type>
	[[nodiscard]] static int InitStructureCount() noexcept;

	//are requireses required?
	template<StructureType StT> requires
		(static_cast<int>(StT) < static_cast<int>(ST::MAX))
	[[nodiscard]] static constexpr BuildingResPack MaxBuildRes() {
		return GetInfo().buildInfos[static_cast<size_t>(StT)].first;
	}

	template<class T> requires requires 
		{ { T::StructType } -> std::same_as<StructureType>; }
	[[nodiscard]] static constexpr BuildingResPack MaxBuildRes() {
		return MaxBuildRes<T::StructType>;
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

	template<class T> requires requires
	{ { T::StructType } -> std::same_as<StructureType>; }
	[[nodiscard]] static constexpr BasicResPack InitRes() {
		return InitRes<T::StructType>;
	}

	template<StructureType StT, ResourceType RsT> requires
		(static_cast<int>(StT) < static_cast<int>(ST::MAX))
	[[nodiscard]] static constexpr float InitRes() {
		return InitRes<StT>().GetRes<RsT>();
	}
};

const BalanceInfo& BalanceSettings::GetInfo() noexcept
{
	return Get().info_;
}

BalanceSettings& BalanceSettings::Get() noexcept
{
	static BalanceSettings instance{};
	return instance;
}

constexpr bool BalanceSettings::Verify(const BalanceInfo& info) noexcept
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

	isValid = info.batteries_ >= 1 && info.cryochambers_ >= 0 && 
		info.solarPanels_ >= 1 && info.workers_ >= 1;

	return isValid;
}
static_assert(BalanceSettings::Verify({}), 
	"default-constructed BalanceSettingsInfo is invalid");

bool BalanceSettings::Set(const BalanceInfo& info) noexcept 
{
	if (Verify(info)) {
		Get().info_ = info;
		return true;
	}
	else {
		return false;
	}
}

BasicResPack BalanceSettings::StorageInitState() noexcept
{
	return GetInfo().storageInitState_;
}

float BalanceSettings::MineInitState() noexcept
{
	return GetInfo().mineAmount_;
}

template<ResourceType Type>
constexpr float BalanceSettings::GetReimburseCoef() noexcept
{
	return GetInfo().reimburseCoef_.GetRes<Type>();
}

template<StructureType Type>
int BalanceSettings::InitStructureCount() noexcept
{
	static_assert(Type != StructureType::ST_MAX, "Invalid structure type");
	return 1;
}

template<>
int BalanceSettings::InitStructureCount<ST::Battery>() noexcept
{
	return GetInfo().batteries_;
}

template<>
int BalanceSettings::InitStructureCount<ST::Cryochamber>() noexcept
{
	return GetInfo().cryochambers_;
}

template<>
int BalanceSettings::InitStructureCount<ST::SolarPanel>() noexcept
{
	return GetInfo().solarPanels_;
}

template<>
int BalanceSettings::InitStructureCount<ST::Worker>() noexcept
{
	return GetInfo().workers_;
}