#pragma once

#include <SerialCommands.h>
#include <TaskSchedulerDeclarations.h>

class ICommandProcessor {
public:
	virtual ~ICommandProcessor() = default;

	virtual void addScheduler(Scheduler *scheduler) = 0;

	virtual void setStream(Stream *stream) = 0;
};

ICommandProcessor *getCommandProcessor(SerialCommands *serialCommands);

class CommandProcessor : public ICommandProcessor {
public:
	unsigned long SerialPortReadRepeatRate = 33;

	explicit CommandProcessor(SerialCommands *serialCommands);

	void addScheduler(Scheduler *scheduler) override;

	void setStream(Stream *stream) override;

private:
	SerialCommands *serialCommands;
	Task *loopTask = nullptr;

	static void doNothing(SerialCommands *sender);

	static void doHelp(SerialCommands *sender);

	static void wifiStaForget(SerialCommands *sender);

	static void wifiStaConnect(SerialCommands *sender);

	static void showStatus(SerialCommands *sender);

	static void wifiStaConnect_wrong(SerialCommands *sender);
};
