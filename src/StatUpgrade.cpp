#include "pch.h"
#include "StatUpgrade.h"
std::string StatUpgrade::GetStatString() {
	static std::ostringstream oss; // static to avoid re creating.
	oss.clear(); // clear and reset string to "".
	oss.str("");
	oss << "Grants + " << getFlat() << " | " << (getMult() - 1) * 100 << "%" << StatTypeToString[type];
	return oss.str();

}

void StatUpgrade::LoadFlat(std::string statType) {
	auto& json = GameData::getStatUpgrade(statType);

	if (json.contains("flat")) {
		const auto& jsonArray = json["flat"];
		if (jsonArray.is_array() && jsonArray.size() == flatStats.size()) {
			flatStats = jsonArray.get<std::array<int, 10>>(); // Direct assignment (C++17+)
		}
	}
}
void StatUpgrade::LoadMult(std::string statType) {
	auto& json = GameData::getStatUpgrade(statType);

	if (json.contains("mult")) {
		const auto& jsonArray = json["mult"];
		if (jsonArray.is_array() && jsonArray.size() == flatStats.size()) {
			multStats = jsonArray.get<std::array<float, 10>>(); // Direct assignment (C++17+)
		}
	}


}