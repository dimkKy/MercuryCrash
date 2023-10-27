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

using BuildingResPack = ResoursePack<
	ResourceType::Composite, ResourceType::Conductor, ResourceType::Time>;

export struct ConstructionInfo {	
	const BuildingResPack resourceToBuild_;
	const float reimburseCoef;

	template<ResourceType Type>
	float GetResourceAmount() const {
		return resourceToBuild_.GetResourceAmount<Type>();
	}
};

export class ConstructibleBase
{
	//how to init?
	const ConstructionInfo& constructionInfo_;
	ResourceContainerT<ResourceType::Composite> compositeStorage_;
	ResourceContainerT<ResourceType::Conductor> conductorStorage_;
	ResourceContainerT<ResourceType::Time> timer_;

	//resource to build -> time to build -> resources after disassemble
	//

	//container to 

protected:
	ConstructibleBase(const ConstructionInfo& constructionInfo) :
		constructionInfo_{ constructionInfo }, 
		compositeStorage_{ constructionInfo.GetResourceAmount<ResourceType::Composite>()}, 
		conductorStorage_{ constructionInfo.GetResourceAmount<ResourceType::Conductor>()},
		timer_{ constructionInfo.GetResourceAmount<ResourceType::Time>()}
	{
			
	}
public:
	virtual ~ConstructibleBase() = default;
};