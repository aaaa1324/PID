#include <stdio.h>
#include "Clock.h"
#include "ColorSensor.h"
using namespace ev3api;

class Calibration{

	public:
		rgb_raw_t cal_rgb_val;			// RGB値を一時的に格納する
		rgb_raw_t m_rgb_ave;			// RGB値の平均値を格納する
		float m_blue_r;					// 青色のR値
		float m_blue_g;					// 青色のG値
		float m_blue_b;					// 青色のB値
	
		Calibration();					// コンストラクタ
		int setCourse();				// コースを選択する
		int getAveRGB();            	// RGB値の平均をとる
		void setWhite();				// 白の閾値をセットする
		void setBlack();            	// 黒の閾値をセットする
		void setBlue();					// 青の閾値をセットする
		void findBlue();				// 青の検知をする
		void colorCalibration();		// カラーキャリブレーションを行う
  		ColorSensor colorSensor;
  		rgb_raw_t m_rgb_val;
  		rgb_raw_t ColorSensor_getRGB();
  		int g_black;
  		int g_white;


  		bool isValidateAveWhite();
		bool isValidateAveBlack();
		bool isValidateAveBlue();
		bool isValidateRawWhite();
		bool isValidateRawBlack();
		bool isValidateRawBlue();
		void color();
		char *judgeColor();


	private:  
  		Clock clock;

#ifndef MAKE_RASPIKE
  const int8_t pwm = (Motor::PWM_MAX) / 6;
#else
  const int8_t pwm = 60;
#endif
  const uint32_t duration = 5000*1000;
};