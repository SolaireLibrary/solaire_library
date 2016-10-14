#include <iostream>
#include <fstream>
#include "value.hpp"
#include "binary.hpp"
#include "value_to_parser.hpp"
#include "encoder.hpp"
#include "dispatcher.hpp"

int main() {
	{
		using namespace solaire;

		serial::function_map_dispatcher disp;
		disp.register_function<float, float, float>("add", [](float a, float b)->float { return a + b; });
		disp.register_function<float, float, float>("sub", [](float a, float b)->float { return a - b; });
		disp.register_function<float, float, float>("mul", [](float a, float b)->float { return a * b; });
		disp.register_function<float, float, float>("div", [](float a, float b)->float { return a / b; });

		std::cout << disp.call<float, float, float>("add", 2, 3) << std::endl;
		std::cout << disp.call<float, float, float>("sub", 2, 3) << std::endl;
		std::cout << disp.call<float, float, float>("mul", 2, 3) << std::endl;
		std::cout << disp.call<float, float, float>("div", 2, 3) << std::endl;
	}

	//{
	//	value v(value::TYPE_ARRAY);
	//	v.get_array().push_back(value("this is the first string"));
	//	v.get_array().push_back(value("this is the second string"));
	//	v.get_array().push_back(value("this is the third string"));
	//	v.get_array().push_back(value("this is the fourth string"));

	//	value_to_binary bin;
	//	value_to_parser(v, bin);

	//	std::ofstream f("test.bin");
	//	f.write(static_cast<const char*>(bin.get()), bin.size());
	//	f.close();
	//}

	//{
	//	std::ifstream f("test.bin");
	//	std::vector<uint8_t> data;
	//	while(!f.eof()) {
	//		uint8_t tmp;
	//		f.read(reinterpret_cast<char*>(&tmp), 1);
	//		data.push_back(tmp);
	//	}

	//	parser_to_value parser;
	//	binary_to_value(&data[0], data.size(), parser);
	//	const value& v = parser.get();

	//	std::cout << (const char*) v[0] << std::endl;
	//	std::cout << (const char*) v[1] << std::endl;
	//	std::cout << (const char*) v[2] << std::endl;
	//	std::cout << (const char*) v[3] << std::endl;
	//}
	system("pause");
	return 0;
}