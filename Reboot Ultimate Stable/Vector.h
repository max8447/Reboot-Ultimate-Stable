#pragma once

#include "inc.h"
#include "UnrealString.h"
#include "GenericPlatformMath.h"

struct FVector
{
public:
#ifdef ABOVE_S20
	using VectorDataType = double;
#else
	using VectorDataType = float;
#endif

	VectorDataType X;
	VectorDataType Y;
	VectorDataType Z;

	FString ToString() const
	{
		FString Result;
		Result.Set((std::to_wstring(X) + L", " + std::to_wstring(Y) + L", " + std::to_wstring(Z)).c_str());
		return Result;
	}

	bool CompareVectors(const FVector& A)
	{
		return X == A.X && Y == A.Y && Z == A.Z;
	}

	FORCEINLINE bool Normalize(float Tolerance)
	{
		const float SquareSum = X * X + Y * Y + Z * Z;
		if (SquareSum > Tolerance)
		{
			const float Scale = FGenericPlatformMath::InvSqrt(SquareSum);
			X *= Scale; Y *= Scale; Z *= Scale;
			return true;
		}
		return false;
	}

	FVector() : X(0), Y(0), Z(0) {}
	FVector(VectorDataType x, VectorDataType y, VectorDataType z) : X(x), Y(y), Z(z) {}

	FVector operator+(const FVector& A)
	{
		return FVector{ this->X + A.X, this->Y + A.Y, this->Z + A.Z };
	}

	FVector operator-(const FVector& A)
	{
		return FVector{ this->X - A.X, this->Y - A.Y, this->Z - A.Z };
	}

	FORCEINLINE VectorDataType SizeSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}

	FORCEINLINE VectorDataType Size() const
	{
		return sqrt(SizeSquared());
	}

	FORCEINLINE VectorDataType DistanceTo(const FVector& Other) const
	{
		return (FVector(*this) - Other).Size();
	}

	FORCEINLINE VectorDataType operator|(const FVector& V) const
	{
		return X * V.X + Y * V.Y + Z * V.Z;
	}

	FVector operator*(const VectorDataType A)
	{
		return FVector{ this->X * A, this->Y * A, this->Z * A };
	}

	/* bool operator==(const FVector& A)
	{
		return X == A.X && Y == A.Y && Z == A.Z;
	} */

	void operator+=(const FVector& A)
	{
		*this = *this + A;
	}

	void operator-=(const FVector& A)
	{
		*this = *this - A;
	}
};