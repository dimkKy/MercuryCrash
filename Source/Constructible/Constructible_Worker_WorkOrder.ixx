export module Constructible.Worker.WorkOrder;

import Resources;

import <memory>;
import <cassert>;
import <algorithm>;
import <atomic>;
import <stdexcept>;

/*
* > Loading
* > unloading
*/

//build order and return order
template<ResourceType Type>
using ContainerPtrT = std::weak_ptr<ContainerT<Type>>;
using ContainerPtrD = std::weak_ptr<Container>;

export class WorkOrder {
protected:
	float AddTo(const ContainerPtrD& to, float amount)& {
		assert(amount >= 0.f);
		if (auto target{ to.lock() }) {
			return target->ChangeAmount(amount);
		}
		else {
			return 0.f;
		}
	}

public:
	//returns amount acquired (positive)
	virtual float RequestFromSource(float amount) & = 0;

	virtual float UnloadToTarget(float amount, bool bAllowReturn) & = 0;
	virtual ~WorkOrder() = default;
};

export class BuildOrder : WorkOrder {
	const ContainerPtrT<RT::Time> timer_;
	const ContainerPtrT<RT::Conductor> conductor_;
	const ContainerPtrT<RT::Composite> composite_;

	//TODO
public:
	BuildOrder() {

	}

	virtual float RequestFromSource(float amount) & override {

	}

	virtual float UnloadToTarget(float amount, bool bAllowReturn) & override {

	}
};

export class TransferOrder : WorkOrder {
	std::atomic<float> requested_;

	const ContainerPtrD source_;
	const ContainerPtrD target_;
	const ContainerPtrD fallback_;

public:
	TransferOrder(ContainerPtrD&& source, ContainerPtrD&& target,
		ContainerPtrD&& fallback, float requested) noexcept(false) :
		source_{ source }, target_{ target }, fallback_{ fallback },
		requested_{ requested } 
	{
		if (requested_ <= 0.f) {
			throw std::invalid_argument{ "requestedAmount must be positive" };
		}
		
		if (auto&& src{ source_.lock() }) {
			ResourceType type{ src->GetType() };

			if (auto&& trgt{ target_.lock() }) {
				if (type != trgt->GetType()) {
					throw std::invalid_argument
						{ "target: containers are not of the same type" };
				}
			}
			else {
				throw std::invalid_argument{ "target is already expired" };
			}

			if (auto&& flbck{ fallback_.lock() }) {
				if (type != flbck->GetType()) {
					throw std::invalid_argument
						{ "fallback: containers are not of the same type" };
				}
			}
		}
		else {
			throw std::invalid_argument{ "source is already expired" };
		}

	}

	float GetAmount() const& {
		return requested_.load(std::memory_order_relaxed);
	}

	virtual float RequestFromSource(float amount) & override {
		using namespace std;
		assert(amount >= 0.f);
		if (auto source{ source_.lock() }) {

			float workLeft{ GetAmount() };
			float amountLeft{ min(amount, workLeft) };

			while (!requested_.compare_exchange_weak(
				workLeft, workLeft - amountLeft)) {
				amountLeft = min(amount, workLeft);
			}

			float acquired{ -1.f * source->ChangeAmount(-1.f * amountLeft) };

			requested_.fetch_add(amountLeft - acquired, memory_order_acq_rel);
			return acquired;
		}
		else {
			return 0.f;
		}
	}

	virtual float UnloadToTarget(float amount, bool bAllowReturn) & override {
		assert(amount >= 0.f);
		float unloaded{ AddTo(target_, amount) };
		if (bAllowReturn) {
			unloaded += AddTo(source_, amount - unloaded);
			unloaded += AddTo(fallback_, amount - unloaded);
		}
		return unloaded;
	}
};