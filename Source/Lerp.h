#pragma once
// Author:H.Suginuma

static float easeInSine(float x) {
	return 1 - cosf((x * DX_PI_F) / 2);
}

/// <summary>
/// 補間関数
/// </summary>
/// <typeparam name="T">補間するものの型</typeparam>
/// <param name="start">開始点</param>
/// <param name="end">終了点</param>
/// <param name="rate">割合(0.0～1.0)</param>
/// <param name="ease">イージング関数(省略可)</param>
/// <returns>補間後の値</returns>
template <typename T>
T Lerp(T start, T end, float rate, float (*ease)(float x) = nullptr) {
	if (rate < 0.0f)rate = 0.0f;
	if (rate > 1.0f)rate = 1.0f;
	if (ease != nullptr) {
		rate = ease(rate);
	}
	return (end - start) * rate + start;
}
