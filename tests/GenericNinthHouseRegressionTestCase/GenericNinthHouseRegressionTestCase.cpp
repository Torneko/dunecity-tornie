#include <catch2/catch_all.hpp>

#include <INIMap/MapPlayerSectionUtils.h>
#include <mod/CustomHouseConfig.h>

#include <array>
#include <string>

TEST_CASE("CustomHouse config: valid numeric fields parse without throwing",
          "[custom-house][config]") {
    int value = -1;
    REQUIRE(CustomHouseConfig::parseInteger("144", value));
    REQUIRE(value == 144);

    REQUIRE(CustomHouseConfig::parseInteger("0", value));
    REQUIRE(value == 0);
}

TEST_CASE("CustomHouse config: malformed numeric fields are rejected without mutation",
          "[custom-house][config][regression]") {
    const std::array<std::string, 5> malformedValues = {
        "",
        "not-a-number",
        "144trailing",
        "144 ",
        "999999999999999999999999999999"
    };

    for(const std::string& malformedValue : malformedValues) {
        CAPTURE(malformedValue);
        int destination = 73;
        REQUIRE_FALSE(CustomHouseConfig::parseInteger(malformedValue, destination));
        REQUIRE(destination == 73);
    }
}

TEST_CASE("PlayerN-only map scans the fixed available-house capacity",
          "[map][players][regression]") {
    constexpr int availableHouseCapacity = 8;
    int sectionsScanned = 0;

    const int numberedPlayerCount = MapPlayerSectionUtils::countNumberedPlayerSections(
        availableHouseCapacity,
        [&sectionsScanned](int playerNumber) {
            ++sectionsScanned;
            return playerNumber == 1 || playerNumber == 8;
        });

    REQUIRE(sectionsScanned == availableHouseCapacity);
    REQUIRE(numberedPlayerCount == 2);
    REQUIRE(0 + numberedPlayerCount == 2);
}
