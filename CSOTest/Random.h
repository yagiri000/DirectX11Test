#pragma once
#include <iostream>
#include <random>
#include <DirectXMath.h>

using namespace DirectX::SimpleMath;

// TODO : ランダムなQuaternionを返す関数を実装

// シングルトンRandomクラス
class Random {
private:
	std::default_random_engine  engine;
	Random();

public:
	Random(const Random& r) = delete;
	Random& operator=(const Random& r) = delete;

	static Random& get();

	//MTの初期値をRandom_deviceを使用して設定
	static void InitializeSeed(int seed);

	//mtエンジン本体を取得
	static std::default_random_engine & getMT();

	//完全乱数
	static unsigned int RamdomDeviceValue();

	//通常のrand取得
	static int RandInt();

	//0~max指定取得(int)
	static int RandInt(int max);

	//min~maxを返す(int)
	static int RandInt(int min, int max);

	//1か-1を返す関数
	static int RandPM1();

	//0~1を返す(double)
	static double Value();

	//0~maxを返す(double)
	static double Range(double max);

	//min~maxを返す(double)
	static double Range(double min, double max);

	//正規分布
	static double RangeND(double mean, double sigma);

	//正規分布 平均とレンジ指定
	static double RangeND(double mean, double sigma, double limit);

	//正規分布 平均とレンジ指定
	static double NormalDist(double ave, double range);

	//ラジアンで取得
	static double FRadRand();

	//ラジアンで取得
	static double FRadRand(double deg_min, double deg_max);

	//半径rの円上の点を返す
	static void OnCircle(double r, double &x, double &y);

	//半径rの円上の点を返す　原点からの角度も返す
	static void OnCircle(double r, double &x, double &y, double &ang);

	//半径rの円内部の点を返す
	static void InCircle(double r, double &x, double &y);

	//半径rの円内部の点を返す　原点からの角度も返す
	static void InCircle(double r, double &x, double &y, double &ang);

	//半径rの円上の点を返す
	static Vector2 OnCircle(double r);

	//半径r2の円から半径r1の円の領域を引いた領域上の点を返す
	static Vector2 OnCircle(double r1, double r2);

	//半径rの円内部の点を返す
	static Vector2 InCircle(double r);
	
	//pの確率でtrueを返す
	static bool Bool(double p);

	// 直径1の球表面上の点を返す
	static Vector3 OnSphere();

};
