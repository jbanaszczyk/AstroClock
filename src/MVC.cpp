#include "MVC.h"

#include "Runner.h"
#include <NTPSync.h>
#include <ConfigManager.h>
#include "utilities.h"
#include <sntp.h>
#include <coredecls.h>
#include <umm_malloc/umm_malloc.h>

void initEngine() {

	static Model aModel{};
	static Controller aController(aModel);
	static SystemInterface systemInterface(aModel, aController);
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
					model.setRefresh();
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
		tOnTimeSync = new Task(
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
					auto newConfig = getConfigManager()->getEepromData().getStoredData().getConfigData();
					model.setTimeZone(newConfig.timeZone);
				},
				scheduler,
				true);

		settimeofday_cb([this]() -> void {
			tOnTimeSync->restart();
		});
		getConfigManager()->setConfigSaveCallback([this]() -> void {
			tConfigChanged->restart();
		});
	}
}

SystemInterface::SystemInterface(Model &model, Controller &controller) : View(model, controller) {
	model.observeTimeZone([this](const String &name, const String &value) -> void {
		configTime(value.c_str(), sntp_getservername(0), sntp_getservername(1), sntp_getservername(2));
	});
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

	model.observeTimeZone([this, model](const String &name, const String &value) -> void {
		auto timeNow = ntpSync->timeStr(ntpSync->getNow());
		strcpy_s(&(dashboardOutput->currentTime[0]), timeNow.get(), sizeof(dashboardInput.currentTime));

		auto startupTime = ntpSync->timeStr(model.getFirstSync().getValue());
		strcpy_s(&(dashboardOutput->startupTime[0]), timeNow.get(), sizeof(dashboardInput.startupTime));

		auto syncTime = ntpSync->timeStr(model.getLastSync().getValue());
		strcpy_s(&(dashboardOutput->syncTime[0]), timeNow.get(), sizeof(dashboardInput.syncTime));
	});

	model.observeRefresh([this](const String &name, const long int &value) -> void {
		dashboardOutput->currentFreeMem = system_get_free_heap_size();
		dashboardOutput->memUsage = system_get_free_heap_size() * float{1.0} / initFreeHeapSize;
	});
}
