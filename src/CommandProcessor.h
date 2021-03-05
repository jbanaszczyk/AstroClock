#pragma once

#include <SerialCommands.h>
#include <TaskSchedulerDeclarations.h>

class ICommandProcessor {
public:
	virtual ~ICommandProcessor() = default;
	virtual void loop() = 0;
	virtual void addScheduler(Scheduler *scheduler) =0;
	virtual void setStream(Stream *stream) = 0;
};

ICommandProcessor *getCommandProcessor(SerialCommands *serialCommands);

class CommandProcessor : public ICommandProcessor {
public:
	explicit CommandProcessor(SerialCommands *serialCommands);

	void addScheduler(Scheduler *scheduler) override;
	void loop() override;

	void setStream(Stream *stream) override;

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
};
