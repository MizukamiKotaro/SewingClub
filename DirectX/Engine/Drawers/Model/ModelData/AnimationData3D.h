#pragma once

#include <vector>
#include <string>
#include <map>
#include "Matrix4x4.h"
#include "Quaternion.h"

struct NodeAnime
{
	struct Positions
	{
		float time;
		Vector3 position;
	};

	struct Rotates
	{
		float time;
		Quaternion rotate;
	};

	struct Scales
	{
		float time;
		Vector3 scale;
	};

	std::string name;
	std::vector<Positions> positions;
	std::vector<Rotates> rotates;
	std::vector<Scales> scales;
};

struct AnimationData
{
	float duration;
	float ticksPerSecond;
	std::vector<NodeAnime> channels;
};

template <typename tValue>
struct Keyframe
{
	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve
{
	std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation
{
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

class Animation
{
public:
	float duration; // アニメーション全体の尺
	std::map<std::string, NodeAnimation> nodeAnimations;
};