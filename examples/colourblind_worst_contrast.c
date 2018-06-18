#include <stdio.h>
#include "../colourblind.h"
#define cbIMPLEMENTATION
#include "../colourblind.h"

int main() {
    cb_rgb Purple = { 0.8f, 0.1f, 0.6f };
    cb_rgb Green  = { 0.1f, 0.8f, 0.2f };

    float WorstImpairmentContrast = 21.f;         // maximum possible contrast for the test used
    cb_impairment WorstImpairment = cbUnimpaired; // == 0

    for(cb_impairment Impairment = cbUnimpaired; Impairment < cbImpairmentCount; ++Impairment) {
        cb_rgb PurpleImpaired = ColourblindRGB(Impairment, Purple);
        cb_rgb GreenImpaired  = ColourblindRGB(Impairment, Green);

        printf("The %s equivalent of Purple is { %.2f, %.2f, %.2f }\n",
                cbImpairmentStrings[Impairment], PurpleImpaired.R, PurpleImpaired.G, PurpleImpaired.B);
        printf("The %s equivalent of Green is  { %.2f, %.2f, %.2f }\n",
                cbImpairmentStrings[Impairment], GreenImpaired.R, GreenImpaired.G, GreenImpaired.B);

        float ImpairmentContrast = cbContrastRGB(PurpleImpaired, GreenImpaired);
        printf("The %s Purple-Green contrast is %4f:1\n\n",
                cbImpairmentStrings[Impairment], ImpairmentContrast);

        if(ImpairmentContrast < WorstImpairmentContrast) {
            WorstImpairment = Impairment;
            WorstImpairmentContrast = ImpairmentContrast;
        }
    }

    char *PassFail = WorstImpairmentContrast >= cbGuidelineScores[cbWCAG_Contrast_AA]
                   ? "passes"
                   : "fails";
    printf("\nThe contrast was worst in the %s condition, with a value of %4f:1\n"
           "This %s the WCAG AA requirement.",
           cbImpairmentStrings[WorstImpairment], WorstImpairmentContrast, PassFail);

    return 0;
}

