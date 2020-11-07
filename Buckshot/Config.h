#pragma once
#include "Dependencies/JasonPP.hpp"
#include <utility>
#include <vector>

class Config
{
public:
	struct Settings
	{
	public:
		bool showDebugConsole = false;
		std::vector<int> keybind = { 0x71 };

		std::string defaultSavePath = "screenshots";
		std::string defaultNamingScheme = "$(winName) - $(time).png";

		std::vector<std::pair<std::string, std::string>> savePathsPerApp = { {"World of Shipduty", "C:\\my WoS screenshots\""} , {"Call of Warcraft", "C:\\user\\piggyback\\images\\my CoW screenshots\""} };
		std::vector<std::pair<std::string, std::string>> namingSchemePerApp = { {"World of Shipduty", "WoS - $(time).png"} };
		std::vector<std::string> appWhitelist;
		std::vector<std::string> appBlacklist;
		
		bool useWhiteListInstead = false;

	};

public:
	Config();

	static const Settings& GetSettings() { return settings; }

private:
	void LoadFileJson();
	void GenerateDefaultFile();
	std::string ReadJsonFileToString();
	void LoadFromJsonObject(const JasonPP::Json& json);
	void FatalError_AskUserForReset(const std::string reason);
	bool IsJsonFieldValid(const JasonPP::JsonBlock& jb, std::string key, JasonPP::JDType type);

	static Settings settings;
};

