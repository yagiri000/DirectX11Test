#pragma once
#include <iostream>
#include <random>
#include <DirectXMath.h>

using namespace DirectX::SimpleMath;

// TODO : �����_����Quaternion��Ԃ��֐�������

// �V���O���g��Random�N���X
class Random {
private:
	std::default_random_engine  engine;
	Random();

public:
	Random(const Random& r) = delete;
	Random& operator=(const Random& r) = delete;

	static Random& get();

	//MT�̏����l��Random_device���g�p���Đݒ�
	static void InitializeSeed(int seed);

	//mt�G���W���{�̂��擾
	static std::default_random_engine & getMT();

	//���S����
	static unsigned int RamdomDeviceValue();

	//�ʏ��rand�擾
	static int RandInt();

	//0~max�w��擾(int)
	static int RandInt(int max);

	//min~max��Ԃ�(int)
	static int RandInt(int min, int max);

	//1��-1��Ԃ��֐�
	static int RandPM1();

	//0~1��Ԃ�(double)
	static double Value();

	//0~max��Ԃ�(double)
	static double Range(double max);

	//min~max��Ԃ�(double)
	static double Range(double min, double max);

	//���K���z
	static double RangeND(double mean, double sigma);

	//���K���z ���ςƃ����W�w��
	static double RangeND(double mean, double sigma, double limit);

	//���K���z ���ςƃ����W�w��
	static double NormalDist(double ave, double range);

	//���W�A���Ŏ擾
	static double FRadRand();

	//���W�A���Ŏ擾
	static double FRadRand(double deg_min, double deg_max);

	//���ar�̉~��̓_��Ԃ�
	static void OnCircle(double r, double &x, double &y);

	//���ar�̉~��̓_��Ԃ��@���_����̊p�x���Ԃ�
	static void OnCircle(double r, double &x, double &y, double &ang);

	//���ar�̉~�����̓_��Ԃ�
	static void InCircle(double r, double &x, double &y);

	//���ar�̉~�����̓_��Ԃ��@���_����̊p�x���Ԃ�
	static void InCircle(double r, double &x, double &y, double &ang);

	//���ar�̉~��̓_��Ԃ�
	static Vector2 OnCircle(double r);

	//���ar2�̉~���甼�ar1�̉~�̗̈���������̈��̓_��Ԃ�
	static Vector2 OnCircle(double r1, double r2);

	//���ar�̉~�����̓_��Ԃ�
	static Vector2 InCircle(double r);
	
	//p�̊m����true��Ԃ�
	static bool Bool(double p);

	// ���a1�̋��\�ʏ�̓_��Ԃ�
	static Vector3 OnSphere();

};
