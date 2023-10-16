export module Constructible.Worker;
export import Constructible.ConstructibleBase;

// by Dmitry Kolontay

import <memory>;
import Resources;

enum class WorkerStatus
{
	Waiting,
	Changeover,
	Loading,
	Unloading,
	Malfunction,
};

//what should be atomic?
export class Worker : public ConstructibleBase
{
	std::weak_ptr<ResourceContainerD> _source;
	std::weak_ptr<ResourceContainerD> _target;

	ResourceContainerD _storage;

	WorkerStatus _currentStatus;
	//self container
	//on job finished
	
	bool StartWorking(std::weak_ptr<ResourceContainerD>&& source, std::weak_ptr<ResourceContainerD>&& target) &;
	bool FinishWorking() &;

	void Tick(float deltatime) &;

	void OnWorkCompletion();
};