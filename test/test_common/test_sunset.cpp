#include <unity.h>

#include <ctime>
#include <string>
#include <sstream>
#include <math.h>
#include <sunset.h>

#define LATITUDE_AH     42.058102
#define LONGITUDE_AH    -87.984189
#define TIMEZONE_AH     -6

#define LATITUDE_CT     -33.9249
#define LONGITUDE_CT    18.4241
#define TIMEZONE_CT     2

#define LATITUDE_MC     55.7558
#define LONGITUDE_MC    37.617779
#define TIMEZONE_MC     3

#define LATITUDE_BA     -34.6037
#define LONGITUDE_BA    -58.3816
#define TIMEZONE_BA     -3

#define LATITUDE_PB     71.3875
#define LONGITUDE_PB    -156.4811
#define TIMEZONE_PB     -8

#define LATITUDE_US     -54.8019
#define LONGITUDE_US    -68.3030
#define TIMEZONE_US     -3

#define LATITUDE_ND     28.6139
#define LONGITUDE_ND    77.2090
#define TIMEZONE_ND     5.5

#define INVALID_NEG_TZ  -13
#define INVALID_POS_TZ  15

void test_SunsetTesting_ValidReturnValueSunrise() {
	SunSet sun;
	std::string expected("437.817");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidReturnValueSunset() {
	SunSet sun;

	std::string expected("1005");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidReturnValueCustomSunrise() {
	SunSet sun;
	std::string expected("437.817");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcCustomSunrise(90.833);

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidReturnValueCustomSunset() {
	SunSet sun;

	std::string expected("1005");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcCustomSunset(90.833);

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_MoonPhase() {
	SunSet sun;

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 1, 15);

	TEST_ASSERT_EQUAL(sun.moonPhase(1579097467), 20);
}

void test_SunsetTesting_NegativeLatitudeCapeTown() {
	SunSet sun;
	std::string expected("350.35");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_CT, LONGITUDE_CT, TIMEZONE_CT);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_PositiveLongitudeMoscow() {
	SunSet sun;
	std::string expected("530.075");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_MC, LONGITUDE_MC, TIMEZONE_MC);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_AllNegativeBuenosAires() {
	SunSet sun;
	std::string expected("356.131");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_BA, LONGITUDE_BA, TIMEZONE_BA);
	sun.setCurrentDate(2020, 1, 15);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidAstronomicalSunriseArlingtonHeights() {
	SunSet sun;
	std::string expected("138.945");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 5, 25);
	ss << sun.calcAstronomicalSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidAstronomicalSunsetArlingtonHeights() {
	SunSet sun;
	std::string expected("1280.07");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 5, 25);
	ss << sun.calcAstronomicalSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidNauticalSunriseArlingtonHeights() {
	SunSet sun;
	std::string expected("187.513");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 5, 25);
	ss << sun.calcNauticalSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidNauticalSunsetArlingtonHeights() {
	SunSet sun;
	std::string expected("1231.17");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 5, 25);
	ss << sun.calcNauticalSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidCivilSunriseArlingtonHeights() {
	SunSet sun;
	std::string expected("229.355");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 5, 25);
	ss << sun.calcCivilSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidCivilSunsetArlingtonHeights() {
	SunSet sun;
	std::string expected("1189.15");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	sun.setCurrentDate(2020, 5, 25);
	ss << sun.calcCivilSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidJulianDate() {
	SunSet sun;
	std::string expected("2.45899e+06");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_AH, LONGITUDE_AH, TIMEZONE_AH);
	double jd = sun.setCurrentDate(2020, 5, 25);
	ss << jd;

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

//void test_SunsetTesting_HighLatitudeTestSummer() {
//	SunSet sun;
//
//	sun.setPosition(LATITUDE_PB, LONGITUDE_PB, TIMEZONE_PB);
//	sun.setCurrentDate(2020, 6, 21);
//	double rval = sun.calcSunrise();
//
//	EXPECT_EQ(isnan(rval), true) << "Returned: " << rval;
//}

void test_SunsetTesting_HighLatitudeTestSpring() {
	SunSet sun;

	std::string expected("495.298");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);
	sun.setPosition(LATITUDE_PB, LONGITUDE_PB, TIMEZONE_PB);
	sun.setCurrentDate(2020, 3, 21);
	double rval = sun.calcSunrise();

	ss << rval;

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

//void test_SunsetTesting_HighLatitudeTestWinter() {
//	SunSet sun;
//
//	sun.setPosition(LATITUDE_PB, LONGITUDE_PB, TIMEZONE_PB);
//	sun.setCurrentDate(2020, 12, 21);
//	double rval = sun.calcSunrise();
//
//	EXPECT_EQ(isnan(rval), true) << "Returned: " << rval;
//}

void test_SunsetTesting_SouthernMostPointMay() {
	SunSet sun;
	std::string expected("576.845");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_US, LONGITUDE_US, TIMEZONE_US);
	sun.setCurrentDate(2020, 5, 26);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_SouthernMostPointWinter() {
	SunSet sun;
	std::string expected("294.969");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_US, LONGITUDE_US, TIMEZONE_US);
	sun.setCurrentDate(2020, 12, 26);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidReturnFractionalTimezoneSunrise() {
	SunSet sun;
	std::string expected("325.266");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_ND, LONGITUDE_ND, TIMEZONE_ND);
	sun.setCurrentDate(2020, 5, 26);
	ss << sun.calcSunrise();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_ValidReturnFractionalTimezoneSunset() {
	SunSet sun;

	std::string expected("1151.49");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_ND, LONGITUDE_ND, TIMEZONE_ND);
	sun.setCurrentDate(2020, 5, 26);
	ss << sun.calcSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_InvalidNegativeTimezone() {
	SunSet sun;

	std::string expected("821.487");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_ND, LONGITUDE_ND, INVALID_NEG_TZ);
	sun.setCurrentDate(2020, 5, 26);
	ss << sun.calcSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}

void test_SunsetTesting_InvalidPositiveTimezone() {
	SunSet sun;

	std::string expected("821.487");
	std::stringstream ss(std::stringstream::in | std::stringstream::out);

	sun.setPosition(LATITUDE_ND, LONGITUDE_ND, INVALID_POS_TZ);
	sun.setCurrentDate(2020, 5, 26);
	ss << sun.calcSunset();

	TEST_ASSERT_EQUAL_STRING(ss.str().c_str(), expected.c_str());
}


void run_test_sunset() {
	TEST_MESSAGE("Testing sunset");
	test_SunsetTesting_AllNegativeBuenosAires();
	test_SunsetTesting_HighLatitudeTestSpring();
	test_SunsetTesting_InvalidNegativeTimezone();
	test_SunsetTesting_InvalidPositiveTimezone();
	test_SunsetTesting_NegativeLatitudeCapeTown();
	test_SunsetTesting_PositiveLongitudeMoscow();
	test_SunsetTesting_SouthernMostPointMay();
	test_SunsetTesting_SouthernMostPointWinter();
	test_SunsetTesting_ValidAstronomicalSunriseArlingtonHeights();
	test_SunsetTesting_ValidAstronomicalSunsetArlingtonHeights();
	test_SunsetTesting_ValidCivilSunriseArlingtonHeights();
	test_SunsetTesting_ValidCivilSunsetArlingtonHeights();
	test_SunsetTesting_ValidJulianDate();
	test_SunsetTesting_ValidNauticalSunriseArlingtonHeights();
	test_SunsetTesting_ValidNauticalSunsetArlingtonHeights();
	test_SunsetTesting_ValidReturnFractionalTimezoneSunrise();
	test_SunsetTesting_ValidReturnFractionalTimezoneSunset();
	test_SunsetTesting_ValidReturnValueCustomSunrise();
	test_SunsetTesting_ValidReturnValueCustomSunset();
	test_SunsetTesting_ValidReturnValueSunrise();
	test_SunsetTesting_ValidReturnValueSunset();
	test_SunsetTesting_MoonPhase();
}
