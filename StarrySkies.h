#pragma once

#include "common.h"

enum eStarSides : uint8_t {
    SSide_Left = 0,
    SSide_Right,
    SSide_Front,
    SSide_Back,
    SSide_Up,

    SSidesCount
};

#define AMOUNT_OF_SIDESTARS 100

class CStarrySkies {
public:
    static void InjectHooks();

    static float RandomIt(float min, float max);
    static float ClampFloat(float value, float min, float max);
    static void InitializeThoseStars();
    static void StarrySkies();

public:
    static inline float StarCoorsX[SSidesCount][AMOUNT_OF_SIDESTARS];
    static inline float StarCoorsY[SSidesCount][AMOUNT_OF_SIDESTARS];
    static inline float StarSizes[SSidesCount][AMOUNT_OF_SIDESTARS];

    static inline float fSmallStars;
    static inline float fMiddleStars;
    static inline float fBiggestStars;
    static inline float fBiggestStarsSpawnChance;

    static inline CVector PositionsTable[SSidesCount] =
            {
                    { 100.0f,  0.0f,   10.0f}, // Left
                    {-100.0f,  0.0f,   10.0f}, // Right
                    {   0.0f,  100.0f, 10.0f}, // Front
                    {   0.0f, -100.0f, 10.0f}, // Back
                    {   0.0f,  0.0f,   95.0f}, // Up
            };
};
