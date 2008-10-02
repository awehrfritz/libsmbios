/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim:expandtab:autoindent:tabstop=4:shiftwidth=4:filetype=c:cindent:textwidth=0:
 *
 * Copyright (C) 2005 Dell Inc.
 *  by Michael Brown <Michael_E_Brown@dell.com>
 * Licensed under the Open Software License version 2.1
 *
 * Alternatively, you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 */

#define LIBSMBIOS_SOURCE
#include "smbios_c/compat.h"

#include <string.h>
#include <stdlib.h>

#include "smbios_c/smbios.h"
#include "smbios_c/version.h"
#include "smbios_c/system_info.h"
#include "dell_magic.h"
#include "_impl.h"

const char *smbios_get_library_version_string()
{
    return LIBSMBIOS_RELEASE_VERSION;
}

int smbios_get_library_version_major()
{
    return LIBSMBIOS_RELEASE_MAJOR;
}

int smbios_get_library_version_minor()
{
    return LIBSMBIOS_RELEASE_MINOR;
}

void __internal strip_trailing_whitespace( char *str )
{
    if(!str)
        return;

    if(strlen(str) == 0)
        return;

    size_t ch = strlen(str);
    do
    {
        --ch;
        if( ' ' == str[ch] )
            str[ch] = '\0';
        else
            break;

    } while(ch);
}

__internal char * smbios_get_string_from_table(u8 type, u8 offset)
{
    struct smbios_table *table;
    const struct smbios_struct *s;
    const char *r;
    char *ret=0;

    table = smbios_factory(SMBIOS_GET_SINGLETON);
    if (!table)
        goto out;

    s = smbios_get_next_struct_by_type(table, 0, type);
    if (!s)
        goto out;

    r = smbios_get_string_from_offset(s, offset);
    if (!r)
        goto out;

    ret = calloc(1, strlen(r)+1);
    if(!ret)
        goto out;

    strcpy(ret, r);
    strip_trailing_whitespace(ret);

out:
    smbios_table_free(table);
    return ret;
}

void smbios_string_free(void *f)
{
    free(f);
}

char *smbios_get_vendor_name()
{
    return smbios_get_string_from_table(System_Information_Structure, System_Information_Manufacturer_Offset);
}

char *smbios_get_system_name()
{
    return smbios_get_string_from_table(System_Information_Structure, System_Information_Product_Name_Offset);
}

char *smbios_get_bios_version()
{
    return smbios_get_string_from_table(BIOS_Information_Structure, BIOS_Information_Version_Offset);
}