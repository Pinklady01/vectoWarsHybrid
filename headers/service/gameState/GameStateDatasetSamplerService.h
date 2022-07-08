#pragma once

#include <hybrid_ggpo.h>

#include "../../FrameHolder.h"
#include "../../ml/DatasetSampleMapper.h"

//constexpr int MAX_STORED_SAMPLES = 20;

namespace HybridGGPO
{
	/// <summary>
	/// Converts a game state to a dataset sample and stores it
	/// </summary>
	class GameStateDatasetSamplerService : public IGameStateService
	{
	protected:
		DatasetSampleMapper					_mapper;
		FrameHolder<double*>				_sample;
		double								_sampleBuffer[500];
		// TD: Store multiple samples
		// double*                 _sampleStore[MAX_STORED_SAMPLES];

	protected:
		virtual double*						convertGameStateToSample(unsigned char* const serializedData, int frameCount);
		virtual void						onGameStateReceived(unsigned char* const serializedData, int frameCount);

	public:
		GameStateDatasetSamplerService();
		virtual ~GameStateDatasetSamplerService();

		void								eject() override;
		FrameHolder<double*>				getSample() const;
		void								init(HybridGGPOServiceProvider* serviceProvider) override;
		void								receiveConfirmedGameState(unsigned char* const serializedData, int frameCount) override;
		void								receiveGameState(unsigned char* const serializedData, int frameCount) override;

	};
}
