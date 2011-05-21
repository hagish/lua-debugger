/*
 * The MIT License - see LICENSE file for details
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <list>

#include "DebugThread.h"
#include "MainFrame.h"

#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/Net/ServerSocket.h>

class Server: public Poco::Runnable
{
public:
	Server(unsigned int port, MainFrame *mainFrame);
	~Server();

	virtual void run();
	void stop();
	void start();

private:
	const unsigned int port;
	bool running;

	class ClientThread
	{
		friend class Server;

		Poco::Thread *thread;
		DebugThread *runnable;

		ClientThread(Poco::Thread *thread, DebugThread *runnable) :
			thread(thread), runnable(runnable)
		{
		}
		;
	};

	std::list<ClientThread *> clients;
	MainFrame *mainFrame;
	Poco::Thread *thread;
};

#endif /* SERVER_H_ */
