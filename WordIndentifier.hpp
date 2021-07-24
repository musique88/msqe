#pragma once
#include <unordered_map>
#include <string>

namespace MSQE
{
	enum CPP_KEYWORD{
		CPP_NORMAL, CPP_CONTROL, CPP_TYPE, CPP_ACTION, CPP_CONST, CPP_SPECIFIER
	};
	class WordIdentifier
	{
		static WordIdentifier* _instance;
		WordIdentifier();
		std::unordered_map<std::string, int> _cppIdentifiers;
	public:
		static WordIdentifier* getInstance();
		const CPP_KEYWORD idWord(std::string) const;
	};
}