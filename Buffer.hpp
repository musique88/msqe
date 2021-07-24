#pragma once

#include <string>
#include <vector>
#define TABS 4
namespace MSQE
{
	struct ColoredChar
	{
		char c;
		int col;
		bool bold;
	};

	enum DIRECTION{LEFT,RIGHT,UP,DOWN};
	enum LANGUAGE{NORMAL, CPP, C};
	class Buffer
	{
		int _row, _col;
		std::string _fileName;
		std::vector<std::string> _lines;
		std::vector<std::vector<ColoredChar>> _processedLines;
		LANGUAGE _language;
	public:
		Buffer();
		void open(std::string filePath);
		int getLanguage();
		void save();
		int getRow();
		void processText();
		void processLine(int);
		int getCol();
		void moveCursor(int direction);
		void insertChar(char c);
		void remove(bool pre = true);
		void enter();
		std::vector<std::string> getLines();
		std::vector<std::vector<ColoredChar>> getProcessedLines();
	};
}