#pragma once

#include <string>
#include <vector>
namespace MSQE
{
	enum DIRECTION{LEFT,RIGHT,UP,DOWN};

	class Buffer
	{
		int _row, _col;
		std::string _fileName;
		std::vector<std::string> _lines;
	public:
		Buffer();
		void open(std::string filePath);
		void save();
		int getRow();
		int getCol();
		void moveCursor(int direction);
		void insertChar(char c);
		void remove(bool pre = true);
		void enter();
		std::vector<std::string> getLines();
	};
}