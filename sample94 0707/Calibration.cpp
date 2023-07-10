#include "Calibration.h"

static const sensor_port_t
    color_sensor = EV3_PORT_2;

Calibration::Calibration():
  colorSensor(PORT_2) {
}


// 色の識別
enum detect_color
{
	WHITE = 0,
	BLACK,
	BLUE
} item;

// static const sensor_port_t
//     colorSensor = EV3_PORT_2;

int Calibration::getAveRGB(){
	
	uint16_t red = 0;
	uint16_t green = 0;
	uint16_t blue = 0;
	const int READ_N = 5;
	int count = 0;
	int rgb_ave = 0;

  //printf("cal_rgb_val.r:%d, cal_rgb_val.g:%d, cal_rgb_val.b:%d\n",cal_rgb_val.r,cal_rgb_val.g,cal_rgb_val.b);

	// 異常値を除くN個のRGB値を取得する
	for(int i = 0; count < READ_N; i++){
    colorSensor.getRawColor(m_rgb_val);
    //printf("count=%d\n", count);
		if(m_rgb_val.r <700 && m_rgb_val.g <700 && m_rgb_val.b <700){	// それぞれ0より大きく700より小さい値をとる
			if(m_rgb_val.r >=0 && m_rgb_val.g >=0 &&m_rgb_val.b >=0){
				count ++;
				red += m_rgb_val.r;
				green += m_rgb_val.g;
				blue += m_rgb_val.b;
			}
		}
	}

 // printf("red:%d, green:%d, blue:%d\n",red,green,blue);

	// red,green,blueそれぞれの平均値を計算する
	m_rgb_ave.r = (double)red / (double)count;
	m_rgb_ave.g = (double)green / (double)count;
	m_rgb_ave.b = (double)blue / (double)count;		

	// printf("red:%d, green:%d, blue:%d\n",m_rgb_ave.r,m_rgb_ave.g,m_rgb_ave.b);
	
	// red,green,blueすべて合わせた平均値を計算する
	rgb_ave = (m_rgb_ave.r + m_rgb_ave.g + m_rgb_ave.b) / 3;
    //printf("RGBの平均値:%d\n",rgb_ave);

	return rgb_ave;
}



// 白の閾値かどうか(平均値)
bool Calibration::isValidateAveWhite()
{
	return m_rgb_ave.r > 100 && m_rgb_ave.g > 100 && m_rgb_ave.b > 100;
}

// 黒の閾値かどうか(平均値)
bool Calibration::isValidateAveBlack()
{
	return m_rgb_ave.r < 30 && m_rgb_ave.g < 30 && m_rgb_ave.b < 30;
}

// 青の閾値かどうか(平均値)
bool Calibration::isValidateAveBlue()
{
	return m_rgb_ave.r < 70 && m_rgb_ave.g > 40 && m_rgb_ave.b > 100;
}

// 白の閾値かどうか(RAW値)
bool Calibration::isValidateRawWhite()
{
	return m_rgb_val.r > 100 && m_rgb_val.g > 100 && m_rgb_val.b > 70;
}

// 黒の閾値かどうか(RAW値)
bool Calibration::isValidateRawBlack()
{
	return m_rgb_val.r < 30 && m_rgb_val.g < 30 && m_rgb_val.b < 30;
	
}

// 青の閾値かどうか(RAW値)
bool Calibration::isValidateRawBlue()
{
	return m_rgb_val.b - m_rgb_val.r > 90 && m_rgb_val.g < 100;

}

/* カラーキャリブレーションを行う */
void Calibration::color()
{
	printf("colorCalibration start\n");
	// キャリブレーション用配列
	const int ColorDivision[] = {WHITE, BLACK};
	const int ColorDivisionSize = 2;
	// const int ColorDivision[] = {WHITE,BLACK,BLUE};

	int colorIndex = 0;
	while (colorIndex < ColorDivisionSize)
	{

		if (ColorDivision[colorIndex] == BLACK)
		{
			// 黒のRGB Raw値を取得
			ev3_color_sensor_get_rgb_raw(color_sensor, &m_rgb_val);
			if (isValidateRawBlack())
			{
				// 黒の範囲に収まっていない場合
				printf("black error\n");
				continue;
			}

			// 黒の閾値を設定
			g_black = m_rgb_val.r;

			// 黒の平均値
			getAveRGB();
			if (isValidateAveBlack())
			{
				// 黒の範囲に収まっている場合
				// 次の色へ
				colorIndex++;
			}
			else
			{
				printf("black error\n");
			}
		}
		else if (ColorDivision[colorIndex] == WHITE)
		{
			// 白のRGB Raw値を取得
			ev3_color_sensor_get_rgb_raw(color_sensor, &m_rgb_val);
			if (!isValidateRawWhite())
			{
				// 白の範囲に収まっていない場合
				printf("white error\n");
				continue;
			}

			// 白の閾値を設定
			g_white = m_rgb_val.r;

			// 白の平均値
			getAveRGB();
			if (isValidateAveWhite())
			{
				// 白の範囲に収まっている場合
				// 次の色へ
				colorIndex++;
			}
			else
			{
				printf("white error\n");
			}
		}


		tslp_tsk(100 * 1000U);
	}

	printf("colorCalibration End\n");
}


char *Calibration::judgeColor()
{
	if (isValidateRawBlue())
	{
		return "blue";
	}

	if (isValidateRawWhite())
	{
		return "white";
	}

	if (isValidateRawBlack())
	{
		return "black";
	}

	if (m_rgb_val.r > 100 && m_rgb_val.g < 45 && m_rgb_val.b < 45)
	{
		return "red";
	}

	if (m_rgb_val.r > 100 && m_rgb_val.g > 95 && m_rgb_val.b < 35)
	{
		return "yellow";
	}

	if (m_rgb_val.r < 35 && m_rgb_val.g > 60 && m_rgb_val.b < 40)
	{
		return "lightGreen";
	}

	if (m_rgb_val.r < 35 && m_rgb_val.g >= 25 && m_rgb_val.g <= 60 && m_rgb_val.b < 40)
	{
		return "darkGreen";
	}

	return "unknown";
}
