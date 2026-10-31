#include <gtest/gtest.h>

#include "presentation/AppConfig.hpp"

using namespace casino;

TEST(AppConfigTest, Defaults) {
    AppConfig config = parseArgs({});
    EXPECT_EQ(config.mode, Mode::Manual);
    EXPECT_EQ(config.seed, 42);
    EXPECT_NE(usage().find("--seed"), std::string::npos);
}

TEST(AppConfigTest, BotOptions) {
    AppConfig config = parseArgs({"--mode", "bot", "--seed", "7", "--strategy", "martingale",
                                  "--spins", "200", "--loss-limit", "300", "--win-goal", "100"});
    EXPECT_EQ(config.mode, Mode::Bot);
    EXPECT_EQ(config.seed, 7);
    EXPECT_EQ(config.strategy, "martingale");
    EXPECT_EQ(config.spins, 200);
    EXPECT_EQ(config.lossLimit, 300);
    EXPECT_EQ(config.winGoal, 100);
}

TEST(AppConfigTest, Errors) {
    EXPECT_THROW(parseArgs({"--abc", "1"}), std::invalid_argument);
    EXPECT_THROW(parseArgs({"--seed"}), std::invalid_argument);
    EXPECT_THROW(parseArgs({"--seed", "abc"}), std::invalid_argument);
}
