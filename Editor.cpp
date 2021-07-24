#include "Editor.hpp"
#include <fstream>
#include <ncurses.h>
#include <algorithm>
#include <array>
#include <unordered_map>


namespace MSQE
{
	std::unordered_map<std::string, int> cppIdentifiers;

	Editor::Editor()
	:_shouldClose(false), _currentBuffer(0)
	{
		initscr();
		start_color();
		cbreak();
		noecho();
		notimeout(stdscr, false);
		keypad(stdscr, TRUE);
		for(int col = 1; col <= COLOR_WHITE; col++)
			init_pair(col, col, COLOR_BLACK); // initialize all color pairs without any background
	}

	void Editor::userLoop()
	{
		start();
		while(!_shouldClose)
		{
			draw();
			update();
		}
	} 

	void Editor::start()
	{

	}

	void Editor::update()
	{
		int ch = getch();

		if (ch == 27) // macro is escape
			processCommand();
		else
			write(ch);
	}

	void Editor::processCommand()
	{
		int ch = getch();
		switch (ch)
		{
		case 'q':
			_shouldClose = true;
			break;
		case 'w':
			save();
			break;
		default:
			break;
		}
	}

	void Editor::write(int ch)
	{
		Buffer &currentBuffer = _buffers[_currentBuffer];
		switch (ch)
		{
		case KEY_UP:
			currentBuffer.moveCursor(UP);
			break;
		case KEY_DOWN:
			currentBuffer.moveCursor(DOWN);
			break;
		case KEY_RIGHT:
			currentBuffer.moveCursor(RIGHT);
			break;
		case KEY_LEFT:
			currentBuffer.moveCursor(LEFT);
			break;
		case 127:
		case KEY_BACKSPACE:
			currentBuffer.remove(true);
			break;
		case KEY_DC:     
			currentBuffer.remove(false);
			break;
		case 10:
		case KEY_ENTER:
			currentBuffer.enter();
			break;
		case 9:
			currentBuffer.insertChar((char)ch);
			break;
		default:
			if (ch < 127 && ch > 31)
			currentBuffer.insertChar((char)ch);
			break;
		}
	}

	void Editor::open(std::string file)
	{
		Buffer b;
		b.open(file);
		b.processText();
		_buffers.push_back(b);
	}

	void Editor::save()
	{
		_buffers[_currentBuffer].save();
	}

	void Editor::draw()
	{
		Buffer currentBuffer = _buffers[_currentBuffer];
		clear();
		int height = getmaxy(stdscr);
		int currentlyDrawing = std::max(currentBuffer.getRow() - (height / 2), 0);
		move(std::max(height/2 - currentBuffer.getRow(), 0),0);

		for(int i = std::max(height/2 - currentBuffer.getRow(), 0); i < std::max(height, height/2 - currentBuffer.getRow()); i++)
		{
			if (currentlyDrawing < currentBuffer.getLines().size())
			{
				std::vector<ColoredChar> lineToDraw = currentBuffer.getProcessedLines()[currentlyDrawing];
				printw("%u\t", currentlyDrawing);
				for (int j = 0; j < lineToDraw.size(); j++)
				{
					attron(COLOR_PAIR(lineToDraw[j].col));
					addch(lineToDraw[j].c);
					attroff(COLOR_PAIR(lineToDraw[j].col));
				}
				currentlyDrawing++;
				addch('\n');
			}
		}
		int position = currentBuffer.getCol() + 8;
		for(int i = 0; i < currentBuffer.getCol(); i++)
			if(currentBuffer.getLines()[currentBuffer.getRow()][i] == '\t')
				position += TABS - 1;
		move((height / 2), position);
		refresh();
	}

	Editor::~Editor()
	{
		endwin();
	}
}
