#pragma once
#include "Vector3.h"

/// <summary>
/// 球構造体
/// </summary>
struct Sphere {
	Vector3 center; // 中心座標
	float radius; // 半径	
};

/// <summary>
/// 平面構造体
/// </summary>
struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

/// <summary>
/// 線構造体
/// </summary>
struct Line {
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

/// <summary>
/// 半直線構造体
/// </summary>
struct Ray {
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

/// <summary>
/// 線分構造体
/// </summary>
struct Segment {
	Vector3 origin; // 始点
	Vector3 diff; // 終点
};

/// <summary>
/// 三角形構造体
/// </summary>
struct Triangle {
	Vector3 vertex[3]; // 頂点
};

/// <summary>
/// 立方体構造体
/// </summary>
struct AABB {
	Vector3 min; // 最小点
	Vector3 max; // 最大点
};