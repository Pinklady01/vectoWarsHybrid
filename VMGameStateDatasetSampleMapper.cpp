#include "VMGameStateDatasetSampleMapper.h"

DatasetDSSampleMapper VMGameStateDatasetSampleMapper::ConvertGameStateToSampleMapper(const GameState &gameState, const int inputs[]) {
    DatasetDSSampleMapper sampleMapper;
    for(auto i = 0; i < gameState._num_ships; ++i)
    {
        auto ship = gameState._ships[i];
        sampleMapper.add(ship.position.x);
        sampleMapper.add(ship.position.y);
        sampleMapper.add(ship.velocity.dx);
        sampleMapper.add(ship.velocity.dy);
        sampleMapper.add(ship.velocity.dx);
        sampleMapper.add(ship.velocity.dx);
        sampleMapper.add(ship.heading);
        sampleMapper.add(ship.health);
        sampleMapper.add(ship.speed);
        sampleMapper.add(ship.cooldown);
        for(auto bullet : ship.bullets)
        {
            sampleMapper.add(bullet.active);
            sampleMapper.add(bullet.position.x);
            sampleMapper.add(bullet.position.y);
            sampleMapper.add(bullet.velocity.dx);
            sampleMapper.add(bullet.velocity.dy);
        }
        sampleMapper.add(ship.score);
        sampleMapper.addCategorical(ship.cooldown, _possibleValues.possibleCooldowns);
        sampleMapper.addCategorical(i, _possibleValues.possiblePlayers);
        sampleMapper.add(inputs[i] & INPUT_ROTATE_RIGHT);
        sampleMapper.add(inputs[i] & INPUT_ROTATE_LEFT);
        sampleMapper.add(inputs[i] & INPUT_THRUST);
        sampleMapper.add(inputs[i] & INPUT_BREAK);
        sampleMapper.add(inputs[i] & INPUT_FIRE);
    }
    return sampleMapper;

}
