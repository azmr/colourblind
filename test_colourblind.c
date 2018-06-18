#define Assert(x) Test(x)
#define _CRT_SECURE_NO_WARNINGS
#include <sweet/sweet.h>

/* #define cbGAMMA_FAST */
#define cbIMPLEMENTATION
#include "colourblind.h"

int main()
{
	rgb_255 Black255 = {0,0,0}, Red255 = {255,0,0}, White255 = {255,255,255};
	rgb_norm BlackNorm = cbNorm(Black255), RedNorm = cbNorm(Red255), WhiteNorm = cbNorm(White255);

	TestGroup("Partial Steps")
	{
		TestVEqEps(cbRemoveGammaComponent(cbApplyGammaComponent(0.6)), 0.6, 0.01f, "%f");
		TestVEqEps(cbApplyGammaComponent(cbRemoveGammaComponent(0.6)), 0.6, 0.01f, "%f");
		TestVEqEps(cbDenormComponent(cbNormComponent(32)), 32, 1, "%u");
		TestVEqEps(cbNormComponent(cbDenormComponent(0.6)), 0.6, 0.01f, "%f");
	} EndTestGroup;

	TestGroup("Conversions")
	{
		TestGroup("Colourblind")
		{
			TestGroup("fixed points")
			{
				float R, G, B;
#define TEST(nopia, r1, g1, b1, r2, g2, b2) \
					TestGroup(#nopia) \
						R=r1, G=g1, B=b1; \
						nopia(&R, &G, &B); \
						TestVEqEps(R, r2, 0.0000001, "%lf"); \
						TestVEqEps(G, g2, 0.0000001, "%lf"); \
						TestVEqEps(B, b2, 0.0000001, "%lf"); \
					EndTestGroup

				TestGroup("Black")
					TEST(Protanopia,   0,0,0, 0,0,0);
					TEST(Deuteranopia, 0,0,0, 0,0,0);
					TEST(Tritanopia,   0,0,0, 0,0,0);
				EndTestGroup;

				TestGroup("White")
					TEST(Protanopia,   1.0,1.0,1.0, 1.0,1.0,1.0);
					TEST(Deuteranopia, 1.0,1.0,1.0, 1.0,1.0,1.0);
					TEST(Tritanopia,   1.0,1.0,1.0, 1.0,1.0,1.0);
				EndTestGroup;

				TestGroup("Blue")
					TEST(Protanopia,   0.0,0.0,1.0, 0.0,0.0,1.0);
					TEST(Deuteranopia, 0.0,0.0,1.0, 0.0,0.0,1.0);
				EndTestGroup;
				TestGroup("Red")
					TEST(Tritanopia,   1.0,0.0,0.0, 1.0,0.0,0.0);
				EndTestGroup;
#undef TEST
			} EndTestGroup;

			TestGroup("Known colours")
			{
				rgb_255 RGB;
#define TEST(nopia, r1, g1, b1, r2, g2, b2) \
					TestGroup(#nopia) \
						RGB.R=r1, RGB.G=g1, RGB.B=b1; \
						RGB = nopia(RGB); \
						TestVEqEps(RGB.R, r2, 1, "%X"); \
						TestVEqEps(RGB.G, g2, 1, "%X"); \
						TestVEqEps(RGB.B, b2, 1, "%X"); \
					EndTestGroup
				TestGroup("Burgundy")
					TEST(ProtanopiaRGB255Gamma,   0x88,0x00,0x27, 0x3A,0x3A,0x26);
					TEST(DeuteranopiaRGB255Gamma, 0x88,0x00,0x27, 0x51,0x51,0x1F);
					TEST(TritanopiaRGB255Gamma,   0x88,0x00,0x27, 0x87,0x08,0x08);
				EndTestGroup;

				TestGroup("Teal")
					TEST(ProtanopiaRGB255Gamma,   0x00,0xAA,0xAD, 0x9C,0x9C,0xAD);
					TEST(DeuteranopiaRGB255Gamma, 0x00,0xAA,0xAD, 0x8E,0x8E,0xAF);
				EndTestGroup;

				TestGroup("Pink")
					TEST(ProtanopiaRGB255Gamma,   0xEF,0x3F,0x6D, 0x78,0x78,0x6C);
					TEST(DeuteranopiaRGB255Gamma, 0xEF,0x3F,0x6D, 0x99,0x99,0x65);
					TEST(TritanopiaRGB255Gamma,   0xEF,0x3F,0x6D, 0xED,0x47,0x47);
				EndTestGroup;
#undef TEST
			} EndTestGroup;
		} EndTestGroup;

	} EndTestGroup;

	TestGroup("Contrast")
	{
		TestVEqEps(cbContrastRGB255(White255, Black255),    21.f,  0.005, "%f");
		TestVEqEps(cbContrastRGB255(White255, Red255),      4.f,   0.005, "%f");
		TestVEqEps(cbContrastRGB255(Black255, Red255),      5.25f, 0.005, "%f");
	}
	EndTestGroup;

	TestGroup("Contrast consistency")
	{
		TestVEqEps(cbContrastRGB(WhiteNorm, BlackNorm),    cbContrastRGB255(White255, Black255),    0.0,   "%f");
		TestVEqEps(cbContrastRGB(WhiteNorm, RedNorm),      cbContrastRGB255(White255, Red255),      0.005, "%f");
		TestVEqEps(cbContrastRGB(BlackNorm, RedNorm),      cbContrastRGB255(Black255, Red255),      0.005, "%f");
	}
	EndTestGroup;

	return PrintTestResults(1);
}

SWEET_END_TESTS;
