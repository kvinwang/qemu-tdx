#include "qemu/osdep.h"
#include <glib.h>

#include "hw/i386/acpi-common.h"

static void run_case(const char *env, bool expected)
{
    gchar *saved = NULL;
    const char *current = g_getenv("QEMU_ACPI_COMPAT_VER");

    if (current) {
        saved = g_strdup(current);
    }

    acpi_dump_compat_reset();

    if (env) {
        g_setenv("QEMU_ACPI_COMPAT_VER", env, true);
    } else {
        g_unsetenv("QEMU_ACPI_COMPAT_VER");
    }

    g_assert_cmpint(acpi_dump_compat_9_1(), ==, expected);

    if (saved) {
        g_setenv("QEMU_ACPI_COMPAT_VER", saved, true);
    } else {
        g_unsetenv("QEMU_ACPI_COMPAT_VER");
    }

    g_free(saved);
}

static void test_default(void)
{
    run_case(NULL, false);
}

static void test_valid_pre_release(void)
{
    run_case("9.1.1", true);
}

static void test_valid_boundary(void)
{
    run_case("9.1.99", true);
    run_case("9.2.0", false);
}

static void test_newer_versions(void)
{
    run_case("9.2.0", false);
    run_case("9.2.1", false);
    run_case("9.3.0", false);
    run_case("10.0.0", false);
}

static void test_invalid_strings(void)
{
    run_case("", false);
    run_case("9.1", false);
    run_case("9.1.1.2", false);
    run_case("foo", false);
    run_case("9.-1.0", false);
}

static void test_version_comparison(void)
{
    acpi_dump_compat_reset();
    g_setenv("QEMU_ACPI_COMPAT_VER", "10.2.0", true);

    g_assert_false(acpi_dump_compat_before(10, 0, 0));
    g_assert_false(acpi_dump_compat_before(10, 2, 0));
    g_assert_true(acpi_dump_compat_before(11, 0, 0));
    g_assert_true(acpi_dump_compat_before(11, 1, 0));

    acpi_dump_compat_reset();
    g_setenv("QEMU_ACPI_COMPAT_VER", "11.1.0", true);
    g_assert_false(acpi_dump_compat_before(11, 1, 0));

    acpi_dump_compat_reset();
    g_setenv("QEMU_ACPI_COMPAT_VER", "invalid", true);
    g_assert_false(acpi_dump_compat_before(99, 0, 0));

    g_unsetenv("QEMU_ACPI_COMPAT_VER");
}

static void test_cache_reset(void)
{
    acpi_dump_compat_reset();
    g_setenv("QEMU_ACPI_COMPAT_VER", "9.1.1", true);
    g_assert_true(acpi_dump_compat_9_1());

    /* cached result should hide later changes until we reset again */
    g_setenv("QEMU_ACPI_COMPAT_VER", "9.3.0", true);
    g_assert_true(acpi_dump_compat_9_1());

    acpi_dump_compat_reset();
    g_assert_false(acpi_dump_compat_9_1());

    g_unsetenv("QEMU_ACPI_COMPAT_VER");
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/acpi/compat/default", test_default);
    g_test_add_func("/acpi/compat/valid_pre_release", test_valid_pre_release);
    g_test_add_func("/acpi/compat/valid_boundary", test_valid_boundary);
    g_test_add_func("/acpi/compat/newer_versions", test_newer_versions);
    g_test_add_func("/acpi/compat/invalid_strings", test_invalid_strings);
    g_test_add_func("/acpi/compat/version_comparison", test_version_comparison);
    g_test_add_func("/acpi/compat/cache_reset", test_cache_reset);

    return g_test_run();
}
