/* ISC License Copyright (c) 2018 Andrew Reece
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/* TODO:
 * - Achromatopsia
 * - Add working ColourDifference with Luv
 */

#ifdef I_PREFER_THE_AMERICAN_SPELLING_OF_COLOR_BECAUSE_I_AM_SILLY
#define Colo_rblind       Colorblind
#define Colo_rblind255    Colorblind255
#define Colo_rblindRGB255 ColorblindRGB255
#define Colo_rblindRGB    ColorblindRGB
#else/*I_PREFER_THE_AMERICAN_SPELLING_OF_COLOR_BECAUSE_I_AM_SILLY*/
#define Colo_rblind       Colourblind
#define Colo_rblind255    Colourblind255
#define Colo_rblindRGB255 ColourblindRGB255
#define Colo_rblindRGB    ColourblindRGB
#endif/*I_PREFER_THE_AMERICAN_SPELLING_OF_COLOR_BECAUSE_I_AM_SILLY*/

/******************************************************************************
 * Types
 *******/
typedef struct cb_rgb_255 cb_rgb_255;
typedef struct cb_rgb cb_rgb;

typedef enum cb_impairment cb_impairment;
typedef enum cb_guideline cb_guideline;

/******************************************************************************
 * Constants
 ***********/
char *cbImpairmentStrings[];
char *cbGuidelineStrings[];
float cbGuidelineScores[];

/******************************************************************************
 * Function Prototypes
 *********************/
/* Conversions from normal rgb values to their colourblind equivalent for simulating different impairments.
 * The conversion depends on the cone that you are simulating as non-functioning.
 * All pointers are used as in and out variables */

/* Missing the first/long-wave/red cone - Red-Green colourblind */
void       Protanopia(float *Red, float *Green, float *Blue);
void       Protanopia255(unsigned char *R, unsigned char *G, unsigned char *B);
cb_rgb     ProtanopiaRGB(cb_rgb RGB);
cb_rgb_255 ProtanopiaRGB255(cb_rgb_255 RGB);
cb_rgb_255 ProtanopiaRGB255Gamma(cb_rgb_255 RGB);

/* Missing the second/medium-wave/green cone - Red-Green colourblind 
 * This is the most common form of colourblindness */
void       Deuteranopia(float *Red, float *Green, float *Blue);
void       Deuteranopia255(unsigned char *R, unsigned char *G, unsigned char *B);
cb_rgb     DeuteranopiaRGB(cb_rgb RGB);
cb_rgb_255 DeuteranopiaRGB255(cb_rgb_255 RGB);
cb_rgb_255 DeuteranopiaRGB255Gamma(cb_rgb_255 RGB);

/* Missing the third/short-wave/blue cone - Blue-Yellow colourblind */
void       Tritanopia(float *Red, float *Green, float *Blue);
void       Tritanopia255(unsigned char *R, unsigned char *G, unsigned char *B);
cb_rgb     TritanopiaRGB(cb_rgb RGB);
cb_rgb_255 TritanopiaRGB255(cb_rgb_255 RGB);
cb_rgb_255 TritanopiaRGB255Gamma(cb_rgb_255 RGB);

/* Applies the above impairment simulation (or no-op) based on the enum value given */
void       Colo_rblind(cb_impairment Impairment, float *R, float *G, float *B);
void       Colo_rblind255(cb_impairment Impairment, unsigned char *R, unsigned char *G, unsigned char *B);
cb_rgb     Colo_rblindRGB(cb_impairment Impairment, cb_rgb RGB);
cb_rgb_255 Colo_rblindRGB255(cb_impairment Impairment, cb_rgb_255 RGB);

/* WCAG-defined contrast: (L_H+0.5) / (L_L+0.5) */
/* Results range from 1 (the same colour) to 21 (white with black) */
float cbContrast(float RA, float GA, float BA, float RB, float GB, float BB);
float cbContrast255(unsigned char RA, unsigned char GA, unsigned char BA, unsigned char RB, unsigned char GB, unsigned char BB);
float cbContrastRGB(cb_rgb A, cb_rgb B);
float cbContrastRGB255(cb_rgb_255 A, cb_rgb_255 B);

/* ISO 9241-3 contrast modulation: (L_H-L_L) / (L_H+L_L) */
float cbContrastModulation(float RA, float GA, float BA, float RB, float GB, float BB);
float cbContrastModulation255(unsigned char RA, unsigned char GA, unsigned char BA, unsigned char RB, unsigned char GB, unsigned char BB);
float cbContrastModulationRGB(cb_rgb A, cb_rgb B);
float cbContrastModulationRGB255(cb_rgb_255 A, cb_rgb_255 B);

