/*
 * The MIT License - see LICENSE file for details
 */

#ifndef _DEBUG_CONSOLE_THREAD_H_
#define _DEBUG_CONSOLE_THREAD_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <queue>

#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/Mutex.h>

#include "DebugPanel.h"

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/StreamCopier.h>

class DebugThread: public Poco::Runnable
{
public:
	DebugThread(Poco::Net::StreamSocket client);

	void stop();

	void print(const std::string &output);
	void postline(const std::string &line);
	bool hasBreakpointAt(const std::string &file, unsigned int line);
	std::vector<unsigned int> listBreakpointOfFile(const std::string &file);

	void assignDebugPanel(DebugPanel *panel);

	void notifyFileAndLine(const std::string &file, unsigned int line);

protected:
	virtual void run();

private:
	std::string readline();

	void send(const std::string &s);
	std::string receive();
	std::string receive(unsigned int len);

	std::map<std::string, std::map<std::string, bool> > breakpoints;
	std::map<std::string, std::string> watches;

	DebugPanel *panel;
	Poco::Net::SocketStream client;
	bool running;

	std::queue<std::string> lines;
	Poco::Mutex mutex;
};

#endif
