// by Dmitry Kolontay

export module Constructible.ConstructibleBase;

import Resources;

export struct ConstructionInfo {
	const ResourceInfo<ResourceType::Composite> _compositeToBuild;
	const ResourceInfo<ResourceType::Conductor> _conductorsToBuild;
	const ResourceInfo<ResourceType::Time> _timeToBuild;
	const float _reimburseCoef;
};

export class ConstructibleBase
{
	//how to init?
	
	ResourceContainerT<ResourceType::Composite> _compositeStorage;
	ResourceContainerT<ResourceType::Conductor> _conductorStorage;
	//resource to build -> time to build -> resources after disassemble
	//

	//container to 
public:
	virtual ~ConstructibleBase() = default;
};