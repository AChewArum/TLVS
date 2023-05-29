#pragma once

#include "Math\TL_Math.h"

class Transform
{
public:
	Transform();
	~Transform();

	void UpdateWorld();

	TL_Math::Matrix& GetWorldMatrix();

	TL_Math::Vector3& Pos();
	TL_Math::Vector3& Rot();
	TL_Math::Quaternion& Quat();
	TL_Math::Vector3& Scale();

	const TL_Math::Vector3& PosW();
	const TL_Math::Vector3& RotW();
	const TL_Math::Vector3& ScaleW();

	const TL_Math::Vector3& Up();
	const TL_Math::Vector3& Right();
	const TL_Math::Vector3& Forward();
private:
	TL_Math::Vector3 pos, rot, scale;
	TL_Math::Quaternion quat;

	TL_Math::Vector3 posW, rotW, scaleW;
	TL_Math::Quaternion quatW;

	TL_Math::Vector3 up, right, forward;

	TL_Math::Matrix local, S, R, T;
	TL_Math::Matrix world;

	Transform* parent;

	const static TL_Math::Vector3 WorldUp, WorldRight, WorldForward;
};

inline TL_Math::Matrix& Transform::GetWorldMatrix()
{ 
	 return world; 
}

inline TL_Math::Vector3& Transform::Pos()
{
	return pos;
}
inline TL_Math::Vector3& Transform::Rot()
{
	return rot;
}
inline TL_Math::Vector3& Transform::Scale()
{
	return scale;
}

inline TL_Math::Quaternion& Transform::Quat()
{
	return quat;
}

inline const TL_Math::Vector3& Transform::PosW()
{
	return posW;
}

inline const TL_Math::Vector3& Transform::RotW()
{
	return rotW;
}

inline const TL_Math::Vector3& Transform::ScaleW()
{
	return scaleW;
}

inline const TL_Math::Vector3& Transform::Up()
{
	XMStoreFloat3(&up, XMVector4Transform(WorldUp, world));
	up.Normalize();
	return  up;
}

inline const TL_Math::Vector3& Transform::Right()
{
	XMStoreFloat3(&right, XMVector4Transform(WorldRight, world));
	right.Normalize();
	return  right;
}

inline const TL_Math::Vector3& Transform::Forward()
{
	XMStoreFloat3(&forward, XMVector4Transform(WorldForward, world));
	forward.Normalize();
	return  forward;
}