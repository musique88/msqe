#include "Buffer.hpp"
#include <fstream>

namespace MSQE
{
	int Buffer::getCol()
	{
		return _col;
	}

	int Buffer::getRow()
	{
		return _row;
	}
	void Buffer::open(std::string filePath)    
	{
		_fileName = filePath;
		std::ifstream file(filePath);
		if(file.is_open())
		{
			std::string line;
			while(getline(file, line))
				_lines.push_back(line);
			file.close();
		}
	}

	void Buffer::save()
	{
		std::ofstream file(_fileName, std::ofstream::trunc);

		for(int i = 0; i < _lines.size(); i++)
			file << _lines[i] << '\n';

		file.close();
	}

	Buffer::Buffer()
	:_fileName(""), _col(0), _row(0)
	{}

	void Buffer::moveCursor(int direction)
	{
		switch(direction)
		{
		case UP:
			_row--;
			break;
		case DOWN:
			_row++;
			break;
		case LEFT:
			_col--;
			break;
		case RIGHT:
			_col++;
			break;
		default:
			break;
		}
		if(_row < 0)
			_row = 0;
		else if(_row >= _lines.size())
			_row = _lines.size() - 1;
		if(_col < 0)
			_col = 0;
		else if(_col > _lines[_row].size())
			_col = _lines[_row].size();
	}

	std::vector<std::string> Buffer::getLines()
	{
		return _lines;
	}

	void Buffer::insertChar(char c)
	{
		_lines[_row].insert(_lines[_row].begin() + _col,c);
		_col++;
	}

	void Buffer::remove(bool pre)
	{
		if(pre)
		{
			
			if (_col > 0)
			{
				_lines[_row].erase(_lines[_row].begin() + _col - 1);
				_col--;
			}
			else if (_row > 0)
			{
				_col = _lines[_row - 1].size();
				_lines[_row - 1] += _lines[_row];
				_lines.erase(_lines.begin() + _row);
				_row--;
			}
		}
		else
		{
			if (_col == _lines[_row].size())
			{
				if (_row < _lines.size())
				_lines[_row]+=_lines[_row + 1];
				_lines.erase(_lines.begin() + _row + 1);
			}
			else
				_lines[_row].erase(_lines[_row].begin() + _col);
		}
	}
	
	void Buffer::enter()
	{
		if (_row < _lines.size())
		{
			std::string old = _lines[_row].substr(_col);
			_lines.insert(_lines.begin() + _row + 1, old);
			_lines[_row] = _lines[_row].substr(0, _col);
			_row++;
			_col = 0;
		}
	}
}