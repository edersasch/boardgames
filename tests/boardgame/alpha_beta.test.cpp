#include "boardgame/alpha_beta.h"
#include "muehle/muehle.h"

#include "gtest/gtest.h"

TEST(Alpha_Beta, identify_win_muehle)
{
    boardgame::Alpha_Beta<muehle::Muehle_Key, muehle::Engine_Helper> engine;
    engine.start(muehle::Muehle_Key("1101000001010000000000000001001100000000100011111010010"));
    EXPECT_FALSE(engine.is_running());
    EXPECT_EQ(muehle::Muehle_Key("0110000001000000000000000001001100000000100111110010010"), engine.get_next().front());
}
