

/* Copyright (C) 1997 Luke Howard.
   This file is part of the nss_ldap library.
   Contributed by Luke Howard, <lukeh@padl.com>, 1997.

   The nss_ldap library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The nss_ldap library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the nss_ldap library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
 */

#ifdef IRS_NSS

#include <errno.h>
#include "irs-nss.h"

/* $Id$ */

static void gr_close (struct irs_gr *);
static struct group *gr_next (struct irs_gr *);
static struct group *gr_byname (struct irs_gr *, const char *);
static struct group *gr_bygid (struct irs_gr *, gid_t);
static void gr_rewind (struct irs_gr *);
static void gr_minimize (struct irs_gr *);

struct pvt
  {
    struct group result;
    char buffer[NSS_BUFLEN_GROUP];
    context_handle_t state;
  };

static struct group *
gr_byname (struct irs_gr *this, const char *name)
{
  LOOKUP_NAME (name, this, filt_getgrnam, gr_attributes, _nss_ldap_parse_gr);
}

static struct group *
gr_bygid (struct irs_gr *this, gid_t gid)
{
  LOOKUP_NUMBER (gid, this, filt_getgrgid, gr_attributes, _nss_ldap_parse_gr);
}

static void
gr_close (struct irs_gr *this)
{
  LOOKUP_ENDENT (this);
}

static struct group *
gr_next (struct irs_gr *this)
{
  LOOKUP_GETENT (this, filt_getgrent, gr_attributes, _nss_ldap_parse_gr);
}

static void
gr_rewind (struct irs_gr *this)
{
  LOOKUP_SETENT (this);
}

static void
gr_minimize (struct irs_gr *this)
{
}


struct irs_gr *
irs_ldap_gr (struct irs_acc *this)
{
  struct irs_gr *gr;
  struct pvt *pvt;

  gr = calloc (1, sizeof (*gr));
  if (gr == NULL)
    return NULL;

  pvt = calloc (1, sizeof (*pvt));
  if (pvt == NULL)
    return NULL;

  pvt->state = NULL;
  gr->private = pvt;
  gr->close = gr_close;
  gr->next = gr_next;
  gr->byname = gr_byname;
  gr->bygid = gr_bygid;
  gr->list = make_group_list;
  gr->rewind = gr_rewind;
  gr->minimize = gr_minimize;
  return gr;
}

#endif /*IRS_NSS */
