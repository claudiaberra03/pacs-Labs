// Compile with:
// g++ -std=c++11 -I /path/to/json ex2.cpp -o ex2
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include <json.hpp>

using json = nlohmann::json;

int main()
{
	std::ifstream input_file("input.json");
	if (!input_file) {
		std::cerr << "Cannot open input.json\n";
		return 1;
	}

	const json input = json::parse(input_file);

	const double t0 = input.at("time").value("t0", 0.0);
	const double dt = input.at("time").value("dt", 0.1);

	auto properties =
	    input.at("properties").get < std::map < std::string,
	    std::vector < double >>>();

	// JSON "properties": { "mass": [ ... ] } becomes
	// properties.at("mass") in C++.
	const std::vector < double >&mass = properties.at("mass");
	std::vector < double >&position = properties.at("position");
	const std::vector < double >&velocity = properties.at("velocity");

	if (mass.size() != position.size()
	    || position.size() != velocity.size()) {
		throw
		    std::runtime_error
		    ("mass, position and velocity must have the same size");
	}

	for (std::size_t i = 0; i < position.size(); ++i) {
		position[i] += velocity[i] * dt;
	}

	json output;
	output["time"] = {
		{"t0", t0},
		{"dt", dt},
		{"t1", t0 + dt},
	};
	output["properties"] = properties;
	
	std::ofstream output_file("output.json");
	if (!output_file) {
		std::cerr << "Cannot open output.json\n";
		return 1;
	}

	output_file << output.dump(2) << '\n';
	std::cout <<
	    "Updated particle positions with x += v * dt and wrote output.json\n";
	return 0;
}
