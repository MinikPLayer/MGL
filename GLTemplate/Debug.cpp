#include "Debug.h"


void Log_Recursive(const char* file, int line, LogTypes type, LogLevels level, std::ostringstream& msg)
{
	std::cout << "\033[";
	switch (type)
	{
	case Log:
		std::cout << "37";
		break;
	case Warning:
		std::cout << "33";
		break;
	case Error:
		std::cout << "91";
		break;
	case FatalError:
		std::cout << "31";
		break;
	default:
		std::cout << "[Unknown log type]";
		break;
	}
	switch (level)
	{
	case Engine:
		std::cout << ";2";
		break;
	case User:
		std::cout << ";1";
		break;
	default:
		std::cout << "[Unknown log level]";
		break;
	}

	std::cout << "m";

    std::cout << file << "(" << line << "): " << msg.str() << std::endl;

	if (type == LogTypes::FatalError)
		throw std::exception(msg.str().c_str());
}
