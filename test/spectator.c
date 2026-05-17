// SPDX-License-Identifier: LicenseRef-AGPL-3.0-only-OpenSSL

#include <munit.h>
#include <string.h>
#include <chiaki/session.h>
#include <chiaki/log.h>
#include <chiaki/feedback.h>

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

// Verify chiaki_session_init copies spectator_mode from ConnectInfo to session.
static MunitResult test_session_init_copies_spectator_mode(
		const MunitParameter params[], void *user)
{
	(void)params; (void)user;
	ChiakiConnectInfo info;
	memset(&info, 0, sizeof(info));
	info.host = "127.0.0.1";
	info.regist_key[0] = '\0';
	info.spectator_mode = true;

	ChiakiLog log;
	chiaki_log_init(&log, CHIAKI_LOG_ALL, NULL, NULL);

	ChiakiSession session;
	// chiaki_session_init may return an error because host isn't reachable
	// or registration data is incomplete, but the spectator_mode copy
	// happens before any network I/O / heavy validation.
	chiaki_session_init(&session, &info, &log);
	munit_assert_true(session.spectator_mode);
	chiaki_session_fini(&session);
	return MUNIT_OK;
}

static MunitResult test_feedback_state_neutralize_zeroes_everything(
		const MunitParameter params[], void *user)
{
	(void)params; (void)user;
	ChiakiFeedbackState state = {
		.gyro_x = 1.0f, .gyro_y = 2.0f, .gyro_z = 3.0f,
		.accel_x = 4.0f, .accel_y = 5.0f, .accel_z = 6.0f,
		.orient_x = 7.0f, .orient_y = 8.0f, .orient_z = 9.0f, .orient_w = 10.0f,
		.left_x = 100, .left_y = 200, .right_x = 300, .right_y = 400,
	};
	chiaki_feedback_state_neutralize(&state);
	munit_assert_float(state.gyro_x, ==, 0.0f);
	munit_assert_float(state.gyro_y, ==, 0.0f);
	munit_assert_float(state.gyro_z, ==, 0.0f);
	munit_assert_float(state.accel_x, ==, 0.0f);
	munit_assert_float(state.accel_y, ==, 0.0f);
	munit_assert_float(state.accel_z, ==, 0.0f);
	munit_assert_float(state.orient_x, ==, 0.0f);
	munit_assert_float(state.orient_y, ==, 0.0f);
	munit_assert_float(state.orient_z, ==, 0.0f);
	munit_assert_float(state.orient_w, ==, 0.0f);
	munit_assert_int(state.left_x, ==, 0);
	munit_assert_int(state.left_y, ==, 0);
	munit_assert_int(state.right_x, ==, 0);
	munit_assert_int(state.right_y, ==, 0);
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
	{
		"/session_init_copies_spectator_mode",
		test_session_init_copies_spectator_mode,
		NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL
	},
	{
		"/feedback_state_neutralize_zeroes_everything",
		test_feedback_state_neutralize_zeroes_everything,
		NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
