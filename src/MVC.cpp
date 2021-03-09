#include "MVC.h"

#include "Runner.h"
#include <NTPSync.h>
#include <ConfigManager.h>
#include "utilities.h"
#include <sntp.h>

void initEngine() {

	static Model aModel{};
	static Controller aController(aModel);
	static DashboardView dashboardView(aModel, aController);

	auto runner = getRunner();
	aController.addScheduler(runner->getScheduler());
}

void Controller::addScheduler(Scheduler *scheduler) {
	if (scheduler != nullptr) {
		Controller::scheduler = scheduler;
		tRefresh = new Task(
				RefreshInterval,
				-1,
				[this]() -> void {
					model.setPulse();
				},
				scheduler,
				true);
		tSetNow = new Task(
				SampleTimeInterval,
				-1,
				[this]() -> void {
					model.setNow(getNTPSync()->getNow());
				},
				scheduler,
				true);
		tShowTimeOnSync = new Task(
				0,
				1,
				[this]() -> void {
					auto syncTime = getNTPSync()->getNow();
					model.setNow(syncTime);
					model.setFirstSync(syncTime);
					model.setLastSync(syncTime);
				},
				scheduler,
				true);
		tConfigChanged = new Task(
				0,
				1,
				[this]() -> void {
					Serial.printf("________ Config written\n");
					auto newConfig = getConfigManager()->getEepromData().getStoredData().getConfigData();
					model.setTimeZone(newConfig.timeZone);

					if (esp8266::coreVersionNumeric() >= 20700000) {
						Serial.printf("___ path 1\n");
						configTime(newConfig.timeZone, "pool.ntp.org");
					} else {
						Serial.printf("___ path 2\n");
						setenv("TZ", newConfig.timeZone, 1);
						configTime(0, 0, "pool.ntp.org");
					}

					setTZ(configDefaults.timeZone);
				},
				scheduler,
				true);
		getNTPSync()->addSyncCallback([this]() -> void {
			tShowTimeOnSync->restart();
		});
		getConfigManager()->setConfigSaveCallback([this]() -> void {
			tConfigChanged->restart();
		});

	}
}

DashboardView::DashboardView(Model &model, Controller &controller) : View(model, controller) {

	strcpy_P_s(&(dashboardOutput->restartReason[0]), getLastRestartReason_P(), sizeof(dashboardInput.restartReason));
	dashboardOutput->initFreeMem = initFreeHeapSize;

	model.observeNow([this](const String &name, const time_t &value) -> void {
		auto timeNow = ntpSync->timeStr(value);
		strcpy_s(&(dashboardOutput->currentTime[0]), timeNow.get(), sizeof(dashboardInput.currentTime));
	});

	model.observeFirstSync([this](const String &name, const time_t &value) -> void {
		auto timeNow = ntpSync->timeStr(value);
		strcpy_s(&(dashboardOutput->startupTime[0]), timeNow.get(), sizeof(dashboardInput.startupTime));
	});

	model.observeLastSync([this](const String &name, const time_t &value) -> void {
		auto timeNow = ntpSync->timeStr(value);
		strcpy_s(&(dashboardOutput->syncTime[0]), timeNow.get(), sizeof(dashboardInput.syncTime));
	});

	model.observePulse([this](const String &name, const long int &value) -> void {
		dashboardOutput->currentFreeMem = system_get_free_heap_size();
		dashboardOutput->memUsage = system_get_free_heap_size() * float{1.0} / initFreeHeapSize;
	});

	model.observeTimeZone([this](const String &name, const String &value) -> void {
		Serial.printf("__________ timezone %s\n", value.c_str());

	});

}
