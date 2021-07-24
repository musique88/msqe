#include "WordIndentifier.hpp"

namespace MSQE
{
	WordIdentifier* WordIdentifier::_instance = nullptr;

	WordIdentifier* WordIdentifier::getInstance()
	{
		if (_instance == nullptr)
			_instance = new WordIdentifier();
		return _instance;
	}

	WordIdentifier::WordIdentifier()
	{
			std::array<std::string, 30> cppActions = {	"alignas", "alignof", "and", "and_eq", "asm", "bitand", "bitor", "const_cast", "co_await", "co_return", "co_yield",
														"decltype", "delete", "dynamic_cast", "new", "not", "not_eq", "or", "or_eq", "reinterpret_cast", "requires",
														"sizeof", "static_assert", "static_cast", "typeid", "using", "xor", "xor_eq"};
			std::array<std::string, 20> cppControls = {	"break", "case", "catch", "continue", "default", "do", "else", "for", "goto", "if", "return", "switch", "throw",
														"try", "while"};
			std::array<std::string, 20> cppTypes = {	"auto", "bool", "char", "char8_t", "char16_t", "char32_t", "class", "concept", "double", "enum", "int", "long", 
														"namespace", "short", "struct", "typedef", "typename", "union", "void", "wchat_t"};
			std::array<std::string, 10> cppConst = {	"false", "nullptr", "this", "true"};
			std::array<std::string, 30> cppSpecifier = {"const", "consteval", "constexpr", "constinit", "explicit", "export", "extern", "friend", "inline", "mutable",
														"noexept", "operator", "private", "protected", "public", "register", "signed", "static", "template", "thread_local",
														"unsigned", "virtual", "volatile", "final", "override"};
		for(std::string s : cppActions)
			_cppIdentifiers.emplace(s, CPP_ACTION);
		for(std::string s : cppControls)
			_cppIdentifiers.emplace(s, CPP_CONTROL);
		for(std::string s : cppTypes)
			_cppIdentifiers.emplace(s, CPP_TYPE);
		for(std::string s : cppConst)
			_cppIdentifiers.emplace(s, CPP_CONST);
		for(std::string s : cppSpecifier)
			_cppIdentifiers.emplace(s, CPP_SPECIFIER);
	}

	const CPP_KEYWORD WordIdentifier::idWord(std::string str) const
	{
		std::unordered_map<std::string, int>::const_iterator got = _cppIdentifiers.find(str);

		if(got == _cppIdentifiers.end())
			return CPP_NORMAL;

		return (CPP_KEYWORD)got->second;
	}
}

