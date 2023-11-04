// by Dmitry Kolontay

export module ConstructibleBase;

import Resources;
import Utils;

import <tuple>;

class ConstructionInfoProvider {
	ConstructionInfoProvider();
	~ConstructionInfoProvider() = default;

public:
	static ConstructionInfoProvider& Get() {
		static ConstructionInfoProvider instance;
		return instance;
	}

	/*template<class T>
	const ConstructionInfo& GetInfo() const {

	}*/
};

export using BuildingResPack = 
	ResoursePack<RT::Composite, RT::Conductor, RT::Time>;

export using BasicResPack = ResoursePack<RT::Composite, RT::Conductor>;

export struct ConstructionInfo {	
	const BuildingResPack resourceToBuild_;
	const float reimburseCoef;

	template<ResourceType Type>
	float GetAmount() const {
		return resourceToBuild_.GetAmount<Type>();
	}

	/*template<ResourceType Type>
	float GetAmount() const {
		return resourceToBuild_.GetAmount<Type>();
	}*/
};

enum class ConstructionStatus {
	Building,
	Constructed,
	Demolition,
};

export class ConstructibleBase
{
	//how to init?
	const ConstructionInfo& constructionInfo_;
	ContainerT<RT::Composite> composite_;
	ContainerT<RT::Conductor> conductor_;
	ContainerT<RT::Time> timer_;

	ConstructionStatus status_;
	//resource to build -> time to build -> resources after disassemble
	//

	//container to 

protected:
	constexpr ConstructibleBase(const ConstructionInfo& info) :
		constructionInfo_{ info }, 
		composite_{ info.GetAmount<RT::Composite>()},
		conductor_{ info.GetAmount<RT::Conductor>()},
		timer_{ info.GetAmount<RT::Time>()},
		status_{ ConstructionStatus::Building }
	{
			
	}

	constexpr ConstructibleBase(const ConstructionInfo& info, const BasicResPack& pack) :
		constructionInfo_{ info },
		composite_{ info.GetAmount<RT::Composite>(), pack.GetAmount<RT::Composite>() },
		conductor_{ info.GetAmount<RT::Conductor>(), pack.GetAmount<RT::Conductor>() },
		timer_{ info.GetAmount<RT::Time>() }, status_{ ConstructionStatus::Building }
	{

	}
public:

	Container* VerifyForConstruction(){
		if (status_ != ConstructionStatus::Building) {
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