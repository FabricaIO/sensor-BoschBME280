/*
* This file and associated .cpp file are licensed under the GPLv3 License Copyright (c) 2024 Sam Groveman
* Contributors: Sam Groveman
* 
* External libraries needed:
* Adafruit_BME280_Library: https://github.com/adafruit/Adafruit_BME280_Library
*
* BME280: https://www.adafruit.com/product/2652
*/

#pragma once
#include <Wire.h>
#include <Sensor.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/// @brief Device for suing BME280 sensor
class BoschBME280 : public Sensor {
	public:
		BoschBME280(int Address = 0x77, TwoWire* I2CBus = &Wire, String ConfigFile = "BME280.json");
		bool begin();
		bool takeMeasurement();
		String getConfig();
		bool setConfig(String config, bool save);
	
	protected:
		/// @brief Soil moisture sensor configuration
		struct {
			/// @brief The value air pressure as sea level
			double pressureSeaLevel;
		} current_config;

		/// @brief Adafruit BEM280 device
		Adafruit_BME280 bme;

		/// @brief I2C address of device
		int address;

		/// @brief I2C bus to use
		TwoWire* i2cbus;

		/// @brief Full path to config file
		String config_path;
};