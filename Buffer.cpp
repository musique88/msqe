#include "Buffer.hpp"
#include <fstream>
#include "WordIndentifier.hpp"


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
		std::string extension = filePath.substr(filePath.find('.'));
		if (extension == ".cpp" || extension == ".hpp")
			_language = CPP;
		else if (extension == "c" || extension == "h")
			_language = C;
		else
			_language = NORMAL;
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
		processLine(_row);
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
				_processedLines[_row - 1].insert(_processedLines[_row - 1].end(), _processedLines[_row].begin(), _processedLines[_row].end());
				_processedLines.erase(_processedLines.begin() + _row);
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
		processLine(_row + 1);
		processLine(_row);
		processLine(_row - 1);
	}
	
	void Buffer::enter()
	{
		if (_row < _lines.size())
		{
			_lines.insert(_lines.begin() + _row + 1, _lines[_row].substr(_col));
			_lines[_row] = _lines[_row].substr(0, _col);
			_processedLines.insert(_processedLines.begin() + _row + 1, std::vector<ColoredChar>(_processedLines[_row].begin() + _col, _processedLines[_row].end()));
			_processedLines[_row].erase(_processedLines[_row].begin() + _col, _processedLines[_row].end());
			processLine(_row);
			processLine(_row + 1);
			_row++;
			_col = 0;
		}
	}

	int Buffer::getLanguage()
	{
		return _language;
	}

	std::vector<std::vector<ColoredChar>> Buffer::getProcessedLines()
	{
		return _processedLines;
	}

	void Buffer::processText()
	{
		_processedLines.resize(_lines.size());
		for (int i = 0; i < _lines.size(); i++)
			processLine(i);
	}

	void Buffer::processLine(int i)
	{
		if(i < 0 || i >= _processedLines.size())
			return;

		_processedLines[i].resize(_lines[i].size());

		for(int j = 0; j < _lines[i].size(); j++)
			_processedLines[i][j] = {_lines[i][j], 0, false};
		
		if (_processedLines[i].size() == 0)
			return;

		if (_processedLines[i][0].c == '#')
		{
			for (int j = 0; j < _processedLines[i].size(); j++)
				_processedLines[i][j].col = 6;
			return;
		}
		int firstOfLowercase = -1;
		for (int j = 0; j < _processedLines[i].size(); j++)
		{
			if (_processedLines[i][j].c >= 'a' && _processedLines[i][j].c <= 'z')
			{
				if (firstOfLowercase == -1)
					firstOfLowercase = j;
			}
			else
			{
				if(firstOfLowercase != -1)
				{
					std::string currentWord = "";
					for (int k = firstOfLowercase; k < j; k++)
						currentWord += _processedLines[i][k].c;
					int id = WordIdentifier::getInstance()->idWord(currentWord);
					for (int k = 0; k < currentWord.size(); k++)
						_processedLines[i][k + firstOfLowercase] = {currentWord[k], id, false};
					firstOfLowercase = -1;
				}

				if (_processedLines[i][j].c == '\t')
				{
					_processedLines[i].erase(_processedLines[i].begin() + j);
					_processedLines[i].insert(_processedLines[i].begin() + j, TABS,{' ', 0, false}); //TODO
				}
			}
		}
		if(firstOfLowercase != -1)
		{
			std::string currentWord = "";
			for (int k = firstOfLowercase; k < _processedLines[i].size(); k++)
				currentWord += _lines[i][k];
			int id = WordIdentifier::getInstance()->idWord(currentWord);
			for (int k = 0; k < currentWord.size(); k++)
				_processedLines[i][k + firstOfLowercase] = {currentWord[k], id, false};
			firstOfLowercase = -1;
		}

	}
}