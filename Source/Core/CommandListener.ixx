// by Dmitry Kolontay

export module CommandListener;

import Ship;

import <iostream>;
import <regex>;
import <string_view>;
import <unordered_map>;

#define DEFINE_COMMAND(_name_) constexpr std::string_view _name_{ #_name_ };

namespace Commands {
	using namespace std;

	constexpr string_view build{ "build" };
	constexpr string_view fix{ "fix" };
	constexpr string_view mine{ "mine" };
	constexpr string_view assign{ "assign" };
	constexpr string_view abort{ "abort" };
	constexpr string_view dismantle{ "dismantle" };

	const regex workerPattern{ "^w[0-9]+$" };
	const regex orderPattern{ "^o[0-9]+$" };

	int WorkerID(const string& arg) {
		return stoi(arg.substr(1));
	}
	bool IsWorkerID(const string& arg) {
		return regex_match(arg, orderPattern);
	}

	int OrderID(const string& arg) {
		return stoi(arg.substr(1));
	}
	bool IsOrderID(const string& arg) {
		return regex_match(arg, orderPattern);
	}
};

namespace InputMsgs {

};

using ProcessFunctionPtr = void(*)(const std::vector<std::string>&);
using ArgIter = std::vector<std::string>::const_iterator;

export class CommandListener {
	Ship& ship_;

	std::string buffer_;
	std::vector<std::string> args_;

	const std::regex delim_{ "\\s+" };
	const std::sregex_token_iterator end_;

	static std::unordered_map<
		std::string_view, ProcessFunctionPtr> commandsMap_;

	std::vector<int> static ParseForWorkers(ArgIter begin, ArgIter end) {
		using namespace Commands;

		std::vector<int> workerIDs;
		workerIDs.reserve(std::distance(begin, end));

		for (; begin != end; ++begin) {
			if (IsWorkerID(*begin)) {
				workerIDs.push_back(WorkerID(*begin));
			}
		}
		return workerIDs;
	}

	std::vector<int> static ParseForOrders(ArgIter begin, ArgIter end) {
		using namespace Commands;

		std::vector<int> orderIDs;
		orderIDs.reserve(std::distance(begin, end));

		for (; begin != end; ++begin) {
			if (IsOrderID(*begin)) {
				orderIDs.push_back(OrderID(*begin));
			}
		}

		return orderIDs;
	}

	static void buildCommand(const std::vector<std::string>& args) {
		//offset for build info?
		std::vector<int> workerIDs{ ParseForWorkers(args.begin() + 2, args.end())};

	}

	static void fixCommand(const std::vector<std::string>& args) {
		using namespace Commands;

		if (args.size() > 2 && IsWorkerID(args[1]) && IsWorkerID(args[2])) {
			int w1{ WorkerID(args[1]) };
			int w2{ WorkerID(args[2]) };
			if (w1 != w2) {
				//w1 to fix 2;
			}
		}
	}

	static void mineCommand(const std::vector<std::string>& args) {
		std::vector<int> workerIDs{ ParseForWorkers(args.begin(), args.end()) };
		//do smthing with ids
	}

	static void assignCommand(const std::vector<std::string>& args) {

	}

	static void abortCommand(const std::vector<std::string>& args) {
		std::vector<int> workerIDs{ ParseForWorkers(args.begin(), args.end()) };
		std::vector<int> orderIDs{ ParseForOrders(args.begin(), args.end()) };
		//for each delete
	}

	static void dismantleCommand(const std::vector<std::string>& args) {

	}

public:
	CommandListener(Ship& ship) :
		ship_{ ship } {}

	void Run(std::istream& iStream = std::cin) {
		while (true) {
			std::getline(iStream, buffer_);
			args_ = { {buffer_.begin(), buffer_.end(), delim_, -1 }, end_ };

			if (auto&& cmnd{ commandsMap_.find(args_[0]) }; cmnd != commandsMap_.end()) {
				cmnd->second(args_);
			}
			else {
				//not found
			}

		}
	}

	void operator()() {
		Run();
	}
};

#define PAIR_PROCESS_FUNCTION(_name_) {Commands::_name_, &CommandListener::_name_##Command}

std::unordered_map<std::string_view, ProcessFunctionPtr> CommandListener::commandsMap_{
	PAIR_PROCESS_FUNCTION(build),
	PAIR_PROCESS_FUNCTION(fix),
	PAIR_PROCESS_FUNCTION(mine),
	PAIR_PROCESS_FUNCTION(assign),
	PAIR_PROCESS_FUNCTION(abort),
	PAIR_PROCESS_FUNCTION(dismantle),
};