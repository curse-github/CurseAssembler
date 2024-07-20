#include "intelConstants.h"

uint8_t intToSIBScale(const unsigned int& num) {
    switch (num) {
        case 1:
            return INTEL_SIB_Scale_1;
        case 2:
            return INTEL_SIB_Scale_2;
        case 4:
            return INTEL_SIB_Scale_4;
        case 8:
            return INTEL_SIB_Scale_8;
        default:
            return INTEL_SIB_Scale_None;
    }
}
unsigned int SIBScaleToInt(const uint8_t& scale) {
    switch (scale) {
        case INTEL_SIB_Scale_1:
            return 1;
        case INTEL_SIB_Scale_2:
            return 2;
        case INTEL_SIB_Scale_4:
            return 4;
        case INTEL_SIB_Scale_8:
            return 8;
        default:
            return 0;
    }
}