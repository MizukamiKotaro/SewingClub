#pragma once
#include"Vector2.h"
#include<optional>
#include<vector>

// 四角形の辺を表す構造体
struct Edge {
    Vector2 start, end;
};

// 線を表す構造体
struct Lineee {
    Vector2 start, end;
};

// 交点を計算する関数
std::optional<Vector2> CalculateIntersection(const Edge& edge, const Lineee& line);

// 四角形のすべての辺と線の交点を求める関数
std::vector<Vector2> FindIntersectionPoints(const std::vector<Edge>& edges, const Lineee& line);