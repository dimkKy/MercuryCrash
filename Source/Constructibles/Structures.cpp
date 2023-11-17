// by Dmitry Kolontay

module Structures;

import WorkOrder;
import <cassert>;

void Worker::Tick(float deltatime) &
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
	StartChangeover();
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

	float workDone{ 0.f };
	if (auto order{ order_.lock() }) {
		workDone = order->RequestFromSource(
			std::min(storageInitState_.GetFreeSpace(), deltatime * workRate));
	}
	else {
		return deltatime;
	}
	storageInitState_.ChangeAmount(-1.f * workDone);
	return deltatime - workDone / workRate;
}

float Worker::AddTo(float deltatime)
{
	assert(deltatime >= 0.f && status_ == WorkerStatus::Unloading);

	float workDone{ 0.f };
	if (auto order{ order_.lock() }) {
		workDone = order->UnloadToTarget(
			std::min(storageInitState_.GetRes(), deltatime * workRate));
	}
	else {
		return deltatime;
	}
	storageInitState_.ChangeAmount(-1.f * workDone);
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