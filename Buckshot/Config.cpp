#include <fstream>
#include <sstream>
#include <Windows.h>
#include "Config.h"
#include "DebugLog.h"

using namespace JasonPP;

Config::Config()
{
    LoadFileJson();

    return;
}

void Config::LoadFileJson()
{
    std::ifstream ifs;
    ifs.open("config.json");
    if (!ifs.good())
    {
        DebugLog("Unable to read config file (can't read) \"config.json\"... Creating new one.");
        GenerateDefaultFile();
    }
    else
    {
        std::string jsonCode = ReadJsonFileToString();

        if (IsJsonValid(jsonCode))
        {
            Json json;
            json.Parse(jsonCode);
            LoadFromJsonObject(json);
        }
        else
        {
            FatalError_AskUserForReset("Invalid json syntax");
        }
    }
    return;
}

void Config::GenerateDefaultFile()
{
    Json cfg = JsonBlock();
    cfg["showDebugConsole"] = settings.showDebugConsole;

    cfg["keybind"] = JsonArray();
    cfg["keybind"].AsArray.Add(settings.keybind);

    cfg["defaultSavePath"] = settings.defaultSavePath;
    cfg["defaultNamingScheme"] = settings.defaultNamingScheme;

    cfg["useWhiteListInstead"] = settings.useWhiteListInstead;
    cfg["appWhitelist"] = JsonArray();
    cfg["appWhitelist"].AsArray.Add(settings.appWhitelist);

    cfg["appBlacklist"] = JsonArray();
    cfg["appBlacklist"].AsArray.Add(settings.appBlacklist);

    {
        cfg["savePathsPerApp"] = JsonArray();
        JsonArray& arrCache = cfg["savePathsPerApp"].AsArray;
        arrCache.ReserveMore(settings.appBlacklist.size());
        for (std::size_t i = 0; i < settings.savePathsPerApp.size(); i++)
        {
            JsonBlock jb;
            jb["app"] = settings.savePathsPerApp[i].first;
            jb["path"] = settings.savePathsPerApp[i].second;
            arrCache.Add(jb);
        }
    }

    {
        cfg["namingSchemePerApp"] = JsonArray();
        JsonArray& arrCache = cfg["namingSchemePerApp"].AsArray;
        arrCache.ReserveMore(settings.appBlacklist.size());
        for (std::size_t i = 0; i < settings.namingSchemePerApp.size(); i++)
        {
            JsonBlock jb;
            jb["app"] = settings.namingSchemePerApp[i].first;
            jb["scheme"] = settings.namingSchemePerApp[i].second;
            arrCache.Add(jb);
        }
    }

    std::ofstream ofs("config.json");
    ofs << "# Buckshot config file" << std::endl
        << std::endl << std::endl
        << "# If you don't want to read all this stuff:" << std::endl
        << "# It works just out of the box. F2 is the default screenshot key." << std::endl
        << std::endl << std::endl
        << "# Explanation:" << std::endl
        << "# defaultSavePath: directory where to save the screenshots. It MUST already exist." << std::endl
        << "# defaultNamingScheme: How to name the screenshots. It will always be in png format!" << std::endl
        << "# Naming variables are:" << std::endl
        << "#   Naming variables are:" << std::endl
        << "#       $(winName) : the windows title. For example \"World of Shipcraft\"" << std::endl
        << "#       $(time)    : the time the screenshot was taken at. For example \"07-11-2020 21-16-50\"" << std::endl
        << "#       $(unixtime): the timestamp the screenshot was taken at. For example \"1604780697\"" << std::endl
        << "# Tip: Use ONLY backslashes! (\\). Note that JSON requires you to escape a \\ as \\\\" << std::endl
        << "# useWhiteListInstead: Use the whitelist instead of the blacklist?" << std::endl
        << "# Tip: App names are simply their window's name. Not the name of the executeable! The name you see in the window's header bar!" << std::endl
        << "# appWhitelist: Only take screenshots of these apps. Must set useWhiteListInstead to true!" << std::endl
        << "# appBlacklist: Don't take screenshots of these apps! Must set useWhiteListInstead to false!" << std::endl
        << "# savePathsPerApp: Overwrite the save path for specific apps" << std::endl
        << "# namingSchemePerApp: Overwrite the naming scheme for specific apps" << std::endl
        << std::endl
        << "# keybind: An array of key-ids that need to be pressed all at once to take a screenshot. The default (0x71) is just F2. " << std::endl
        << "#    To use, let's say, ALT+DEL you would use this: \"\"keybind\": [ 0x12, 0x2E ]\" 0x12 is they key id of ALT and 0x2E of DEL." << std::endl
        << "#    A complete list of key ids: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes" << std::endl
        << "#    You are free to enter either decimal or hexadecimal ID's. " << std::endl
        << "#    A few more commonly used key ids are: " << std::endl
        << "#       LSHIFT    : 0xA0" << std::endl
        << "#       RSHIFT    : 0xA1" << std::endl
        << "#       LCTRL     : 0xA2" << std::endl
        << "#       RCTRL     : 0xA3" << std::endl
        << "#       ALT       : 0x12" << std::endl
        << "#       DEL       : 0x2E" << std::endl
        << "#       F1-F24    : 112 - 136" << std::endl
        << "#       0-9       : 48 - 57" << std::endl
        << "#       0-9 numpad: 96 - 105" << std::endl
        << std::endl << std::endl
        << "# ################## EDITABLE CONFIG BELOW ###################"
        << std::endl << std::endl
        << cfg << std::endl;

    ofs.close();
}

