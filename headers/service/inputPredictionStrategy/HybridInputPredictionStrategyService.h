#pragma once

#include <hybrid_ggpo.h>

#include "../../../vectorwar.h"
#include "../../FrameHolder.h"
#include "../../ml/MLPModelWrapper.h"
#include "../gameState/GameStateDatasetSamplerService.h"

namespace HybridGGPO
{
    class HybridGGPOServiceProvider;

    class HybridInputPredictionStrategyService : public IInputPredictionStrategyService
    {
    protected:
        FrameHolder<VectorWarInputs>        _lastInput;
        MLPModelWrapper*                    _models[GGPO_MAX_PLAYERS];
        GameStateDatasetSamplerService*     _gameStateService = NULL;
    public:
        HybridInputPredictionStrategyService();
        virtual ~HybridInputPredictionStrategyService();

        void                                confirm(int frameNumber, void* inputs, int inputSize, int playerIndex) override;
        void                                eject() override;
        void                                init(HybridGGPOServiceProvider* serviceProvider) override;
        void                                predict(int requestedFrame, void* inputs, int inputSize, int playerIndex) override;
    };
}
