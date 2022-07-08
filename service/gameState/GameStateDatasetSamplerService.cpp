#include "../../headers/service/gameState/GameStateDatasetSamplerService.h"
#include "../../gamestate.h"

using namespace HybridGGPO;

GameStateDatasetSamplerService::GameStateDatasetSamplerService() : _mapper(_sampleBuffer, 500), _sample(NULL, -1), _sampleBuffer()
{
}

GameStateDatasetSamplerService::~GameStateDatasetSamplerService()
{
}

void GameStateDatasetSamplerService::eject()
{
}

FrameHolder<double*> GameStateDatasetSamplerService::getSample() const
{
	return this->_sample;
}

void GameStateDatasetSamplerService::init(HybridGGPOServiceProvider*)
{
}

double* GameStateDatasetSamplerService::convertGameStateToSample(unsigned char* const serializedData, int)
{
	GameState* const gameState = reinterpret_cast<GameState* const>(serializedData);

	for (int i = 0; i < gameState->_num_ships; ++i)
	{
		Ship& ship = gameState->_ships[i];

		this->_mapper.add(ship.position.x);
		this->_mapper.add(ship.position.y);
		this->_mapper.add(ship.velocity.dx);
		this->_mapper.add(ship.velocity.dy);
		this->_mapper.add(ship.velocity.dx);
		this->_mapper.add(ship.velocity.dx);
		this->_mapper.add(ship.heading);
		this->_mapper.add(ship.health);
		this->_mapper.add(ship.speed);
		this->_mapper.add(ship.cooldown);
		this->_mapper.add(ship.score);

		for (Bullet& bullet : ship.bullets)
		{
			this->_mapper.add(bullet.active);
			this->_mapper.add(bullet.position.x);
			this->_mapper.add(bullet.position.y);
			this->_mapper.add(bullet.velocity.dx);
			this->_mapper.add(bullet.velocity.dy);
		}
	}

	this->_mapper.getData(this->_sampleBuffer);
	this->_mapper.reset();

	return this->_sampleBuffer;
}

void GameStateDatasetSamplerService::onGameStateReceived(unsigned char* const serializedData, int frameCount)
{
	if (this->_sample.frameCount < frameCount)
		return;

	this->_sample.data = this->convertGameStateToSample(serializedData, frameCount);
	this->_sample.frameCount = frameCount;
}

void GameStateDatasetSamplerService::receiveConfirmedGameState(unsigned char* const serializedData, int frameCount)
{
	this->onGameStateReceived(serializedData, frameCount);
}

void GameStateDatasetSamplerService::receiveGameState(unsigned char* const serializedData, int frameCount)
{
	this->onGameStateReceived(serializedData, frameCount);
}
