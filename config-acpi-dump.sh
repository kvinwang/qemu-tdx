#!/bin/sh
CFLAGS="-DDUMP_ACPI_TABLES" ../configure --target-list=x86_64-softmmu --prefix=`pwd`/dist
