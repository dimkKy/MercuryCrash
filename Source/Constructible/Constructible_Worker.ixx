// by Dmitry Kolontay

export module Constructible.Worker;
export import Constructible.ConstructibleBase;

import Resources;

import <memory>;
import <cmath>;
import <cassert>;
import <algorithm>;
import <atomic>;
import <stdexcept>;


enum class WorkerStatus
{
	Waiting,
	Changeover,
	Loading,
	Unloading,
	Malfunction,
};

export class WorkOrder {
	
	std::atomic<float> requestedAmount_;

public:
	using ContainerPtr = std::weak_ptr<ResourceContainerD>;
	const ContainerPtr source_;
	const ContainerPtr target_;
	const ContainerPtr fallback_;

	void ChangeAmount(float amount)& {
		requestedAmount_.fetch_add(amount);
	};


	WorkOrder(ContainerPtr&& source, ContainerPtr&& target,
		ContainerPtr&& fallback, float requestedAmount) noexcept(false) :
		source_{source}, target_{ target }, fallback_{ fallback }, 
		requestedAmount_{ requestedAmount }
	{
		if (requestedAmount_ <= 0.f) {
			throw std::invalid_argument{"requestedAmount must be positive"};
		}
		/*if (!source_.operator bool() || source_.expired() || !target_ || target_.expired()) {
			throw std::invalid_argument{ "containers are not of the same type" };
		}
		if (source_->GetType() != target_->GetType() != fallback_->GetType()) {
			throw std::invalid_argument{ "containers are not of the same type" };
		}*/
	}
};

//what should be atomic?
export class Worker : public ConstructibleBase
{
	//class job task?
	std::weak_ptr<WorkOrder> order_;

	float requestedAmount;

	ResourceContainerD storage_;

	ResourceContainerT<ResourceType::Time> timer_;

	constexpr static float workRate{ 1.f };

	WorkerStatus status_;
	//self container
	//on job finished
	void OnWorkCompletion();
	void OnChangeover(float deltatime);
	
	float LoadFrom(float deltatime);
	float UnloadTo(float deltatime);

public:

	bool StartWorking(std::weak_ptr<ResourceContainerD>&& source, std::weak_ptr<ResourceContainerD>&& target)&;
	bool FinishWorking()&;

	void Tick(float deltatime)&;
};

void Worker::Tick(float deltatime)&
{
	assert(deltatime >= 0.f);

	switch (status_) {
	case WorkerStatus::Changeover:
		{
			float timeLeft{ deltatime - timer_.ChangeAmount(deltatime) };
			//CHANGE STATUS
			if (std::fabsf(timeLeft) > 0.001) {
				Tick(timeLeft);
			}
		}
		break;
	case WorkerStatus::Loading:
		LoadFrom(deltatime);
		break;
	case WorkerStatus::Unloading:
		UnloadTo(deltatime);
		break;
	default:
		//untick?
		break;
	}
	
}

void Worker::OnWorkCompletion()
{

}

void Worker::OnChangeover(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Changeover);

	float timeLeft{ deltatime - timer_.ChangeAmount(deltatime) };
	if (std::fabsf(timeLeft) > 0.001) {
		Tick(timeLeft);
	}
}

float Worker::LoadFrom(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Loading);

	float availableSpace{ storage_.GetFreeSpace() };
	float workAvailable{ deltatime * workRate };
	/*ADD requestedAmount
	float workRequested{ std::min(availableSpace, workAvailable) };
	float workDone{ 0.f };

	if (auto lockedSouce{ order_.lock() }) {
		workDone = -1.f * lockedSouce->ChangeAmount(-1.f * workRequested);
	}
	else {
		//try to return?
	}

	requestedAmount -= workDone;
	//float */
	return 0.0f;
}

float Worker::UnloadTo(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Unloading);

	/*float availableSpace{ storage_.GetFreeSpace() };
	float workAvailable{ std::min(deltatime * workRate, storage_.GetFreeSpace()) };
	//ADD requestedAmount
	//float workRequested{ std::min(availableSpace, workAvailable) };
	float workDone{ 0.f };

	if (auto lockedTaget{ target_.lock() }) {
		workDone = lockedTaget->ChangeAmount(workAvailable);
	}
	else {
		//try to return?
	}

	requestedAmount -= workDone;
	*/
	return 0.0f;
}
