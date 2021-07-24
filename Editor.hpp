#pragma once
#include <vector>
#include <string>
#include "Buffer.hpp"

namespace MSQE
{
	class Editor
	{
		std::vector<Buffer> _buffers;
		bool _shouldClose;
		bool _macroPressed;
		int _currentBuffer;
	public:
		Editor();
		~Editor();
		void open(std::string file);
		void userLoop();
		void start();
		void save();
		void update();
		void draw();
		void processCommand();
		void write(int ch);
		void moveCursor(int direction);
	};
}