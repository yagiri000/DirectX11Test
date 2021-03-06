#include "pch.h"
#include "Random.h"

double Random::PI = 3.14159265359;

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

//MTの初期値設定
void Random::InitializeSeed(int seed)
{
	get().engine.seed(seed);
}

//mtエンジン本体を取得
std::default_random_engine& Random::getMT()
{
	return get().engine;
}

//完全乱数
unsigned int Random::RamdomDeviceValue()
{
	std::random_device r;
	return r();
}

//通常のrand取得
int Random::RandInt()
{
	return get().engine();
}

//0~max指定取得(int)
int Random::RandInt(int max)
{
	std::uniform_int_distribution<int> uid(0, max);
	return uid(get().engine);
}

//min~maxを返す(int)
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

//1か-1を返す関数
int Random::RandPM1()
{
	std::uniform_int_distribution<int> uid(0, 1);
	return uid(get().engine) * 2 - 1;
}

//0~1を返す(double)
double Random::Value()
{
	std::uniform_real_distribution<double> urd(0, 1);
	return urd(get().engine);
}

//0~maxを返す(double)
double Random::Range(double max)
{
	if (max < 0.0) {
		return 0.0;
	}
	std::uniform_real_distribution<double> urd(0, max);
	return urd(get().engine);
}

//min~maxを返す(double)
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

//正規分布
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

//正規分布 平均とレンジ指定
double Random::NormalDist(double ave, double range)
{
	std::normal_distribution<double> nd(ave, range / 3.0);
	double ret = nd(get().engine);
	if (ret < ave - range || ret > ave + range) {
		ret = Range(ave - range, ave + range);
	}
	return ret;
}

//半径rの円上の点を返す
void Random::OnCircle(double r, double &x, double &y)
{
	double randang = Range(-PI, PI);
	x = r * cos(randang);
	y = r * sin(randang);
}

//半径rの円上の点を返す　原点からの角度も返す
void Random::OnCircle(double r, double &x, double &y, double &ang)
{
	double randang = Range(-PI, PI);
	x = r * cos(randang);
	y = r * sin(randang);
	ang = randang;
}

//半径rの円内部の点を返す
void Random::InCircle(double r, double &x, double &y)
{
	double randang = Range(-PI, PI);
	double rr = r * Value();
	x = rr * cos(randang);
	y = rr * sin(randang);
}

//半径rの円内部の点を返す　原点からの角度も返す
void Random::InCircle(double r, double &x, double &y, double &ang)
{
	double randang = Range(-PI, PI);
	double rr = r * Value();
	x = rr * cos(randang);
	y = rr * sin(randang);
	ang = randang;
}

//半径rの円上の点を返す
Vector2 Random::OnCircle(double r)
{
	Vector2 temp;
	double randang = Range(-PI, PI);
	temp.x = r * cos(randang);
	temp.y = r * sin(randang);
	return temp;
}

//半径rの円上の点を返す
Vector2 Random::OnCircle(double r1, double r2)
{
	Vector2 temp;
	double r = Random::Range(r1, r2);
	double randang = Range(-PI, PI);
	temp.x = r * cos(randang);
	temp.y = r * sin(randang);
	return temp;
}

//半径rの円内部の点を返す
Vector2 Random::InCircle(double r)
{
	Vector2 temp;
	double randang = Range(-PI, PI);
	double rr = r * Value();
	temp.x = rr * cos(randang);
	temp.y = rr * sin(randang);
	return temp;
}

//pの確率でtrueを返す
bool Random::Bool(double p)
{
	return Random::Range(1.0) < p;
}

Vector3 Random::OnSphere()
{
	double z = Range(-1.0, 1.0);
	double theta = Range(-PI, PI);
	double minusZ = sqrt(1.0 - z * z);
	double x = minusZ * cos(theta);
	double y = minusZ * sin(theta);
	return Vector3(x, y, z);
}

Quaternion Random::Rotation()
{
	return Quaternion(Range(-1.0, 1.0), Range(-1.0, 1.0), Range(-1.0, 1.0), Range(-1.0, 1.0));
}
