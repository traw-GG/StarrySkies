void CClouds::InjectHooks() {
    CHook::Redirect("_ZN7CClouds6RenderEv", &CClouds::Render);
}

void CClouds::Render() {
    Render_MaybeRenderRockstarLogo(colorBalance);
}

void CClouds::Render_MaybeRenderRockstarLogo(float colorBalance) {
    CCamera& TheCamera = *reinterpret_cast<CCamera*>(g_libGTASA + (VER_x32 ? 0x00951FA8 : 0xBBA8D0));

    constexpr auto LOGO_VISIBLE_FROM_HRS  = 22u,
            LOGO_VISIBLE_UNTIL_HRS = 5u;

    constexpr auto R_OFFSET_FROM_CAMERA    = CVector{ 100.f, 0.f, 10.f };
    constexpr auto STAR_OFFSET_FROM_CAMERA = CVector{ 100.f, 0.f, R_OFFSET_FROM_CAMERA.z - 90.f };

    constexpr auto  STARS_NUM_POSITIONS                    = 9;
    constexpr float STARS_Y_POSITIONS[STARS_NUM_POSITIONS] = { 0.00f, 0.05f, 0.13f, 0.40f, 0.70f, 0.60f, 0.27f, 0.55f, 0.75f };
    constexpr float STARS_Z_POSITIONS[STARS_NUM_POSITIONS] = { 0.00f, 0.45f, 0.90f, 1.00f, 0.85f, 0.52f, 0.48f, 0.35f, 0.20f };
    constexpr float STARS_SIZES[STARS_NUM_POSITIONS]       = { 1.00f, 1.40f, 0.90f, 1.00f, 0.60f, 1.50f, 1.30f, 1.00f, 0.80f };

    if (!CClock::GetIsTimeInRange(LOGO_VISIBLE_FROM_HRS, LOGO_VISIBLE_UNTIL_HRS)) {
        return;
    }

    const auto time = CClock::GetGameClockHours() == LOGO_VISIBLE_FROM_HRS
                      ? CClock::GetGameClockMinutes()
                      : 60u - CClock::GetGameClockMinutes();

    const auto colorB  = 255u * time / 60u;
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, RWRSTATE(RwTextureGetRaster(CCoronas::gpCoronaTexture[0])));
	
    CStarrySkies::StarrySkies();
    CSprite::FlushSpriteBuffer();
}