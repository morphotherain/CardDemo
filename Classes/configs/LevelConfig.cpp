// LevelConfig.cpp
#include "LevelConfig.h"
#include "external/nlohmann/json.hpp"

using json = nlohmann::json;

bool LevelConfig::parseFromJson(const std::string& jsonStr) {
    try {
        json j = json::parse(jsonStr);

        // Ω‚ŒˆPlayfieldø®≈∆
        if (j.contains("Playfield") && j["Playfield"].is_array()) {
            for (auto& cardJson : j["Playfield"]) {
                CardInfo card;
                card.cardFace = cardJson["CardFace"].get<int>();
                card.cardSuit = cardJson["CardSuit"].get<int>();
                card.position.x = cardJson["Position"]["x"].get<float>();
                card.position.y = cardJson["Position"]["y"].get<float>();
                playfieldCards.push_back(card);
            }
        }

        // Ω‚ŒˆStackø®≈∆
        if (j.contains("Stack") && j["Stack"].is_array()) {
            for (auto& cardJson : j["Stack"]) {
                CardInfo card;
                card.cardFace = cardJson["CardFace"].get<int>();
                card.cardSuit = cardJson["CardSuit"].get<int>();
                card.position.x = cardJson["Position"]["x"].get<float>();
                card.position.y = cardJson["Position"]["y"].get<float>();
                stackCards.push_back(card);
            }
        }

        return true;
    }
    catch (const std::exception& e) {
        CCLOGERROR("JSON parsing error: %s", e.what());
        return false;
    }
}