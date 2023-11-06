// by Dmitry Kolontay

export module ConstructibleBase;

import Resources;
import Utils;

import <tuple>;

/*class ConstructionInfoProvider {
	ConstructionInfoProvider();
	~ConstructionInfoProvider() = default;

public:
	static ConstructionInfoProvider& Get() {
		static ConstructionInfoProvider instance;
		return instance;
	}

	/*template<class T>
	const ConstructionInfo& GetInfo() const {

	}*
};

export struct ConstructionInfo {	
	const BuildingResPack resourceToBuild_;
	//redo
	static constexpr BasicResPack reimburseCoef_{ 0.25f, 0.5f };

	template<typename... Args>
	constexpr ConstructionInfo(Args&&... args) :
		resourceToBuild_{ std::forward<Args>(args)... } {}

	/*constexpr ConstructionInfo(float com, float con, float t, float reimburseCoef = 0.5f) :
		resourceToBuild_{ com,  con, t },
		reimburseCoef_{ /*reimburseCoef*0.5f } {}*

	template<ResourceType Type>
	[[nodiscard]] float GetRes() const & noexcept {
		return resourceToBuild_.GetRes<Type>();
	}



	/*template<ResourceType Type>
	float GetRes() const {
		return resourceToBuild_.GetRes<Type>();
	}*
};*/

enum class ConstructionStatus {
	Building,
	Constructed,
	Demolition,
};

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
		timer_{ max.GetRes<RT::Time>()},
		status_{ ConstructionStatus::Building }
	{
			
	}

	constexpr ConstructibleBase(const BuildingResPack& max, const BasicResPack& current) :
		//constructionInfo_{ max },
		composite_{ max.GetRes<RT::Composite>(), current.GetRes<RT::Composite>() },
		conductor_{ max.GetRes<RT::Conductor>(), current.GetRes<RT::Conductor>() },
		timer_{ max.GetRes<RT::Time>() }, status_{ ConstructionStatus::Building }
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