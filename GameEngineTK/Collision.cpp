#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// ���Ƌ��̓����蔻��
/// </summary>
/// <param name="sphereA">��A</param>
/// <param name="sphereB">��B</param>
/// <returns>true: �������Ă��� false:�������Ă��Ȃ�</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// A����B�Ɍ������x�N�g��
	Vector3 sub = sphereB.Center - sphereA.Center;
	// A��B�̋������v�Z
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// ���a�̘a�̓����v�Z
	float radiusSquar = sphereA.Radius + sphereB.Radius;
	radiusSquar = radiusSquar * radiusSquar;

	// ���������a�̘a���傫����Γ������Ă��Ȃ�
	if (distanceSquare > radiusSquar)
	{
		return false;
	}

	// �������Ă���
	return true;
}