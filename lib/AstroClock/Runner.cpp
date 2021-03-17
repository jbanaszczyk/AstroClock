#include "Runner.h"

#include <TaskScheduler.h>

Runner::Runner(Scheduler *scheduler)
		: scheduler(scheduler) {
	scheduler->init();
}

Scheduler *Runner::getScheduler() {
	return scheduler;
}

IRunner *getRunner() {
	static Scheduler scheduler;
	static Runner runner = Runner(&scheduler);
	return &runner;
}
