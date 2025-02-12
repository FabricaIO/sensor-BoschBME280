#include "BoschBME280.h"

/// @brief Creates an Adafruit280 device
/// @param Address The I2C address to use
/// @param I2CBus A pointer to the I2C bus to use
/// @param ConfigFile The file name to store settings in
BoschBME280::BoschBME280(int Address, TwoWire* I2CBus, String ConfigFile) {
	config_path = "/settings/sen/" + ConfigFile;
	address = Address;
	i2cbus = I2CBus;
}

bool BoschBME280::begin() {
	Description.name = "Bosch BME280";
	Description.type = "environmental sensor";
	Description.parameterQuantity = 4;
	Description.parameters = {"Temperature", "Pressure", "Altitude", "Humidity"};
	Description.units = {"C", "hPa", "m", "%RH"};
	Description.version = "0.5";
	values.resize(Description.parameterQuantity);
	bool result = false;
	// Create settings directory if necessary
	if (!checkConfig(config_path)) {
		// Set defaults
		result = setConfig(R"({"pressureSeaLevel": 1013.25})", true);
	} else {
		// Load settings
		result = setConfig(Storage::readFile(config_path), false);
	}
	if (result) {
		result = bme.begin(address, i2cbus);
	}
	return result;
}

bool BoschBME280::takeMeasurement() {
	values[0] = (double)bme.readTemperature();
	values[1] = (double)(bme.readPressure() / 100.0F);
	values[2] = (double)bme.readAltitude(current_config.pressureSeaLevel);
	values[3] = (double)bme.readHumidity();
	return true;
}

/// @brief Gets the current config
/// @return A JSON string of the config
String BoschBME280::getConfig() {
	// Allocate the JSON document
	JsonDocument doc;
	// Assign current values
	doc["pressureSeaLevel"] = current_config.pressureSeaLevel;

	// Create string to hold output
	String output;
	// Serialize to string
	serializeJson(doc, output);
	return output;
}

/// @brief Sets the configuration for this device
/// @param config A JSON string of the configuration settings
/// @param save If the configuration should be saved to a file
/// @return True on success
bool BoschBME280::setConfig(String config, bool save) {
	// Allocate the JSON document
  	JsonDocument doc;
	// Deserialize file contents
	DeserializationError error = deserializeJson(doc, config);
	// Test if parsing succeeds.
	if (error) {
		Logger.print(F("Deserialization failed: "));
		Logger.println(error.f_str());
		return false;
	}
	// Assign loaded values
	current_config.pressureSeaLevel = doc["pressureSeaLevel"].as<double>();
	if (save) {
		return saveConfig(config_path, config);
	}
	return true;
}