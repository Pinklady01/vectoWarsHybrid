#pragma once

#include "hybrid_ggpo.h"
#include "gamestate.h"

struct possibleValues{
    std::vector<int> possibleCooldowns = {0, 1 ,2 ,3, 4, 5, 6, 7};
    std::vector<int> possiblePlayers;
    possibleValues()
    {
        for(auto i = 1; i < MAX_SHIPS+1; ++i)
        {
            possiblePlayers.push_back(i);
        }
    }
};
class VMGameStateSampleMapper
{

private:
    static possibleValues _possibleValues;
public:
    static DatasetDSSampleMapper ConvertGameStateToSampleMapper(const GameState& gameState, int inputs[]);

};
