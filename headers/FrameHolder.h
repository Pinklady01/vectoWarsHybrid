#pragma once

namespace HybridGGPO
{
	template <typename T>
	struct FrameHolder
	{
		T data;
		int frameCount;

		FrameHolder(T data, int frameCount) : data(data), frameCount(frameCount) {}
	};
}
