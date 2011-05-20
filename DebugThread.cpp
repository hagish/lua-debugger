/*
 * The MIT License - see LICENSE file for details
 *
 * based on RemDebug 1.0 Beta
 * Copyright Kepler Project 2005 (http://www.keplerproject.org/remdebug) 
 */

#include <iostream>

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <wx/wx.h>

#include "DebugThread.h"
#include "StringUtils.h"

DebugThread::DebugThread(Poco::Net::StreamSocket client) :
	panel(NULL), client(client)
{
	// without timeout formated socket reading hangs
	client.setReceiveTimeout(Poco::Timespan(50, 0));
	client.setReceiveBufferSize(1024);
	client.setSendTimeout(Poco::Timespan(50, 0));
	client.setSendBufferSize(1024);
}

void DebugThread::assignDebugPanel(DebugPanel *panel)
{
	assert(panel != NULL);

	this->panel = panel;
}

void DebugThread::notifyFileAndLine(const std::string &file, unsigned int line)
{
	assert(panel != NULL);

	wxCommandEvent event(wxEVT_MY_EVENT_NOTIFY_FILE_AND_LINE);
	event.SetString(wxString(file.c_str(), wxConvUTF8));
	event.SetInt(line);
	wxPostEvent(panel, event);
}

void DebugThread::print(const std::string &output)
{
	assert(panel != NULL);

	std::cout << output << std::endl;

	wxCommandEvent event(wxEVT_MY_EVENT_PRINT_LINE);
	event.SetString(wxString(output.c_str(), wxConvUTF8));
	wxPostEvent(panel, event);
}

void DebugThread::stop()
{
	running = false;
}

void DebugThread::send(const std::string &s)
{
	//	std::cout << "DEBUG SEND: '" << s << "'" << std::endl;
	client << s << std::flush;
}

std::string DebugThread::receive()
{
	std::string buffer;
	getline(client, buffer);
	//	std::cout << "DEBUG RECV: '" << buffer << "'" << std::endl;
	return buffer;
}

std::vector<unsigned int> DebugThread::listBreakpointOfFile(
		const std::string &file)
{
	Poco::Mutex::ScopedLock lock(mutex);

	// TODO basedir compatible?

	std::vector<unsigned int> lineNumbers;

	for (std::map<std::string, bool>::iterator it = breakpoints[file].begin(); it
			!= breakpoints[file].end(); ++it)
	{
		if (it->second)
		{
			lineNumbers.push_back(StringUtils::toUInt(it->first));
		}
	}

	return lineNumbers;
}

bool DebugThread::hasBreakpointAt(const std::string &file, unsigned int line)
{
	Poco::Mutex::ScopedLock lock(mutex);

	// TODO basedir compatible?

	return breakpoints[file][(boost::format("%1%") % line).str()];
}

std::string DebugThread::receive(unsigned int len)
{
	std::string buffer;

	char chunk[512];

	while (buffer.size() < len)
	{
		int remaining = std::min(len - buffer.size(), (unsigned int) 512);
		client.read((char *) &chunk, remaining);
		buffer += std::string((const char *) &chunk, (size_t) remaining);
	}

	//	std::cout << "DEBUG RECV(" << len << "): '" << buffer << "'" << std::endl;

	return buffer;
}

void DebugThread::postline(const std::string &line)
{
	Poco::Mutex::ScopedLock lock(mutex);

	lines.push(line);
}

/**
 * blocks until someone posts a line
 */
std::string DebugThread::readline()
{
	while (running)
	{
		{
			Poco::Mutex::ScopedLock lock(mutex);

			if (lines.size() > 0)
			{
				std::string line = lines.front();
				lines.pop();
				return line;
			}
		}

		Poco::Thread::sleep(10);
	}

	return std::string();
}

