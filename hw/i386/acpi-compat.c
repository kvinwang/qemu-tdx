#include "qemu/osdep.h"
#include <limits.h>

#include "hw/i386/acpi-common.h"

static int compat_cached = -1;
static long compat_version[3];
static bool compat_version_valid;

static bool parse_version_tuple(const char *env, long ver[3])
{
    bool valid = false;
    gchar **parts = g_strsplit(env, ".", 0);

    if (parts[0] && parts[1] && parts[2] && !parts[3]) {
        valid = true;
        for (int i = 0; i < 3; i++) {
            char *endptr = NULL;

            if (!*parts[i]) {
                valid = false;
                break;
            }

            ver[i] = strtol(parts[i], &endptr, 10);
            if (*endptr != '\0' || ver[i] < 0 || ver[i] > INT_MAX) {
                valid = false;
                break;
            }
        }
    }

    g_strfreev(parts);
    return valid;
}

bool acpi_dump_compat_9_1(void)
{
    if (compat_cached < 0) {
        const char *env = g_getenv("QEMU_ACPI_COMPAT_VER");

        compat_cached = 0;
        if (env && *env) {
            long ver[3];

            if (parse_version_tuple(env, ver)) {
                memcpy(compat_version, ver, sizeof(compat_version));
                compat_version_valid = true;
                const int cutoff[3] = { 9, 2, 0 };

                if (ver[0] < cutoff[0] ||
                    (ver[0] == cutoff[0] && ver[1] < cutoff[1]) ||
                    (ver[0] == cutoff[0] && ver[1] == cutoff[1] &&
                     ver[2] < cutoff[2])) {
                    compat_cached = 1;
                }
            }
        }
    }

    return compat_cached;
}

bool acpi_dump_compat_before(unsigned major, unsigned minor, unsigned micro)
{
    /* Populate the shared parsed-version cache. */
    acpi_dump_compat_9_1();
    if (!compat_version_valid) {
        return false;
    }
    return compat_version[0] < major ||
           (compat_version[0] == major && compat_version[1] < minor) ||
           (compat_version[0] == major && compat_version[1] == minor &&
            compat_version[2] < micro);
}

void acpi_dump_compat_reset(void)
{
    compat_cached = -1;
    compat_version_valid = false;
}
