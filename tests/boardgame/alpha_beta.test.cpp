#include "boardgame/alpha_beta.h"
#include "muehle/muehle.h"

#include "gtest/gtest.h"

TEST(Alpha_Beta, identify_win_muehle)
{
    muehle::Muehle_Move_Data md;
    boardgame::Alpha_Beta<muehle::Muehle_Key, muehle::Engine_Helper, muehle::Muehle_Move_Data> engine;
    md.consecutive_boring_moves.push_back(0); // consecutive_boring_moves must not be empty, not even for first move
    engine.start(muehle::Muehle_Key("1101000001010000000000000001001100000000100011111010010"), md);
    EXPECT_FALSE(engine.is_running());
    EXPECT_EQ(muehle::Muehle_Key("0110000001000000000000000001001100000000100111110010010"), engine.get_next().front());
}