std::string Config::ReadJsonFileToString()
{
    std::ifstream ifs;
    ifs.open("config.json");
    if (!ifs.good())
    {
        DebugLog("Unable to read config file \"config.json\"...");
        exit(-1);
    }
    else
    {
        std::string buf;
        std::stringstream content;
        while (std::getline(ifs, buf))
        {
            bool isComment = false;
            for (std::size_t i = 0; i < buf.length(); i++)
            {
                if ((buf[i] != ' ') && (buf[i] != '\t'))
                {
                    isComment = buf[i] == '#';
                    break;
                }
            }

            if (!isComment) content << buf;
        }
        return content.str();
    }
}

void Config::LoadFromJsonObject(const JasonPP::Json& json)
{
    if (json.GetDataType() == JDType::JSON)
    {
        const JsonBlock& jb = json.AsJson;

        // showDebugConsole
        if (IsJsonFieldValid(jb, "showDebugConsole", JDType::BOOL))
        {
            settings.showDebugConsole = json["showDebugConsole"];
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (showDebugConsole)");
            return;
        }


        // useWhiteListInstead
        if (IsJsonFieldValid(jb, "useWhiteListInstead", JDType::BOOL))
        {
            settings.useWhiteListInstead = json["useWhiteListInstead"];
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (useWhiteListInstead)");
            return;
        }


        // defaultSavePath
        if (IsJsonFieldValid(jb, "defaultSavePath", JDType::STRING))
        {
            settings.defaultSavePath = json["defaultSavePath"];
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (defaultSavePath)");
            return;
        }

        // defaultNamingScheme
        if (IsJsonFieldValid(jb, "defaultNamingScheme", JDType::STRING))
        {
            settings.defaultNamingScheme = json["defaultNamingScheme"];
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (defaultNamingScheme)");
            return;
        }

        // appWhitelist
        if (IsJsonFieldValid(jb, "appWhitelist", JDType::ARRAY))
        {
            const JsonArray& cArr = jb["appWhitelist"].AsArray;
            settings.appWhitelist.clear();
            settings.appWhitelist.reserve(cArr.Size());
            
            for (std::size_t i = 0; i < cArr.Size(); i++)
            {
                if (cArr[i].GetDataType() != JDType::STRING) {
                    FatalError_AskUserForReset("Invalid json structure (appWhitelist)");
                    return;
                }
                settings.appWhitelist.emplace_back(cArr[i]);
            }
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (appWhitelist)");
            return;
        }

        // appBlacklist
        if (IsJsonFieldValid(jb, "appBlacklist", JDType::ARRAY))
        {
            const JsonArray& cArr = jb["appBlacklist"].AsArray;
            settings.appBlacklist.clear();
            settings.appBlacklist.reserve(cArr.Size());

            for (std::size_t i = 0; i < cArr.Size(); i++)
            {
                if (cArr[i].GetDataType() != JDType::STRING) {
                    FatalError_AskUserForReset("Invalid json structure (appBlacklist)");
                    return;
                }
                settings.appBlacklist.emplace_back(cArr[i]);
            }
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (appBlacklist)");
            return;
        }

        // savePathsPerApp
        if (IsJsonFieldValid(jb, "savePathsPerApp", JDType::ARRAY))
        {
            const JsonArray& cArr = jb["savePathsPerApp"].AsArray;
            settings.savePathsPerApp.clear();
            settings.savePathsPerApp.reserve(cArr.Size());

            for (std::size_t i = 0; i < cArr.Size(); i++)
            {
                if (cArr[i].GetDataType() != JDType::JSON) {
                    FatalError_AskUserForReset("Invalid json type (savePathsPerApp)");
                    return;
                }
                const JsonBlock& arrJb = cArr[i].AsJson;

                // appName
                std::string appName;
                if (IsJsonFieldValid(arrJb, "app", JDType::STRING))
                {
                    appName = arrJb["app"].AsString;
                }
                else {
                    FatalError_AskUserForReset("Invalid json structure (savePathsPerApp[].app)");
                    return;
                }

                // appName
                std::string savePath;
                if (IsJsonFieldValid(arrJb, "path", JDType::STRING))
                {
                    savePath = arrJb["path"].AsString;
                }
                else {
                    FatalError_AskUserForReset("Invalid json structure (savePathsPerApp[].path)");
                    return;
                }
                 
                settings.savePathsPerApp.emplace_back(appName, savePath);
            }
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (savePathsPerApp)");
            return;
        }

        // namingSchemePerApp
        if (IsJsonFieldValid(jb, "namingSchemePerApp", JDType::ARRAY))
        {
            const JsonArray& cArr = jb["namingSchemePerApp"].AsArray;
            settings.namingSchemePerApp.clear();
            settings.namingSchemePerApp.reserve(cArr.Size());

            for (std::size_t i = 0; i < cArr.Size(); i++)
            {
                if (cArr[i].GetDataType() != JDType::JSON) {
                    FatalError_AskUserForReset("Invalid json type (namingSchemePerApp)");
                    return;
                }
                const JsonBlock& arrJb = cArr[i].AsJson;

                // appName
                std::string appName;
                if (IsJsonFieldValid(arrJb, "app", JDType::STRING))
                {
                    appName = arrJb["app"].AsString;
                }
                else {
                    FatalError_AskUserForReset("Invalid json structure (namingSchemePerApp[].app)");
                    return;
                }

                // appName
                std::string namingScheme;
                if (IsJsonFieldValid(arrJb, "scheme", JDType::STRING))
                {
                    namingScheme = arrJb["scheme"].AsString;
                }
                else {
                    FatalError_AskUserForReset("Invalid json structure (namingSchemePerApp[].path)");
                    return;
                }

                settings.namingSchemePerApp.emplace_back(appName, namingScheme);
            }
        }
        else {
            FatalError_AskUserForReset("Invalid json structure (namingSchemePerApp)");
            return;
        }
    }
    else {
        FatalError_AskUserForReset("Invalid json structure");
        return;
    }

    return;
}

void Config::FatalError_AskUserForReset(const std::string reason)
{
    int i = MessageBoxA(0, (std::string("Fatal error in config file:") + reason + "\nReload default config and continue?").c_str(), "Buckshot: Can't parse config!", MB_ICONWARNING | MB_OKCANCEL);
    if (i == IDOK)
    {
        DebugLog(std::string("Unable to read config file (") + reason + ") \"config.json\"... Creating new one.");
        GenerateDefaultFile();

        Settings foo;
        settings = foo;
    }
    else
    {
        exit(-1);
    }
}

bool Config::IsJsonFieldValid(const JsonBlock& jb, std::string key, JDType type)
{
    return ((jb.DoesExist(key)) && (jb[key].GetDataType() == type));
}

Config::Settings Config::settings;