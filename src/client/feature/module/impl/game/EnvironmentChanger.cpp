#include "pch.h"
#include "EnvironmentChanger.h"
#include <client/event/impl/WeatherEvent.h>

EnvironmentChanger::EnvironmentChanger() : Module("EnvironmentChanger", "Environment Changer", "Changes visual features in the environment.", GAME) {
	listen<WeatherEvent>((EventListenerFunc)&EnvironmentChanger::onWeather);
	listen<FogColorEvent>((EventListenerFunc)&EnvironmentChanger::onFogColor);
	listen<GetTimeEvent>((EventListenerFunc)&EnvironmentChanger::onTime);

	addSetting("setFogColor", "Set Fog Color", "", setFogColor);
	addSetting("fogColor", "Fog Color", "The new fog color", fogColor, "setFogColor"_istrue);
	addSetting("setTime", "Set Time", "Whether or not to set the time", setTime);
	addSliderSetting("timeToSet", "Time", "The new time of day to set", time, FloatValue(0.f), FloatValue(1.f), FloatValue(0.1f), "setTime");
	addSetting("showWeather", "Show Weather", "Whether or not to show the weather", showWeather);
}

void EnvironmentChanger::onWeather(Event& evG) {
	auto& ev = reinterpret_cast<WeatherEvent&>(evG);

	if (!std::get<BoolValue>(showWeather)) {
		ev.setShowWeather(false);
	}
}

void EnvironmentChanger::onFogColor(Event& evG) {
	auto& ev = reinterpret_cast<FogColorEvent&>(evG);
	if (std::get<BoolValue>(setFogColor)) {
		auto col = std::get<ColorValue>(fogColor).color1;
		ev.setColor(col.r, col.g, col.b);
	}
}

void EnvironmentChanger::onTime(Event& evG) {
	auto& ev = reinterpret_cast<GetTimeEvent&>(evG);

	if (std::get<BoolValue>(setTime)) {
		ev.setTime(std::get<FloatValue>(time));
	}
}
