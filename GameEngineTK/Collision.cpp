#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// 球と球の当たり判定
/// </summary>
/// <param name="sphereA">球A</param>
/// <param name="sphereB">球B</param>
/// <returns>true: 当たっている false:当たっていない</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// AからBに向かうベクトル
	Vector3 sub = sphereB.Center - sphereA.Center;
	// AとBの距離を計算
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// 半径の和の二乗を計算
	float radiusSquar = sphereA.Radius + sphereB.Radius;
	radiusSquar = radiusSquar * radiusSquar;

	// 距離が半径の和より大きければ当たっていない
	if (distanceSquare > radiusSquar)
	{
		return false;
	}

	// 当たっている
	return true;
}