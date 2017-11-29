#include "pch.h"
#include "Random.h"


//class Random
Random::Random()
{
	std::random_device seedGen;
	engine.seed(seedGen());
}

Random& Random::get()
{
	static Random r;
	return r;
}

//MT�̏����l�ݒ�
void Random::InitializeSeed(int seed)
{
	get().engine.seed(seed);
}

//mt�G���W���{�̂��擾
std::default_random_engine& Random::getMT()
{
	return get().engine;
}

//���S����
unsigned int Random::RamdomDeviceValue()
{
	std::random_device r;
	return r();
}

//�ʏ��rand�擾
int Random::RandInt()
{
	return get().engine();
}

//0~max�w��擾(int)
int Random::RandInt(int max)
{
	std::uniform_int_distribution<int> uid(0, max);
	return uid(get().engine);
}

//min~max��Ԃ�(int)
int Random::RandInt(int min, int max)
{
	if (min > max) {
		int tmp = min;
		min = max;
		max = tmp;
	}
	std::uniform_int_distribution<int> uid(min, max);
	return uid(get().engine);
}

//1��-1��Ԃ��֐�
int Random::RandPM1()
{
	std::uniform_int_distribution<int> uid(0, 1);
	return uid(get().engine) * 2 - 1;
}

//0~1��Ԃ�(double)
double Random::Value()
{
	std::uniform_real_distribution<double> urd(0, 1);
	return urd(get().engine);
}

//0~max��Ԃ�(double)
double Random::Range(double max)
{
	if (max < 0.0) {
		return 0.0;
	}
	std::uniform_real_distribution<double> urd(0, max);
	return urd(get().engine);
}

//min~max��Ԃ�(double)
double Random::Range(double min, double max)
{
	if (min > max) {
		double tmp = min;
		min = max;
		max = tmp;
	}
	std::uniform_real_distribution<double> urd(min, max);
	return urd(get().engine);
}

//���K���z
double Random::RangeND(double mean, double sigma)
{
	std::normal_distribution<double> nd(mean, sigma);
	return nd(get().engine);
}

double Random::RangeND(double mean, double sigma, double limit)
{
	std::normal_distribution<double> nd(mean, sigma);
	double r = nd(get().engine);
	if (r < mean - limit)
		r = mean - limit;
	if (r > mean + limit)
		r = mean + limit;
	return r;
}

//���K���z ���ςƃ����W�w��
double Random::NormalDist(double ave, double range)
{
	std::normal_distribution<double> nd(ave, range / 3.0);
	double ret = nd(get().engine);
	if (ret < ave - range || ret > ave + range) {
		ret = Range(ave - range, ave + range);
	}
	return ret;
}

//���W�A���Ŏ擾
double Random::FRadRand()
{
	return Range(-3.14159265359, 3.14159265359);
}

double Random::FRadRand(double deg_min, double deg_max)
{
	return Range(deg_min, deg_max);
}

//���ar�̉~��̓_��Ԃ�
void Random::OnCircle(double r, double &x, double &y)
{
	double randang = FRadRand();
	x = r * cos(randang);
	y = r * sin(randang);
}

//���ar�̉~��̓_��Ԃ��@���_����̊p�x���Ԃ�
void Random::OnCircle(double r, double &x, double &y, double &ang)
{
	double randang = FRadRand();
	x = r * cos(randang);
	y = r * sin(randang);
	ang = randang;
}

//���ar�̉~�����̓_��Ԃ�
void Random::InCircle(double r, double &x, double &y)
{
	double randang = FRadRand();
	double rr = r * Value();
	x = rr * cos(randang);
	y = rr * sin(randang);
}

//���ar�̉~�����̓_��Ԃ��@���_����̊p�x���Ԃ�
void Random::InCircle(double r, double &x, double &y, double &ang)
{
	double randang = FRadRand();
	double rr = r * Value();
	x = rr * cos(randang);
	y = rr * sin(randang);
	ang = randang;
}

//���ar�̉~��̓_��Ԃ�
Vector2 Random::OnCircle(double r)
{
	Vector2 temp;
	double randang = FRadRand();
	temp.x = r * cos(randang);
	temp.y = r * sin(randang);
	return temp;
}

//���ar�̉~��̓_��Ԃ�
Vector2 Random::OnCircle(double r1, double r2)
{
	Vector2 temp;
	double r = Random::Range(r1, r2);
	double randang = FRadRand();
	temp.x = r * cos(randang);
	temp.y = r * sin(randang);
	return temp;
}

//���ar�̉~�����̓_��Ԃ�
Vector2 Random::InCircle(double r)
{
	Vector2 temp;
	double randang = FRadRand();
	double rr = r * Value();
	temp.x = rr * cos(randang);
	temp.y = rr * sin(randang);
	return temp;
}

//p�̊m����true��Ԃ�
bool Random::Bool(double p)
{
	return Random::Range(1.0) < p;
}

Vector3 Random::OnSphere()
{
	double z = Range(-1.0, 1.0);
	double theta = FRadRand();
	double minusZ = sqrt(1.0 - z * z);
	double x = minusZ * cos(theta);
	double y = minusZ * sin(theta);
	return Vector3(x, y, z);
}
