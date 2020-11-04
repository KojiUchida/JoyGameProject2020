#pragma once

class Timer {
public:
	Timer(float maxTime = 0.0f, bool loopFlag = false);
	~Timer();

	/* 更新 */
	void Update();
	/* 開始 */
	void Start();
	/* 一時停止 */
	void Stop();
	/* リセット */
	void Reset();

	/* 残り時間の設定 */
	void SetLimit(const float limit);
	/* ループの設定 */
	void SetLoop(const bool isLoop);
	/* 時間切れか？ */
	bool IsTime();
	/* 経過時間 */
	float GetElapsed();
	/* 経過時間の割合 */
	float GetRatioElapsed();
	/* 残り時間 */
	float GetRemaining();
	/* 残り時間の割合 */
	float GetRatioRemaining();

private:
	float m_startTime;
	float m_elapsedTime;
	float m_maxTime;
	bool m_isUpdate;
	bool m_isLoop;
	bool m_isTime;
};