/* ISO 9241-3 contrast ratio: L_H / L_L (divides by zero for pure black) */
float cbContrastRatio(float RA, float GA, float BA, float RB, float GB, float BB);
float cbContrastRatio255(unsigned char RA, unsigned char GA, unsigned char BA, unsigned char RB, unsigned char GB, unsigned char BB);
float cbContrastRatioRGB(cb_rgb A, cb_rgb B);
float cbContrastRatioRGB255(cb_rgb_255 A, cb_rgb_255 B);

/* Gives a 'lightness' value for comparing colours */
float cbLuminance(float R, float G, float B);
float cbLuminance255(unsigned char R, unsigned char G, unsigned char B);
float cbLuminanceRGB(cb_rgb RGB);
float cbLuminanceRGB255(cb_rgb_255 RGB);

/* Convert between 0-255 and 0-1 */
cb_rgb     cbNorm(cb_rgb_255 RGB);
cb_rgb_255 cbDenorm(cb_rgb RGB);

/* Convert from Linear to sRGB */
void   cbApplyGamma(float *R, float *G, float *B);
cb_rgb cbApplyGammaRGB(cb_rgb RGB);

/* Convert from sRGB to Linear */
void   cbRemoveGamma(float *R, float *G, float *B);
cb_rgb cbRemoveGammaRGB(cb_rgb RGB);

#ifdef cbIMPLEMENTATION
typedef struct cb_rgb_255 {
    unsigned char R; /* Red */
    unsigned char G; /* Green */
    unsigned char B; /* Blue */
} cb_rgb_255;
typedef struct cb_rgb {
    float R; /* Red */
    float G; /* Green */
    float B; /* Blue */
} cb_rgb;
typedef enum cb_impairment {
    cbUnimpaired, cbProtanopia, cbDeuteranopia, cbTritanopia,
    cbImpairmentCount,
    cbRedGreenDim = cbProtanopia, cbRedGreen     = cbDeuteranopia, cbBlueYellow  = cbTritanopia,
    cbMissingRed  = cbProtanopia, cbMissingGreen = cbDeuteranopia, cbMissingBlue = cbTritanopia,
} cb_impairment;

char *cbImpairmentStrings[] =
{ "Unimpaired", "Protanopia", "Deuteranopia", "Tritanopia" };

#define COL_GUIDELINES \
    COL_GUIDELINE(ISO9241_3, ContrastRatio,      Pass, >=, 3.0) \
    COL_GUIDELINE(WCAG,      Contrast,        AALarge, >=, 4.0) \
    COL_GUIDELINE(WCAG,      Contrast,       AAALarge, >=, 4.5) \
    COL_GUIDELINE(WCAG,      Contrast,             AA, >=, 4.5) \
    COL_GUIDELINE(WCAG,      Contrast,            AAA, >=, 7.0) \
    COL_GUIDELINE(ISO9241_3, ContrastModulation, Pass, >=, 0.5) \

#define COL_GUIDELINE(source, testname, rating, comparison, value) cb## source ##_## testname ##_## rating,
typedef enum cb_guideline    { COL_GUIDELINES cgGuidelineCount } cb_guideline;
#undef COL_GUIDELINE
#define COL_GUIDELINE(source, testname, rating, comparison, value) #source " " #testname " " #rating,
char *cbGuidelineStrings[] = { COL_GUIDELINES };
#undef COL_GUIDELINE
#define COL_GUIDELINE(source, testname, rating, comparison, value) value,
float cbGuidelineScores[]  = { COL_GUIDELINES };
#undef COL_GUIDELINE


/* assumes already normalized to 0-1 */
#ifdef cbGAMMA_FASTER
#ifndef cbSQRT
#include <math.h> /* sqrt, pow */
#define cbSQRT sqrt
#endif/*cbSQRT*/

#define cbApplyGammaComponent(X)  ((float)cbSQRT(X))
#define cbRemoveGammaComponent(X) ((X) * (X))

#else /* cbGAMMA_FASTER */
#ifndef cbPOW
#include <math.h> /* sqrt, pow */
#define cbPOW pow
#endif/*cbPOW*/

#ifdef cbGAMMA_FAST
#define cbApplyGammaComponent(X)  ((float)cbPOW(X, 0.454545454545454545454545454545454545454545))
#define cbRemoveGammaComponent(X) ((float)cbPOW(X, 2.2))

#else /* cbGAMMA_FAST */
#define cbApplyGammaComponent(X) \
    ((float)(X > 0.00313080495356037151702786377709   ? \
        1.055 * cbPOW((X), 0.4166666666) - 0.055      : \
        X * 12.92))
