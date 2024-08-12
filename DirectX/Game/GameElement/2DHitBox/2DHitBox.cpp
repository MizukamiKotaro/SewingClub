#include"2DHitBox.h"

// 交点を計算する関数
std::optional<Vector2> CalculateIntersection(const Edge& edge, const Lineee& line) {
    Vector2 r = edge.end - edge.start;
    Vector2 s = line.end - line.start;

    float rxs = r.x * s.y - r.y * s.x;
    float t = ((line.start - edge.start).x * s.y - (line.start - edge.start).y * s.x) / rxs;
    float u = ((line.start - edge.start).x * r.y - (line.start - edge.start).y * r.x) / rxs;

    // tとuが0から1の間にある場合、交点が存在する
    if (rxs != 0 && t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        return edge.start + r * t;
    }

    return std::nullopt;  // 交点が存在しない場合
}

// 四角形のすべての辺と線の交点を求める関数
std::vector<Vector2> FindIntersectionPoints(const std::vector<Edge>& edges, const Lineee& line) {
    std::vector<Vector2> intersection_points;

    for (const auto& edge : edges) {
        auto intersection = CalculateIntersection(edge, line);
        if (intersection.has_value()) {
            intersection_points.push_back(intersection.value());
        }
    }

    return intersection_points;
}