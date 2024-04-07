#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "discord.h"
#include "log.h"
#include "json.hpp"

using json = nlohmann::json;

static inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t total_size = size * nmemb;
    buffer->append((char*)contents, total_size);
    return total_size;
}

inline std::vector<std::string> PlayerBotNames;

static inline DWORD WINAPI InitBotNames(LPVOID)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        LOG_ERROR(LogBots, "Failed to init BotNames.");
        return 0;
    }

    std::string url = "https://raw.githubusercontent.com/max8447/Fortnite-Bot-Names/main/PhoebeNames_NA.json";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string response;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        LOG_ERROR(LogBots, "Failed to init BotNames: {}", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    json jsonData = json::parse(response);

    for (const auto& row : jsonData[0]["Rows"])
    {
        PlayerBotNames.push_back(row["Name"]);
    }
}