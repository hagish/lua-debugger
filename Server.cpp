/*
 * The MIT License - see LICENSE file for details
 */

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/SocketAddress.h"

#include "Server.h"

#include <boost/foreach.hpp>

Server::Server(unsigned int port, MainFrame *mainFrame) :
	port(port), mainFrame(mainFrame), thread(NULL)
{

}

Server::~Server()
{
	if (thread != NULL)
	{
		stop();
	}
}

void Server::start()
{
	assert (thread == NULL);

	thread = new Poco::Thread();
	thread->start(*this);
}

void Server::stop()
{
	assert (thread != NULL);

	running = false;

	thread->join();

	delete thread;
	thread = NULL;
}

void Server::run()
{
	Poco::Net::ServerSocket socket(port);

	running = true;

	while (running)
	{
		if (socket.poll(Poco::Timespan(1, 0),
				Poco::Net::ServerSocket::SELECT_READ))

		{
			Poco::Net::StreamSocket ss = socket.acceptConnection();

			DebugThread *runnable = new DebugThread(ss);

			ClientThread *c = new ClientThread(new Poco::Thread(), runnable);
			clients.push_back(c);

			wxCommandEvent event(wxEVT_MY_EVENT_ADD_DEBUG_THREAD);
			event.SetClientData((void *) runnable);
			wxPostEvent(mainFrame, event);

			c->thread->start(*(c->runnable));
		}
	}

	BOOST_FOREACH(ClientThread *c, clients)
				{
					c->runnable->stop();
					c->thread->join();

					delete c->runnable;
					delete c->thread;

					delete c;
				}
}
