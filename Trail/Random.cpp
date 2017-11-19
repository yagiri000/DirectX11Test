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

//MT‚Ì‰Šú’lİ’è
void Random::InitializeSeed(int seed)
{
	get().engine.seed(seed);
}

//mtƒGƒ“ƒWƒ“–{‘Ì‚ğæ“¾
std::default_random_engine& Random::getMT()
{
	return get().engine;
}

//Š®‘S—”
unsigned int Random::RamdomDeviceValue()
{
	std::random_device r;
	return r();
}

//’Êí‚Ìrandæ“¾
int Random::RandInt()
{
	return get().engine();
}

//0~maxw’èæ“¾(int)
int Random::RandInt(int max)
{
	std::uniform_int_distribution<int> uid(0, max);
	return uid(get().engine);
}

//min~max‚ğ•Ô‚·(int)
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

//1‚©-1‚ğ•Ô‚·ŠÖ”
int Random::RandPM1()
{
	std::uniform_int_distribution<int> uid(0, 1);
	return uid(get().engine) * 2 - 1;
}

//0~1‚ğ•Ô‚·(double)
double Random::Value()
{
	std::uniform_real_distribution<double> urd(0, 1);
	return urd(get().engine);
}

//0~max‚ğ•Ô‚·(double)
double Random::Range(double max)
{
	if (max < 0.0) {
		return 0.0;
	}
	std::uniform_real_distribution<double> urd(0, max);
	return urd(get().engine);
}

//min~max‚ğ•Ô‚·(double)
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

//³‹K•ª•z
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

//³‹K•ª•z •½‹Ï‚ÆƒŒƒ“ƒWw’è
double Random::NormalDist(double ave, double range)
{
	std::normal_distribution<double> nd(ave, range / 3.0);
	double ret = nd(get().engine);
	if (ret < ave - range || ret > ave + range) {
		ret = Range(ave - range, ave + range);
	}
	return ret;
}

//ƒ‰ƒWƒAƒ“‚Åæ“¾
double Random::FRadRand()
{
	return Range(-3.14159265359, 3.14159265359);
}

double Random::FRadRand(double deg_min, double deg_max)
{
	return Range(deg_min, deg_max);
}

//”¼Œar‚Ì‰~ã‚Ì“_‚ğ•Ô‚·
void Random::OnCircle(double r, double &x, double &y)
{
	double randang = FRadRand();
	x = r * cos(randang);
	y = r * sin(randang);
}

//”¼Œar‚Ì‰~ã‚Ì“_‚ğ•Ô‚·@Œ´“_‚©‚ç‚ÌŠp“x‚à•Ô‚·
void Random::OnCircle(double r, double &x, double &y, double &ang)
{
	double randang = FRadRand();
	x = r * cos(randang);
	y = r * sin(randang);
	ang = randang;
}

//”¼Œar‚Ì‰~“à•”‚Ì“_‚ğ•Ô‚·
void Random::InCircle(double r, double &x, double &y)
{
	double randang = FRadRand();
	double rr = r * Value();
	x = rr * cos(randang);
	y = rr * sin(randang);
}

//”¼Œar‚Ì‰~“à•”‚Ì“_‚ğ•Ô‚·@Œ´“_‚©‚ç‚ÌŠp“x‚à•Ô‚·
void Random::InCircle(double r, double &x, double &y, double &ang)
{
	double randang = FRadRand();
	double rr = r * Value();
	x = rr * cos(randang);
	y = rr * sin(randang);
	ang = randang;
}

//”¼Œar‚Ì‰~ã‚Ì“_‚ğ•Ô‚·
Vector2 Random::OnCircle(double r)
{
	Vector2 temp;
	double randang = FRadRand();
	temp.x = r * cos(randang);
	temp.y = r * sin(randang);
	return temp;
}

//”¼Œar‚Ì‰~ã‚Ì“_‚ğ•Ô‚·
Vector2 Random::OnCircle(double r1, double r2)
{
	Vector2 temp;
	double r = Random::Range(r1, r2);
	double randang = FRadRand();
	temp.x = r * cos(randang);
	temp.y = r * sin(randang);
	return temp;
}

//”¼Œar‚Ì‰~“à•”‚Ì“_‚ğ•Ô‚·
Vector2 Random::InCircle(double r)
{
	Vector2 temp;
	double randang = FRadRand();
	double rr = r * Value();
	temp.x = rr * cos(randang);
	temp.y = rr * sin(randang);
	return temp;
}

//p‚ÌŠm—¦‚Åtrue‚ğ•Ô‚·
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
