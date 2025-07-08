#include "StarrySkies.h"

inline float CStarrySkies::RandomIt(float min, float max)
{
    return (((float)rand()) / (float)RAND_MAX) * (max - min) + min;
}

inline float CStarrySkies::ClampFloat(float value, float min, float max)
{
    if(value > max) return max;
    if(value < min) return min;
    return value;
}

void CStarrySkies::InitializeThoseStars()
{
    srand(0xBEEF);

    for (int side = 0; side < SSidesCount; ++side)
    {
        for (int i = 0; i < AMOUNT_OF_SIDESTARS; ++i)
        {
            StarCoorsX[side][i] = 95.0f * RandomIt(-1.0f, 1.0f);

            if(side == SSide_Up) StarCoorsY[side][i] = 95.0f * RandomIt(-1.0f, 1.0f);
            else StarCoorsY[side][i] = 95.0f * RandomIt(-0.35f, 1.0f);

            if(RandomIt(0.0f, 1.0f) > fBiggestStarsSpawnChance) StarSizes[side][i] = 0.8f * RandomIt(fSmallStars, fBiggestStars);
            else StarSizes[side][i] = 0.8f * RandomIt(fSmallStars, fMiddleStars);
        }
    }

    srand(time(NULL));
}

uint8 CalculateColorWithBalanceStarry(uint8 blue, float colorBalance) {
    return lerp<uint8>(blue, 0u, colorBalance);
}

void CStarrySkies::StarrySkies()
{
    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));

    CVector ScreenPos, WorldPos, WorldStarPos;
    CVector& CamPos = TheCamera.GetPosition();
    float SZ, SZX, SZY;

    constexpr auto LOGO_VISIBLE_FROM_HRS  = 22u;
    const auto time = CClock::GetGameClockHours() == LOGO_VISIBLE_FROM_HRS
                      ? CClock::GetGameClockMinutes()
                      : 60u - CClock::GetGameClockMinutes();

    const auto colorB  = 255u * time / 60u;
    for (int side = 0; side < SSidesCount; ++side)
    {
        WorldPos = CStarrySkies::PositionsTable[side] + CamPos;
        for (int i = 0; i < AMOUNT_OF_SIDESTARS; ++i)
        {
            WorldStarPos = WorldPos;
            SZ = CStarrySkies::StarSizes[side][i];
            switch (side)
            {
                case SSide_Left:
                case SSide_Right:
                    WorldStarPos.y -= CStarrySkies::StarCoorsX[side][i];
                    WorldStarPos.z += CStarrySkies::StarCoorsY[side][i];
                    break;

                case SSide_Front:
                case SSide_Back:
                    WorldStarPos.x -= CStarrySkies::StarCoorsX[side][i];
                    WorldStarPos.z += CStarrySkies::StarCoorsY[side][i];
                    break;

                default:
                    WorldStarPos.x += CStarrySkies::StarCoorsX[side][i];
                    WorldStarPos.y += CStarrySkies::StarCoorsY[side][i];
                    break;
            }

            if (CSprite::CalcScreenCoors(WorldStarPos, &ScreenPos, &SZX, &SZY, false, true))
            {
                const auto brightness = CalculateColorWithBalanceStarry(colorB, (float)(rand() % 32) * 0.015f);
                CSprite::RenderBufferedOneXLUSprite(ScreenPos.x, ScreenPos.y, ScreenPos.z, SZX * SZ, SZY * SZ,
                                           brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }
    }
}

void CStarrySkies::InjectHooks() {
    fSmallStars = ClampFloat(0.15f, 0.03f, 2.5f);
    fMiddleStars = ClampFloat(0.6f, 0.03f, 2.5f);
    fBiggestStars = ClampFloat(1.2f, 0.03f, 2.5f);
    fBiggestStarsSpawnChance = 1.0f - 0.01f * 20.0f;
    InitializeThoseStars();
}
