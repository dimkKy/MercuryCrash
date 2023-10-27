export module Game;

import Constructible;

import <list>;
import <string>;
import <iostream>;
import <regex>;
import <vector>;

export class Ship {
	Structure<StructureType::Hull> hull_;
	Structure<StructureType::Reactor> reactor_;

	std::list<Structure<StructureType::SolarPanel>> solarPanels_;
	std::list<Structure<StructureType::Cryochamber>> cryochambers_;
	std::list<Structure<StructureType::Battery>> batteries_;
	std::list<Worker> workers_;
	std::list<WorkOrder> orders_;
};

export class CommandListener {
	std::string buffer_;
	std::vector<std::string> commands_;

	const std::regex delim_{ "\\s+" };
	std::sregex_token_iterator end_;

public:
	void Run(std::istream& stream = std::cin) {
		while (true) {
			std::getline(stream, buffer_);

			commands_ = std::vector<std::string>(
				std::sregex_token_iterator{ buffer_.begin(), buffer_.end(), delim_, -1 },
				end_);


		}
	}

	void operator()() {
		Run();
	}
};

export class TickOperator {

public:
	void Run() {
		while (true) {
			//tick all the workers


		}
	}

	void operator()() {
		Run();
	}
};