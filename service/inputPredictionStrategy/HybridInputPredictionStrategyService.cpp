#include "../../headers/service/inputPredictionStrategy/HybridInputPredictionStrategyService.h"

using namespace HybridGGPO;

HybridInputPredictionStrategyService::HybridInputPredictionStrategyService() : _lastInput((VectorWarInputs)0, -1)
{
	for (int i = 0; i < GGPO_MAX_PLAYERS; ++i)
		this->_models[i] = NULL;
}

HybridInputPredictionStrategyService::~HybridInputPredictionStrategyService()
{
	for (int i = 0; i < GGPO_MAX_PLAYERS; ++i)
	{
		if (this->_models[i])
		{
			delete this->_models[i];
			this->_models[i] = NULL;
		}
	}
}

void HybridInputPredictionStrategyService::eject()
{
}

void HybridInputPredictionStrategyService::init(HybridGGPOServiceProvider* serviceProvider)
{
	GameStateDatasetSamplerService* service = dynamic_cast<GameStateDatasetSamplerService*>(serviceProvider->getGameStateService());

	if (!service)
		throw std::exception("HybridInputPredictionStrategyService needs a AGameStateDatasetSamplerService to work");

	this->_gameStateService = service;
}

void HybridInputPredictionStrategyService::confirm(int frameNumber, void*, int, int playerIndex)
{
	FrameHolder<double*> sample = this->_gameStateService->getSample();

	if (sample.frameCount != frameNumber)
		return;

	auto model = this->_models[playerIndex];

	if (!model)
	{
		std::vector<uint> npl = { 322, 30, 6 };
		model = new MLPModelWrapper(npl);
	}

	// TD : Format input from real input data
	static double inputs[6] { 1, 0, 0, 0, 0, 0 };

	model->trainSingle(sample.data, inputs, 0.05);
	this->_lastInput.frameCount = frameNumber;
}

void HybridInputPredictionStrategyService::predict(int requestedFrame, void* inputs, int, int playerIndex)
{
	FrameHolder<double*> sample = this->_gameStateService->getSample();

	if (sample.frameCount != requestedFrame)
		return;

	auto model = this->_models[playerIndex];

	if (!model)
		return;

	m1 result = model->predict(sample.data);

	// TD : return predicted input data
	((int*)inputs)[playerIndex] = VectorWarInputs::INPUT_FIRE;
}