#define cbRemoveGammaComponent(X) \
    ((float)(X > 0.04045                ? \
        cbPOW((X + 0.055) / 1.055, 2.4) : \
        X / 12.92))
#endif /* cbGAMMA_FAST */
#endif /* cbGAMMA_FASTER */

#define cbNormComponent(X)   ((float)(X) / 255.f)
#define cbDenormComponent(X) ((unsigned char)((X) * 255.f + 0.5f))

#define cbGamma(dir) \
void dir##Gamma(float *R, float *G, float *B) { \
    float r = *R, g = *G, b = *B; \
    *R=dir##GammaComponent(r), *G=dir##GammaComponent(g), *B=dir##GammaComponent(b); \
} \
cb_rgb dir##GammaRGB(cb_rgb RGB) { \
    cb_rgb Result = \
    { dir##GammaComponent(RGB.R), dir##GammaComponent(RGB.G), dir##GammaComponent(RGB.B) }; \
    return Result; \
}
cbGamma(cbApply)
cbGamma(cbRemove)
#undef cbGamma

#define cbDomain(name, in, out) \
out name(in RGB) { \
    out Result = \
    { name##Component(RGB.R), name##Component(RGB.G), name##Component(RGB.B) }; \
    return Result; \
}
cbDomain(cbNorm, cb_rgb_255, cb_rgb)
cbDomain(cbDenorm, cb_rgb, cb_rgb_255)
#undef cbDomain

/* Luminances */
float cbLuminance(float R, float G, float B) {
    R = cbRemoveGammaComponent(R);
    G = cbRemoveGammaComponent(G);
    B = cbRemoveGammaComponent(B);
    float Result = 0.2126f*R + 0.7152f*G + 0.0722f*B;
    return Result;
}
float cbLuminance255(unsigned char R, unsigned char G, unsigned char B)
{ return cbLuminance(cbNormComponent(R), cbNormComponent(G), cbNormComponent(B)); }
float cbLuminanceRGB(cb_rgb RGB)
{ return cbLuminance(RGB.R, RGB.G, RGB.B); }
float cbLuminanceRGB255(cb_rgb_255 RGB)
{ return cbLuminance(cbNormComponent(RGB.R), cbNormComponent(RGB.G), cbNormComponent(RGB.B)); }

/* prevents division by 0 */
float cbContrast(float RA, float GA, float BA, float RB, float GB, float BB) {
    float LumA = cbLuminance(RA, GA, BA);
    float LumB = cbLuminance(RB, GB, BB);
    float High = LumA, Low = LumB;
    if(High < Low)
    { High = LumB, Low = LumA; }

    /* from http://www.w3.org/TR/2008/REC-WCAG20-20081211/#contrast-ratiodef */
    float Ratio = (High + 0.05f) / (Low  + 0.05f);
    return Ratio;
}

float cbContrastRatio(float RA, float GA, float BA, float RB, float GB, float BB) {
    float LumA = cbLuminance(RA, GA, BA);
    float LumB = cbLuminance(RB, GB, BB);
    float High = LumA, Low = LumB;
    if(High < Low)
    { High = LumB, Low = LumA; }

    float Ratio = High / Low;
    return Ratio;
}

float cbContrastModulation(float RA, float GA, float BA, float RB, float GB, float BB) {
    float LumA = cbLuminance(RA, GA, BA);
    float LumB = cbLuminance(RB, GB, BB);
    float High = LumA, Low  = LumB;
    if (High == Low) /* for black */
    { return 0; }
    else if (High < Low) {
        High = LumB;
        Low  = LumA;
    }

    float Top    = High - Low;
    float Bottom = High + Low;
    return Top/Bottom;
}

#define cbOTHER_VERSIONS(fn) \
float fn##255(unsigned char RA, unsigned char GA, unsigned char BA, unsigned char RB, unsigned char GB, unsigned char BB) \
{ return fn(cbNormComponent(RA), cbNormComponent(GA), cbNormComponent(BA), \
            cbNormComponent(RB), cbNormComponent(GB), cbNormComponent(BB)); } \
float fn##RGB(cb_rgb A, cb_rgb B) \
{ return fn(A.R, A.G, A.B, B.R, B.G, B.B); } \
float fn##RGB255(cb_rgb_255 A, cb_rgb_255 B) \
{ return fn(cbNormComponent(A.R), cbNormComponent(A.G), cbNormComponent(A.B), \
            cbNormComponent(B.R), cbNormComponent(B.G), cbNormComponent(B.B)); }

cbOTHER_VERSIONS(cbContrast)
cbOTHER_VERSIONS(cbContrastRatio)
cbOTHER_VERSIONS(cbContrastModulation)
#undef cbOTHER_VERSIONS

/******************************************************************************
 * Colourblindness
 *****************/
/* http://ixora.io/projects/colorblindness/color-blindness-simulation-research/ */

void Protanopia(float *Red, float *Green, float *Blue) {
    float R = *Red, G = *Green, B = *Blue;
    *Red   =  0.17055699213417f*R + 0.82944301379913f*G + 2.91188E-9f *B;
    *Green =  0.17055699092998f*R + 0.82944300785005f*G - 5.98679E-10f*B;
    *Blue  = -0.00451714424166f*R + 0.00451714427397f*G +              B;
}

void Deuteranopia(float *Red, float *Green, float *Blue) {
    float R = *Red, G = *Green, B = *Blue;
    *Red   =  0.33066007266046f*R + 0.66933992517563f*G + 3.559314E-9f*B;
    *Green =  0.33066007387760f*R + 0.66933992719147f*G - 1.758327E-9f*B;
    *Blue  = -0.02785538261323f*R + 0.02785538252318f*G +              B;
}

void Tritanopia(float *Red, float *Green, float *Blue) {
    float R = *Red, G = *Green, B = *Blue;
    *Red   =              R + 0.12739886310880f*G - 0.12739886341072f*B;
    *Green = -4.486E-11f *R + 0.87390929928361f*G + 0.12609070101523f*B;
    *Blue  =  3.1113E-10f*R + 0.87390929725848f*G + 0.12609070067115f*B;
}

/* assumes value in 0-1 */
#define cbNOPIA(nopia) \
cb_rgb nopia ##RGB(cb_rgb RGB) { \
    nopia(&RGB.R, &RGB.G, &RGB.B); \
    return RGB; \
} \
/* take and return rgb as 0-255 */ \
cb_rgb_255 nopia ##RGB255(cb_rgb_255 RGB) {\
    cb_rgb RGBNorm = cbNorm(RGB);\
    nopia(&RGBNorm.R, &RGBNorm.G, &RGBNorm.B); \
    cb_rgb_255 Result   = cbDenorm(RGBNorm);\
    return Result;\
} \
/* take and return gamma-corrected rgb as 0-255 */ \
cb_rgb_255 nopia ##RGB255Gamma(cb_rgb_255 RGB) {\
    cb_rgb RGBNorm = cbRemoveGammaRGB(cbNorm(RGB)); \
    nopia(&RGBNorm.R, &RGBNorm.G, &RGBNorm.B); \
    cb_rgb_255 Result = cbDenorm(cbApplyGammaRGB(RGBNorm)); \
    return Result;\
} \
void nopia ##255(unsigned char *R, unsigned char *G, unsigned char *B) { \
    float Rf = cbNormComponent(*R), Gf = cbNormComponent(*G), Bf = cbNormComponent(*B); \
    nopia(&Rf, &Gf, &Bf); \
    *R = cbDenormComponent(Rf); \
    *G = cbDenormComponent(Gf); \
    *B = cbDenormComponent(Bf); \
}

