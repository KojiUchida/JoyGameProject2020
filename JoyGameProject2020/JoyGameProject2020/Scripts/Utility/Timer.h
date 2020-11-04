#pragma once

class Timer {
public:
	Timer(float maxTime = 0.0f, bool loopFlag = false);
	~Timer();

	/* �X�V */
	void Update();
	/* �J�n */
	void Start();
	/* �ꎞ��~ */
	void Stop();
	/* ���Z�b�g */
	void Reset();

	/* �c�莞�Ԃ̐ݒ� */
	void SetLimit(const float limit);
	/* ���[�v�̐ݒ� */
	void SetLoop(const bool isLoop);
	/* ���Ԑ؂ꂩ�H */
	bool IsTime();
	/* �o�ߎ��� */
	float GetElapsed();
	/* �o�ߎ��Ԃ̊��� */
	float GetRatioElapsed();
	/* �c�莞�� */
	float GetRemaining();
	/* �c�莞�Ԃ̊��� */
	float GetRatioRemaining();

private:
	float m_startTime;
	float m_elapsedTime;
	float m_maxTime;
	bool m_isUpdate;
	bool m_isLoop;
	bool m_isTime;
};

