#pragma once

#include <cmath>
#include <array>
#include <limits>

class SinCosTanLookupTable {
public:
	static constexpr size_t TABLE_SIZE = 1024;
	static constexpr float PI = 3.14159265358979323846f;
	static constexpr float TWO_PI = 2.0f * PI;
	static constexpr float HALF_PI = PI / 2.0f;

	SinCosTanLookupTable() {
		for (size_t i = 0; i < TABLE_SIZE; ++i) {
			float angle = static_cast<float>(i) / TABLE_SIZE * TWO_PI;
			table[i] = std::sin(angle);
		}
	}

	float sin(float angle) {
		return lookup(angle);
	}

	float cos(float angle) {
		return lookup(fmod(angle + 0.25f, 1.0f));
	}

	float tan(float angle) {
		float cosVal = cos(angle);
		if (std::abs(cosVal) < 1e-6f) {
			return cosVal > 0 ? std::numeric_limits<float>::max() : -std::numeric_limits<float>::max();
		}
		return sin(angle) / cosVal;
	}

private:
	std::array<float, TABLE_SIZE> table;

	float lookup(float angle) {
		float indexFloat = angle * TABLE_SIZE;
		size_t index = static_cast<size_t>(indexFloat);

		float t = indexFloat - index;
		float a = table[index];
		float b = table[(index + 1) % TABLE_SIZE];

		// linear interpolation
		return a * (1 - t) + b * t;
	}
};
