#pragma once
#include "inputhandler.h"
static inline InputHandler inputhandler;
class App {
public:

	virtual void OnStartup() {};
	virtual void OnClose() {};
	virtual void Run() = 0;
	virtual void MainLoop() = 0;
};