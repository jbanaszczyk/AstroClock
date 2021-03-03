#pragma once

#include <SerialCommands.h>

class ICommandProcessor {
public:
	virtual ~ICommandProcessor() = default;

	virtual void loop() = 0;

	virtual void setStream(Stream *stream) =0;

};

class CommandProcessor : public ICommandProcessor {
public:
	explicit CommandProcessor(SerialCommands *serialCommands);

	void loop() override;

	void setStream(Stream *stream) override;

private:

	SerialCommands *serialCommands;

	static void doHelp(SerialCommands *sender);

#ifdef DEVELOPMENT
	static void wifiStaDisconnect(SerialCommands *sender);

	static void wifiStaConnect(SerialCommands *sender);
#endif

	static void showStatus(SerialCommands *sender);
};

ICommandProcessor *getCommandProcessor(SerialCommands *serialCommands);
