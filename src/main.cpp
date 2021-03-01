#include <Arduino.h>
#include <SerialCommands.h>

void doHello(SerialCommands *sender) {
	sender->getStream()->println("Ello, ello.");
}

void doMore(SerialCommands *sender) {
	sender->getStream()->println("more");
}

void doLess(SerialCommands *sender) {
	sender->getStream()->println("less");
}

void doNothing(SerialCommands *sender) {
	sender->getStream()->println("nothing");
}

void doHelp(SerialCommands *sender) {
	sender->printHelp();
}

void doSthWithArgs(SerialCommands *sender) {
	Stream *stream = sender->getStream();
	stream->printf("Got command\n");
	decltype(sender->Next()) argument;
	while ((argument = sender->Next()) != nullptr) {
		stream->printf("\targument: %s\n", argument);
	}
	stream->printf("=================\n");
}

void iDontKnowWhatShouldIDo(SerialCommands *sender, const char *cmd) {
	Stream *stream = sender->getStream();
	stream->printf("Got unrecognized command [%s]", cmd);
	auto counter = 0U;
	decltype(sender->Next()) argument;
	while ((argument = sender->Next()) != nullptr) {
		if (counter == 0) {
			stream->printf("\tArguments:");
		}
		stream->printf(" #%d:{%s}", counter, argument);
		++counter;
	}
	stream->printf("\n");
}

static SerialCommands *serialProcessor = nullptr;

void setup() {
	Serial.begin(115200);
	while (!Serial) {}
	Serial.println();

	static SerialCommand staticHelp("?", "Show help", doHelp, true, false);

	serialProcessor = new SerialCommands();

	serialProcessor->setDefaultHandler(iDontKnowWhatShouldIDo);

	serialProcessor->AddCommand(new SerialCommand("x", "Example command with arguments", doSthWithArgs, false, true));
	serialProcessor->AddCommand(new SerialCommand("Hello", nullptr, doHello, false, true));
	serialProcessor->AddCommand("help", "Show help", doHelp);
	serialProcessor->AddCommand(new SerialCommand("+", "Do more", doMore, true, true));
	serialProcessor->AddCommand(new SerialCommand("-", "Do less", doLess, true, true));
	serialProcessor->AddCommand(new SerialCommand("0", nullptr, doNothing, true, true));
	serialProcessor->AddCommand(&staticHelp);

	Serial.printf("Press ? to get help\n");
}

void loop() {
	if (serialProcessor != nullptr) {
		serialProcessor->ReadSerial();
	}
}
