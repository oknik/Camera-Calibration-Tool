#include "../pch.h"
enum __declspec(dllexport)RadialDistortionType{
	TWO_COEFFICIENTS, // use two radial distortion parameters£¨K1, K2£©
	THREE_COEFFICIENTS // use three radial distortion parameters£¨K1, K2, K3£©
};

struct __declspec(dllexport)CalibrationOptions {
	RadialDistortionType RadialDistortion = THREE_COEFFICIENTS;
	bool UseTangentialDistortion = false; // whether to use tangential distortion
	bool OptimizeParameters = false; // whether to use optimize parameters
	bool SkewEnabled = false; // whether to use skew
};