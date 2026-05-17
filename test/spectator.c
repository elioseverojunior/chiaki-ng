// SPDX-License-Identifier: LicenseRef-AGPL-3.0-only-OpenSSL

#include <munit.h>
#include <string.h>
#include <chiaki/session.h>

// Verify ChiakiConnectInfo has a spectator_mode field that defaults to false.
static MunitResult test_connect_info_has_spectator_mode_default_false(
        const MunitParameter params[], void *user)
{
    (void)params; (void)user;
    ChiakiConnectInfo info;
    memset(&info, 0, sizeof(info));
    // Zero-init should leave spectator_mode false (it's a bool).
    munit_assert_false(info.spectator_mode);
    return MUNIT_OK;
}

// Verify the field is independently settable.
static MunitResult test_connect_info_spectator_mode_settable(
        const MunitParameter params[], void *user)
{
    (void)params; (void)user;
    ChiakiConnectInfo info;
    memset(&info, 0, sizeof(info));
    info.spectator_mode = true;
    munit_assert_true(info.spectator_mode);
    return MUNIT_OK;
}

MunitTest tests_spectator[] = {
    {
        "/connect_info_has_spectator_mode_default_false",
        test_connect_info_has_spectator_mode_default_false,
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL
    },
    {
        "/connect_info_spectator_mode_settable",
        test_connect_info_spectator_mode_settable,
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
