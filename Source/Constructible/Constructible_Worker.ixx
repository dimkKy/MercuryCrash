// by Dmitry Kolontay

export module Constructible.Worker;
export import Constructible.Worker.WorkOrder;
export import Constructible.ConstructibleBase;

import Resources;
import Utils;

import <memory>;
//import <cmath>;
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

//what should be atomic?
export class Worker : public ConstructibleBase
{
	std::weak_ptr<WorkOrder> order_;
	std::weak_ptr<WorkOrder> nextOrder_;

	ResourceContainerD storage_;

	ResourceContainerT<ResourceType::Time> timer_;

	constexpr static float workRate{ 1.f };

	WorkerStatus status_;
	//self container
	//on job finished
	void OnWorkCompletion();

	//returns time left
	float OnChangeover(float deltatime);
	//returns time left
	float LoadFrom(float deltatime);
	//returns time left
	float AddTo(float deltatime);

	void StartChangeover();
	void StartChangeover(float initTime);

public:
	bool AssignWorkOrder(const std::shared_ptr<WorkOrder>& order)&;
	void FinishWorking()&;

	void Tick(float deltatime)&;
};

void Worker::Tick(float deltatime)&
{
	assert(deltatime > 0.f);
	//decrease power
	switch (status_) {
	case WorkerStatus::Changeover:
		{
			float timeLeft{ deltatime - timer_.ChangeAmount(deltatime) };
			//CHANGE STATUS
			if (Utils::NotNegligibleTime(timeLeft)) {

				Tick(timeLeft);
			}
		}
		break;
	case WorkerStatus::Loading:
		LoadFrom(deltatime);
		break;
	case WorkerStatus::Unloading:
		AddTo(deltatime);
		break;
	default:
		//untick?
		break;
	}
	
}

void Worker::OnWorkCompletion()
{
	//try return resources?
}

void Worker::StartChangeover()
{
	status_ = WorkerStatus::Changeover;
	//redo
	timer_.Reset();
}

void Worker::StartChangeover(float initTime)
{
	status_ = WorkerStatus::Changeover;
	//redo
	timer_.Reset();
	OnChangeover(initTime);
}

float Worker::OnChangeover(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Changeover);

	return deltatime - timer_.ChangeAmount(deltatime);
}

//no amount check!
float Worker::LoadFrom(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Loading);

	/*if (auto order{ order_.lock() }) {
		float workDone{ order->RequestFromSource(
			std::min(storage_.GetFreeSpace(), deltatime * workRate)) };
		storage_.ChangeAmount(workDone);
		return deltatime - workDone / workRate;
	}
	else {
		return deltatime;
	}*/
	float workDone{ 0.f };
	if (auto order{ order_.lock() }) {
		workDone = order->RequestFromSource(
			std::min(storage_.GetFreeSpace(), deltatime * workRate));
	}
	else {
		return deltatime;
	}
	storage_.ChangeAmount(-1.f * workDone);
	return deltatime - workDone / workRate;
}

float Worker::AddTo(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Unloading);

	/*if (auto order{ order_.lock() }) {
		float workDone{ order->UnloadToTarget(
			std::min(storage_.GetAmount(), deltatime * workRate), true) };
		storage_.ChangeAmount(-1.f * workDone);
		return deltatime - workDone / workRate;
	}
	else {
		return deltatime;
	}
	*/
	float workDone{ 0.f };
	if (auto order{ order_.lock() }) {
		workDone = order->UnloadToTarget(
			std::min(storage_.GetAmount(), deltatime * workRate), true);
	}
	else {
		return deltatime;
	}
	storage_.ChangeAmount(-1.f * workDone);
	return deltatime - workDone / workRate;
}

bool Worker::AssignWorkOrder(const std::shared_ptr<WorkOrder>& order)&
{
	switch (status_) {
	case WorkerStatus::Changeover:
		[[fallthrough]];
	case WorkerStatus::Loading:
		[[fallthrough]];
	case WorkerStatus::Unloading:
		if (order_.lock() == order) {
			break;
		}
		[[fallthrough]];
	case WorkerStatus::Waiting:
		StartChangeover();
		[[fallthrough]];
	case WorkerStatus::Malfunction:
		[[fallthrough]];
	default:
		order_ = order;
		break;
	}
	return true;
}

void Worker::FinishWorking()&
{
	order_.reset();
	//try return resources?
	status_ = WorkerStatus::Waiting;
	
}
