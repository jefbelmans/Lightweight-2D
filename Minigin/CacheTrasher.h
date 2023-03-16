#pragma once
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <chrono>
#include <numeric>
#include <algorithm>
#include "Singleton.h"

namespace LW2D
{
	struct Transf
	{
		float matrix[16] =
		{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1,
		};
	};

	class GameObject3D
	{
	public:
		GameObject3D() : transform(Transf()), ID(20) {}
		Transf transform;
		int ID;
	};

	class GameObject3DAlt
	{
	public:
		GameObject3DAlt() : transform(new Transf()), ID(21) {}
		~GameObject3DAlt() {
			if (transform != nullptr)
			{
				delete transform;
				transform = nullptr;
			}
		}
		Transf* transform;
		int ID;
	};

	class CacheTrasher final : public Singleton<CacheTrasher>
	{
	public:
		template <typename T> std::vector<float> TakeMeasurement(int nrMeasurements);
	};

	template<typename T>
	inline std::vector<float> CacheTrasher::TakeMeasurement(int nrMeasurements)
	{
		// Guard clause typename T
		if constexpr (!std::is_same<T, float_t>::value && !std::is_same<T, GameObject3D>::value && !std::is_same<T, GameObject3DAlt>::value)
			throw std::runtime_error("\"TakeMeasurement()\" only supports float, GameObject3D or GameObject3DAlt!\n");

		// Init data
		int vecLength = 10'000'000;
		std::vector<T> vec(vecLength);
		constexpr int maxSteps = 11;
		std::vector<std::vector<long long>> measurements(maxSteps, std::vector<long long>(nrMeasurements));

		// Take measurements
		for (int i = 0; i < nrMeasurements; i++)
		{
			int id = 0;
			for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
			{
				auto start = std::chrono::high_resolution_clock::now();
				for (int j = 0; j < vecLength; j += stepsize)
				{
					if constexpr(std::is_same<T, GameObject3D>::value || std::is_same<T, GameObject3DAlt>::value)
						vec[j].ID *= 2;
					else 
						vec[j] *= 2;
				}
				auto end = std::chrono::high_resolution_clock::now();

				auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				measurements[id++][i] = total;
			}
		}

		// Erase outliers from data set
		int stepSize{ 1 };
		std::vector<float> avarages;
		for (int i = 0; i < maxSteps; i++)
		{
			std::sort(measurements[i].begin(), measurements[i].end());
			measurements[i].pop_back();
			measurements[i].erase(measurements[i].begin());
			float avarage = std::accumulate(measurements[i].begin(), measurements[i].end(), 0.f) / measurements[i].size();
			stepSize *= 2;
			avarages.push_back(avarage);
		}

		return avarages;
	}
}