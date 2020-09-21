#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1ull << index;
    uint64_t temp = mask & number;
    temp >>= index;
    return temp;
}

bool checkSign (uint64_t number) {
    return !getBit(number, 63);
}

bool checkExp (uint64_t number) {
    // there is no zero in the exponent
    for (uint64_t i = 1; i < 12; ++ i)
        if (!getBit(number, 63 - i))
            return 0;

    return 1;
}

bool checkNormalized (uint64_t number) {
    int count = 0;
    for (uint64_t i = 1; i < 12; ++ i)
        if (getBit(number, 63 - i))
            count++;

    if (count < 11 && count > 0)
        return 1;

    return 0;
}

bool checkFractoin (uint64_t number) {
    // there is at least one unit in the fraction
    for (uint64_t i = 12; i < 64; ++ i)
        if (getBit(number, 63 - i))
            return 1;

    return 0;
}

/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf (uint64_t number) {
    return number == 0xFFF0000000000000;
}

bool checkForPlusNormal (uint64_t number) {
    if (checkSign(number) && checkNormalized(number)) 
        return 1;

    return 0;
}

bool checkForMinusNormal (uint64_t number) {
    if (!checkSign(number) && checkNormalized(number)) 
        return 1;
    
    return 0;
}

bool checkForPlusDenormal (uint64_t number) {
    if (checkSign(number) && !checkExp(number))
        return checkFractoin(number);

    return 0;
}

bool checkForMinusDenormal (uint64_t number) {
    if (!checkSign(number) && !checkExp(number))
        return checkFractoin(number);

    return 0;
}

bool checkForSignalingNan (uint64_t number) {
    if (checkExp(number))
        if (!getBit(number, 51) && checkFractoin(number))
            return 1;

    return 0;
}

bool checkForQuietNan (uint64_t number) {
    if (checkExp(number))
        return getBit(number, 51);

    return 0;
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}

