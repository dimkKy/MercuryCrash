// by Dmitry Kolontay

export module Constructible.ConstructibleBase;

import Resources;
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

export using BuildingResPack = ResoursePack<RT::Composite, RT::Conductor, RT::Time>;

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

export class ConstructibleBase
{
	//how to init?
	const ConstructionInfo& constructionInfo_;
	ContainerT<RT::Composite> composite_;
	ContainerT<RT::Conductor> conductor_;
	ContainerT<RT::Time> timer_;

	//resource to build -> time to build -> resources after disassemble
	//

	//container to 

protected:
	ConstructibleBase(const ConstructionInfo& info) :
		constructionInfo_{ info }, 
		composite_{ info.GetAmount<RT::Composite>()},
		conductor_{ info.GetAmount<RT::Conductor>()},
		timer_{ info.GetAmount<RT::Time>()}
	{
			
	}

	ConstructibleBase(const ConstructionInfo& info, const BasicResPack& pack) :
		constructionInfo_{ info },
		composite_{ info.GetAmount<RT::Composite>(), pack.GetAmount<RT::Composite>() },
		conductor_{ info.GetAmount<RT::Conductor>(), pack.GetAmount<RT::Conductor>() },
		timer_{ info.GetAmount<RT::Time>() }
	{

	}
public:
	virtual ~ConstructibleBase() = default;
};