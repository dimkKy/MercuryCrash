// by Dmitry Kolontay

export module ConstructibleBase;

import Resources;
import BalanceSettings;
import Utils;

import <tuple>;

enum class ConstructionStatus {
	Building,
	Constructed,
	Demolition,
};

export namespace CS {
	inline constexpr ConstructionStatus Building
		{ ConstructionStatus::Building };
	inline constexpr ConstructionStatus Constructed
		{ ConstructionStatus::Constructed };
	inline constexpr ConstructionStatus Demolition
		{ ConstructionStatus::Demolition };
}

export class ConstructibleBase
{
	//how to init?
	//const ConstructionInfo& constructionInfo_;
	ContainerT<RT::Composite> composite_;
	ContainerT<RT::Conductor> conductor_;
	ContainerT<RT::Time> timer_;

	ConstructionStatus status_;
	//resource to build -> time to build -> resources after disassemble
	//

	//container to 

protected:

	constexpr ConstructibleBase(const BuildingResPack& max) :
		//constructionInfo_{ max }, 
		composite_{ max.GetRes<RT::Composite>()},
		conductor_{ max.GetRes<RT::Conductor>()},
		timer_{ max.GetRes<RT::Time>()}
	{
		if (status_ == CS::Constructed) {
			//composite_
		}
	}

	constexpr ConstructibleBase(const ConstructibleBase& other) noexcept = default;

	static constexpr ConstructibleBase GetConstructed(const BuildingResPack& max);

	constexpr ConstructibleBase(const BuildingResPack& max, const BasicResPack& current) :
		//constructionInfo_{ max },
		composite_{ max.GetRes<RT::Composite>(), current.GetRes<RT::Composite>() },
		conductor_{ max.GetRes<RT::Conductor>(), current.GetRes<RT::Conductor>() },
		timer_{ max.GetRes<RT::Time>() }, status_{ CS::Building }
	{

	}

	constexpr bool FinishBuilding()& {
		switch (status_) {
		case CS::Constructed:
			return true;
		case CS::Demolition:
			return false;
		case CS::Building:
			[[fallthrough]];
		default:
			break;
		}
		//check?
	}
public:

	Container* CheckIfBuilding() & {
		if (status_ != CS::Building) {
			return nullptr;
		}

		if(Utils::NotNegligibleAmount(composite_.GetFreeSpace()) ||
			Utils::NotNegligibleAmount(conductor_.GetFreeSpace())) {
			//status?
			return nullptr;
		}
		else {
			return &timer_;
		}
	}

	virtual ~ConstructibleBase() = default;
};

constexpr ConstructibleBase ConstructibleBase::GetConstructed(const BuildingResPack& max)
{
	ConstructibleBase res{ max };
	res.FinishBuilding();
	return res;
}
