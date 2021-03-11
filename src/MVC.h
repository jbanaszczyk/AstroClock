
#pragma once

#include <Arduino.h>
#include <list>
#include <utility>
#include <TaskSchedulerDeclarations.h>
#include <dashboard.h>
#include <NTPSync.h>
#include <generated/config.h>

class Model;

template<class T>
class ModelItem {
	String name;
	T value;
public:
	ModelItem(String name, T value) : name(std::move(name)), value(value) {}

	ModelItem(const ModelItem &a) : name(a.name), value(a.value) {}

	ModelItem(ModelItem &&o) noexcept:
			value(std::move(o.value)),
			name(std::move(o.name)) {}

	ModelItem &operator=(const ModelItem &other) {
		name = other.name;
		value = other.value;
		return *this;
	}

	ModelItem &operator=(ModelItem &&other) noexcept {
		name = std::move(other.name);
		value = std::move(other.value);
		return *this;
	}

	virtual ~ModelItem() = default;

	T getValue() const { return value; }
	const String &getName() const { return name; }

	void setValue(T value) {
		if (ModelItem::value != value) {
			ModelItem::value = value;
			for (const auto &observer : observers) {
				observer(ModelItem::name, ModelItem::value);
			}
		}
	}

	typedef std::function<void(const String &, const T &)> Observer;

	std::list<Observer> observers{};

	void addObserver(Observer observer) { observers.push_back(observer); }
};


class Model {
	ModelItem<time_t> now{"Time now", 0};
	ModelItem<time_t> lastSync{"Last time synchronization", 0};
	ModelItem<time_t> firstSync{"System startup time", 0};
	ModelItem<unsigned long> refresh{"Just to be periodically updated", 0};
	ModelItem<String> timeZone{"System time zone", "" };
public:
	explicit Model() = default;
	virtual ~Model() = default;

	const ModelItem<time_t> &getNow() const { return now; }
	void setNow(const time_t &now) { Model::now.setValue(now); }
	void observeNow(ModelItem<time_t>::Observer observer) { now.addObserver(std::move(observer)); }

	const ModelItem<time_t> &getLastSync() const { return lastSync; }
	void setLastSync(const time_t &lastSync) { Model::lastSync.setValue(lastSync); }
	void observeLastSync(ModelItem<time_t>::Observer observer) { lastSync.addObserver(std::move(observer)); }

	const ModelItem<time_t> &getFirstSync() const { return firstSync; }
	void setFirstSync(const time_t &firstSync) { if (Model::firstSync.getValue() == 0) { Model::firstSync.setValue(firstSync); }}
	void observeFirstSync(ModelItem<time_t>::Observer observer) { firstSync.addObserver(std::move(observer)); }

	const ModelItem<unsigned long> &getRefresh() const { return refresh; }
	void setRefresh() { refresh.setValue(refresh.getValue() + 1); }
	void observeRefresh(ModelItem<unsigned long>::Observer observer) { refresh.addObserver(std::move(observer)); }

	const ModelItem<String> &getTimeZone() const { return timeZone; }
	void setTimeZone(const String &timeZone) { Model::timeZone.setValue(timeZone); }
	void observeTimeZone(ModelItem<String>::Observer observer) { timeZone.addObserver(std::move(observer)); }
};

class Controller {
	const unsigned long RefreshInterval = 1000;
	const unsigned long SampleTimeInterval = 10000;
	Model &model;
	Scheduler *scheduler = nullptr;
	Task *tRefresh = nullptr;
	Task *tSetNow = nullptr;
	Task *tOnTimeSync = nullptr;
	Task *tConfigChanged = nullptr;

public:
	explicit Controller(Model &model) : model(model) {}

	virtual ~Controller() = default;

	void addScheduler(Scheduler *scheduler);

};

class View {
	Model &model;
	Controller &controller;
public:
	View(Model &model, Controller &controller) : model(model), controller(controller) {}
};

class SystemInterface : public View {
public:
	SystemInterface(Model &model, Controller &controller);
};

class DashboardView : public View {
	DashboardData dashboardInput = getDashboard()->getDashboardData();
	DashboardData *dashboardOutput = getDashboard()->getMutualDashboardData();
	INTPSync *ntpSync = getNTPSync();
public:
	DashboardView(Model &model, Controller &controller);
};

void initEngine();
