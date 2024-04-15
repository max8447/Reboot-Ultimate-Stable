#pragma once

#include "GenericPlatformMath.h"
#include "Vector.h"

struct FMath : public FGenericPlatformMath
{
	template< class T >
	static FORCEINLINE T Clamp(const T X, const T Min, const T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	template< class T >
	static FORCEINLINE T Square(const T A)
	{
		return A * A;
	}

	static FORCEINLINE int Rand()
	{
		return rand();
	}

	static FORCEINLINE float FRand()
	{
		// FP32 mantissa can only represent 24 bits before losing precision
		constexpr int32 RandMax = 0x00ffffff < RAND_MAX ? 0x00ffffff : RAND_MAX;
		return (Rand() & RandMax) / (float)RandMax;
	}

#define KINDA_SMALL_NUMBER	(1.e-4f)
	/** Return a uniformly distributed random unit length vector = point on the unit sphere surface. */
	static FORCEINLINE FVector VRand()
	{
		FVector Result;

		float L;

		do
		{
			// Check random vectors in the unit sphere so result is statistically uniform.
			Result.X = FRand() * 2.f - 1.f;
			Result.Y = FRand() * 2.f - 1.f;
			Result.Z = FRand() * 2.f - 1.f;
			L = Result.SizeSquared();
		} while (L > 1.0f || L < KINDA_SMALL_NUMBER);

		return Result * (1.0f / Sqrt(L));
	}
#undef KINDA_SMALL_NUMBER

#define FASTASIN_HALF_PI (1.5707963050f)
	/**
	* Computes the ASin of a scalar value.
	*
	* @param Value  input angle
	* @return ASin of Value
	*/
	static FORCEINLINE float FastAsin(float Value)
	{
		// Clamp input to [-1,1].
		bool nonnegative = (Value >= 0.0f);
		float x = FMath::Abs(Value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float root = FMath::Sqrt(omx);
		// 7-degree minimax approximation
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
		result *= root;  // acos(|x|)
		// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
		return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
	}
#undef FASTASIN_HALF_PI
};