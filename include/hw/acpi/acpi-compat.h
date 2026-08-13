#ifndef HW_ACPI_COMPAT_H
#define HW_ACPI_COMPAT_H

bool acpi_dump_compat_9_1(void);
bool acpi_dump_compat_before(unsigned major, unsigned minor, unsigned micro);
void acpi_dump_compat_reset(void);

#endif