cbNOPIA(Protanopia)
cbNOPIA(Deuteranopia)
cbNOPIA(Tritanopia)
#undef cbNOPIA

cb_rgb Colo_rblindRGB(cb_impairment Impairment, cb_rgb RGB) {
    switch(Impairment) {
        case cbProtanopia:   return ProtanopiaRGB(  RGB);
        case cbDeuteranopia: return DeuteranopiaRGB(RGB);
        case cbTritanopia:   return TritanopiaRGB(  RGB);
        default:             return RGB;
    }
}
cb_rgb_255 Colo_rblindRGB255(cb_impairment Impairment, cb_rgb_255 RGB) {
    switch(Impairment) {
        case cbProtanopia:   return ProtanopiaRGB255(  RGB);
        case cbDeuteranopia: return DeuteranopiaRGB255(RGB);
        case cbTritanopia:   return TritanopiaRGB255(  RGB);
        default:             return RGB;
    }
}
void Colo_rblind255(cb_impairment Impairment, unsigned char *R, unsigned char *G, unsigned char *B) {
    switch(Impairment) {
        case cbProtanopia:   Protanopia255(  R, G, B); break;
        case cbDeuteranopia: Deuteranopia255(R, G, B); break;
        case cbTritanopia:   Tritanopia255(  R, G, B);
    }
}
void Colo_rblind(cb_impairment Impairment, float *R, float *G, float *B) {
    switch(Impairment) {
        case cbProtanopia:   Protanopia(  R, G, B); break;
        case cbDeuteranopia: Deuteranopia(R, G, B); break;
        case cbTritanopia:   Tritanopia(  R, G, B);
    }
}

#endif/* cbIMPLEMENTATION */
