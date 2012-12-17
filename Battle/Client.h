#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "SDL/SDL_net.h"
#include <string>
#include <map>
#include <memory>

#include "CommandProcessor.h"

#include "util/LagMeasure.hpp"

class CommandPing;
class CommandPong;
class CommandSetCharacter;
class CommandSetPlayerData;
class CommandSetClientData;
class Server;

class Client : public CommandProcessor
{
public:

	enum State
	{
		CONNECTING = 1,
		CHARACTER_REQUESTED = 2,
		CHARACTER_INITIALIZED = 3,
		ACTIVE = 4,
	};


	Client(int client_id, TCPsocket socket, Server * const server);
	Client & operator=(Client&& other);

	bool process(std::unique_ptr<Command> command);
	bool process(CommandPing *command);
	bool process(CommandPong *command);
	bool process(CommandSetCharacter *command);
	bool process(CommandSetPlayerData *command);

	// accessors
	TCPsocket socket() { return socket_; }
	int id() { return client_id_; }
	int getClientId() { return client_id_; }
	Client::State getState() { return currentState_; }
	void setState(Client::State state) { currentState_ = state; }
	char getCharacter() { return character_; }
	LagMeasure &lag() { return lag_; }

	// communication
	void send(Command &command);

	void cleanup(); 
	
	// lag
	Uint32 getLastLagTime() { return lastLagTime_; }
	void setLastLagTime(Uint32 time) { lastLagTime_ = time; }
	int getInitialLagTests() { return initialLagTests_; }
	void setInitialLagTests(int v) { initialLagTests_ = v; }

	short test;

private:
	
	friend std::map<int, Client>;
	Client();
	
	int client_id_;
	char character_;

	Server * server_;

	LagMeasure lag_;
	Uint32 lastLagTime_;
	int initialLagTests_;

	Client::State currentState_;
};

#endif //__CLIENT_H__