#pragma once

#include <SerialCommands.h>
#include <TaskSchedulerDeclarations.h>

class ICommandProcessor {
public:
	virtual ~ICommandProcessor() = default;

	virtual void createLoopTask() = 0;

	virtual void setStream(Stream *stream) = 0;
};

ICommandProcessor *getCommandProcessor(SerialCommands *serialCommands);

class CommandProcessor : public ICommandProcessor {
public:
	explicit CommandProcessor(SerialCommands *serialCommands);

	void setStream(Stream *stream) override;

	void createLoopTask() override;

private:
	SerialCommands *serialCommands;
	Task *loopTask = nullptr;

	static void doNothing(SerialCommands *sender);

	static void doHelp(SerialCommands *sender);

#ifdef DEVELOPMENT

	static void wifiStaDisconnect(SerialCommands *sender);

	static void wifiStaConnect(SerialCommands *sender);

#endif

	static void showStatus(SerialCommands *sender);

	static void doConf(SerialCommands *sender);

	static void doSave(SerialCommands *sender);
};