void DebugThread::run()
{
	try
	{
		running = true;

		static const boost::regex pattern_spaces("^([[:blank:]]*)$");
		static const boost::regex pattern_number("^([0-9]+)");
		static const boost::regex pattern_line202(
				"^202 Paused\\s+(.+)[[:blank:]]+([0-9]+)$");
		static const boost::regex pattern_line203(
				"^203 Paused\\s+(.+)[[:blank:]]+([0-9]+)[[:blank:]]+([0-9]+)$");
		static const boost::regex pattern_line401(
				"^401 Error in Execution ([0-9]+)$");
		static const boost::regex pattern_command("^([a-z]+)");
		static const boost::regex pattern_file_line(
				"^[a-z]+[[:blank:]]+(.+)[[:blank:]]+([0-9]+)$");
		static const boost::regex pattern_param("^[a-z]+[[:blank:]]+(.+)$");
		static const boost::regex pattern_200ok("^200 OK ([0-9]+)$");
		static const boost::regex pattern_exec_result(
				"^([0-9]+)[a-zA-Z ]+([0-9]+)$");

		// wait for panel to connect
		while (panel == NULL)
		{
			Poco::Thread::sleep(10);
		}

		send("STEP\n");
		receive();

		std::string breakpoint = receive();

		boost::match_results<std::string::const_iterator> what;

		if (boost::regex_search(breakpoint, what, pattern_line202))
		{
			std::string file = what[1].str();
			std::string line = what[2].str();

			print("Paused at file " + file + " line " + line);
			print("Type 'help' for commands");

			notifyFileAndLine(file, StringUtils::toUInt(line));
		}
		else if (boost::regex_search(breakpoint, what, pattern_line401))
		{
			unsigned int size = StringUtils::toUInt(what[1].str());

			print("Error in remote application: ");
			print(receive(size));
			running = false;
		}

		std::string basedir = "";

		while (running)
		{
//			std::cout << "please enter command: ";
			std::string line = readline();

			if (boost::regex_search(line, what, pattern_command))
			{
				std::string command = what[1].str();

				if ((command == "run") || (command == "step") || (command
						== "over"))
				{
					send(StringUtils::toUpper(command) + "\n");
					receive();
					std::string breakpoint = receive();
					if (breakpoint.size() == 0)
					{
						print("Program finished");
						running = false;
					}

					if (boost::regex_search(breakpoint, what, pattern_number))
					{
						std::string status = what[1].str();

						if (status == "202")
						{
							if (boost::regex_search(breakpoint, what,
									pattern_line202))
							{
								std::string file = what[1].str();
								std::string line = what[2].str();

								print(
										"Paused at file " + file + " line "
												+ line);

								notifyFileAndLine(file,
										StringUtils::toUInt(line));
							}
						}
						else if (status == "203")
						{
							if (boost::regex_search(breakpoint, what,
									pattern_line203))
							{
								std::string file = what[1].str();
								std::string line = what[2].str();
								std::string watch_idx = what[3].str();

								print(
										"Paused at file " + file + " line "
												+ line + " (watch expression "
												+ watch_idx + ": ["
												+ watches[watch_idx] + "])");

								notifyFileAndLine(file,
										StringUtils::toUInt(line));
							}
						}
						else if (status == "401")
						{
							if (boost::regex_search(breakpoint, what,
									pattern_line401))
							{
								unsigned int size = StringUtils::toUInt(
										what[1].str());

								print("Error in remote application: ");
								print(receive(size));
								running = false;
							}
						}
						else
						{
							print("Unknown error");
							running = false;
						}
					}
				}
				else if (command == "exit")
				{
					client.close();
					running = false;
				}
				else if (command == "setb")
				{
					if (boost::regex_search(line, what, pattern_file_line))
					{
						std::string filename = basedir + what[1].str();
						std::string line = what[2].str();

						send("SETB " + filename + " " + line + "\n");
						if (receive() == "200 OK")
						{
							Poco::Mutex::ScopedLock lock(mutex);
							breakpoints[filename][line] = true;
						}
						else
						{
							print("Error: breakpoint not inserted");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "setw")
				{
					if (boost::regex_search(line, what, pattern_param))
					{
						std::string exp = what[1].str();
						send("SETW " + exp + "\n");
						std::string answer = receive();

						if (boost::regex_search(answer, what, pattern_200ok))
						{
							std::string watch_idx = what[1].str();
							watches[watch_idx] = exp;
							print("Inserted watch exp no. " + watch_idx);
						}
						else
						{
							print("Error: Watch expression not inserted");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "delb")
				{
					if (boost::regex_search(line, what, pattern_file_line))
					{
						std::string filename = basedir + what[1].str();
						std::string line = what[2].str();

						send("DELB " + filename + " " + line + "\n");
						if (receive() == "200 OK")
						{
							Poco::Mutex::ScopedLock lock(mutex);
							breakpoints[filename].erase(line);
						}
						else
						{
							print("Error: breakpoint not inserted");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "delallb")
				{
					Poco::Mutex::ScopedLock lock(mutex);

					for (std::map<std::string, std::map<std::string, bool> >::iterator
							it_file = breakpoints.begin(); it_file
							!= breakpoints.end(); ++it_file)
					{
						for (std::map<std::string, bool>::iterator it_line =
								it_file->second.begin(); it_line
								!= it_file->second.end(); ++it_line)
						{
							send(
									"DELB " + it_file->first + " "
											+ it_line->first + "\n");
							if (receive() == "200 OK")
							{
								breakpoints[it_file->first].erase(it_line++);
							}
							else
							{
								print("Error: breakpoint not inserted");
							}
						}

						if (it_file->second.size() == 0)
						{
							breakpoints.erase(it_file++);
						}
					}
				}
				else if (command == "delw")
				{
					if (boost::regex_search(line, what, pattern_param))
					{
						std::string index = what[1].str();
						send("DELW " + index + "\n");

						if (receive() == "200 OK")
						{
							watches.erase(index);
						}
						else
						{
							print("Error: watch expression not removed");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "delallw")
				{
					for (std::map<std::string, std::string>::iterator it =
							watches.begin(); it != watches.end(); ++it)
					{
						send("DELW " + it->first + "\n");

						if (receive() == "200 OK")
						{
							watches.erase(++it);
						}
						else
						{
							print(
									"Error: watch expression at index "
											+ it->first + " [" + it->second
											+ "] not removed");
						}
					}
				}
				else if (command == "eval")
				{
					if (boost::regex_search(line, what, pattern_param))
					{
						std::string exp = what[1].str();
						send("EXEC return (" + exp + ")\n");
						std::string line = receive();

						if (boost::regex_search(line, what, pattern_exec_result))
						{
							std::string status = what[1].str();
							std::string len = what[2].str();
							if (status == "200")
							{
								print( receive(StringUtils::toUInt( len)));
							}
							else if (status == "401")
							{
								print("Error in expression:");
								print( receive(StringUtils::toUInt( len)));
							}
						}
						else
						{
							print("Unknown error");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "exec")
				{
					if (boost::regex_search(line, what, pattern_param))
					{
						std::string exp = what[1].str();
						send("EXEC " + exp + "\n");
						std::string line = receive();

						if (boost::regex_search(line, what, pattern_exec_result))
						{
							std::string status = what[1].str();
							std::string len = what[2].str();
							if (status == "200")
							{
								print( receive(StringUtils::toUInt( len)));
							}
							else if (status == "401")
							{
								print("Error in expression:");
								print( receive(StringUtils::toUInt( len)));
							}
						}
						else
						{
							print("Unknown error");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "file")
				{
					if (boost::regex_search(line, what, pattern_param))
					{
						std::string file = basedir + what[1].str();
						send("FILE " + file + "\n");
						std::string line = receive();

						if (boost::regex_search(line, what, pattern_exec_result))
						{
							std::string status = what[1].str();
							std::string len = what[2].str();
							if (status == "200")
							{
								std::string src = receive(
										StringUtils::toUInt(len));
								print(src);
							}
							else if (status == "400")
							{
								print("Could not find file");
							}
						}
						else
						{
							print("Unknown error");
						}
					}
					else
					{
						print("Invalid command");
					}
				}
				else if (command == "listb")
				{
					Poco::Mutex::ScopedLock lock(mutex);

					for (std::map<std::string, std::map<std::string, bool> >::iterator
							it_file = breakpoints.begin(); it_file
							!= breakpoints.end(); ++it_file)
					{
						for (std::map<std::string, bool>::iterator it_line =
								it_file->second.begin(); it_line
								!= it_file->second.end(); ++it_line)
						{
							print(it_file->first + ":" + it_line->first);
						}
					}
				}
				else if (command == "listw")
				{
					for (std::map<std::string, std::string>::iterator it =
							watches.begin(); it != watches.end(); ++it)
					{
						print("Watch exp. " + it->first + ": " + it->second);
					}
				}
				else if (command == "basedir")
				{
					if (boost::regex_search(line, what, pattern_param))
					{
						std::string dir = what[1].str();
						basedir = dir;
						print("New base directory is: '" + basedir + "'");
					}
					else
					{
						print("Current basedir is: '" + basedir + "'");
					}
				}
				else if (command == "help")
				{
					print("setb <file> <line>    -- sets a breakpoint");
					print("delb <file> <line>    -- removes a breakpoint");
					print("delallb               -- removes all breakpoints");
					print(
							"setw <exp>            -- adds a new watch expression");
					print(
							"delw <index>          -- removes the watch expression at index");
					print(
							"delallw               -- removes all watch expressions");
					print("run                   -- run until next breakpoint");
					print(
							"step                  -- run until next line, stepping into function calls");
					print(
							"over                  -- run until next line, stepping over function calls");
					print("listb                 -- lists breakpoints");
					print("listw                 -- lists watch expressions");
					print(
							"eval <exp>            -- evaluates expression on the current context and returns its value");
					print(
							"exec <stmt>           -- executes statement on the current context");
					print(
							"basedir [<path>]      -- sets the base path of the remote application, or shows the current one");
					print("file <file>           -- receives file content");
					print("exit                  -- exits debugger");

				}

				// breakpoint update needed?
				if (command == "delb" || command == "setb" || command
						== "delallb")
				{
					wxCommandEvent event(
							wxEVT_MY_EVENT_NOTIFY_BREAKPOINT_UPDATE);
					wxPostEvent(panel, event);
				}
			}
			else
			{
				if (!boost::regex_search(line, what, pattern_spaces))
				{
					print("Invalid command");
				}
			}
		}
	} catch (std::exception &e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
}

