/* Copyright (C) 1997-2004 Luke Howard.
   This file is part of the nss_ldap library.
   Contributed by Luke Howard, <lukeh@padl.com>, 1997.
   (The author maintains a non-exclusive licence to distribute this file
   under their own conditions.)

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

#ifndef _LDAP_NSS_LDAP_UTIL_H
#define _LDAP_NSS_LDAP_UTIL_H

/* utility routines.  */

#define CN_ATTR			"CN"

#define DC_ATTR			"DC"
#define DC_ATTR_AVA		DC_ATTR"="
#define DC_ATTR_AVA_LEN		(sizeof(DC_ATTR_AVA) - 1)

/*
 * get the RDN's value: eg. if the RDN was cn=lukeh, getrdnvalue(entry)
 * would return lukeh.
 */
NSS_STATUS _nss_ldap_getrdnvalue (LDAP * ld,
				  LDAPMessage * entry,
				  const char *rdntype,
				  char **rval, char **buf, size_t * len);

#ifdef RFC2307BIS
/*
 * map a distinguished name to a login name, or group entry
 */
NSS_STATUS _nss_ldap_dn2uid (LDAP * ld,
			     const char *dn,
			     char **uid, char **buf, size_t * len,
			     int *pIsNestedGroup, LDAPMessage ** pRes);
#endif /* RFC2307BIS */

#ifdef AT_OC_MAP
#define NSS_LDAP_KEY_MAP_ATTRIBUTE      "nss_map_attribute"
#define NSS_LDAP_KEY_MAP_OBJECTCLASS    "nss_map_objectclass"
#define NSS_LDAP_KEY_SET_OVERRIDE       "nss_override_attribute_value"
#define NSS_LDAP_KEY_SET_DEFAULT        "nss_default_attribute_value"
#endif /* AT_OC_MAP */

#define NSS_LDAP_CONFIG_BUFSIZ		4096
#define NSS_LDAP_KEY_HOST		"host"
#define NSS_LDAP_KEY_SCOPE		"scope"
#define NSS_LDAP_KEY_BASE		"base"
#define NSS_LDAP_KEY_PORT		"port"
#define NSS_LDAP_KEY_BINDDN		"binddn"
#define NSS_LDAP_KEY_BINDPW   		"bindpw"
#define	NSS_LDAP_KEY_USESASL		"use_sasl"
#define	NSS_LDAP_KEY_SASLID		"sasl_auth_id"
#define NSS_LDAP_KEY_DEREF    		"deref"
#define NSS_LDAP_KEY_ROOTBINDDN		"rootbinddn"
#define	NSS_LDAP_KEY_ROOTUSESASL	"rootuse_sasl"
#define	NSS_LDAP_KEY_ROOTSASLID		"rootsasl_auth_id"
#define NSS_LDAP_KEY_LDAP_VERSION	"ldap_version"
#define NSS_LDAP_KEY_TIMELIMIT		"timelimit"
#define NSS_LDAP_KEY_BIND_TIMELIMIT	"bind_timelimit"
#define NSS_LDAP_KEY_SSL		"ssl"
#define NSS_LDAP_KEY_SSLPATH		"sslpath"
#define NSS_LDAP_KEY_REFERRALS		"referrals"
#define NSS_LDAP_KEY_RESTART		"restart"
#define NSS_LDAP_KEY_URI		"uri"
#define NSS_LDAP_KEY_IDLE_TIMELIMIT     "idle_timelimit"
#define NSS_LDAP_KEY_RECONNECT_POLICY	"bind_policy"
#define NSS_LDAP_KEY_SASL_SECPROPS      "sasl_secprops"
#ifdef CONFIGURE_KRB5_CCNAME
#define NSS_LDAP_KEY_KRB5_CCNAME        "krb5_ccname"
#endif /* CONFIGURE_KRB5_CCNAME */
#define NSS_LDAP_KEY_LOGDIR		"logdir"
#define NSS_LDAP_KEY_DEBUG		"debug"

/*
 * support separate naming contexts for each map 
 * eventually this will support the syntax defined in
 * the DUAConfigProfile searchDescriptor attribute
 */
#define NSS_LDAP_KEY_NSS_BASE_PASSWD		"nss_base_passwd"
#define NSS_LDAP_KEY_NSS_BASE_SHADOW		"nss_base_shadow"
#define NSS_LDAP_KEY_NSS_BASE_GROUP		"nss_base_group"
#define NSS_LDAP_KEY_NSS_BASE_HOSTS		"nss_base_hosts"
#define NSS_LDAP_KEY_NSS_BASE_SERVICES		"nss_base_services"
#define NSS_LDAP_KEY_NSS_BASE_NETWORKS		"nss_base_networks"
#define NSS_LDAP_KEY_NSS_BASE_PROTOCOLS		"nss_base_protocols"
#define NSS_LDAP_KEY_NSS_BASE_RPC		"nss_base_rpc"
#define NSS_LDAP_KEY_NSS_BASE_ETHERS		"nss_base_ethers"
#define NSS_LDAP_KEY_NSS_BASE_NETMASKS		"nss_base_netmasks"
#define NSS_LDAP_KEY_NSS_BASE_BOOTPARAMS	"nss_base_bootparams"
#define NSS_LDAP_KEY_NSS_BASE_ALIASES		"nss_base_aliases"
#define NSS_LDAP_KEY_NSS_BASE_NETGROUP		"nss_base_netgroup"
#define NSS_LDAP_KEY_NSS_BASE_AUTOMOUNT		"nss_base_automount"

/*
 * There are a number of means of obtaining configuration information.
 *
 * (a) DHCP (Cf draft-hedstrom-dhc-ldap-00.txt)
 * (b) a configuration file (/etc/ldap.conf) **
 * (c) a coldstart file & subsequent referrals from the LDAP server
 * (d) a custom LDAP bind protocol
 * (e) DNS **
 *
 * This should be opaque to the rest of the library.
 * ** implemented
 */

NSS_STATUS _nss_ldap_init_config (ldap_config_t *);

NSS_STATUS _nss_ldap_readconfig (ldap_config_t ** result,
				 char *buf, size_t buflen);

/*
 * Escape '*' in a string for use as a filter
 */

NSS_STATUS _nss_ldap_escape_string (const char *str,
				    char *buf, size_t buflen);

#define MAP_H_ERRNO(nss_status, herr)   do { \
		switch ((nss_status)) {		\
		case NSS_SUCCESS:		\
			(herr) = 0;		\
			break;			\
		case NSS_TRYAGAIN:		\
			(herr) = TRY_AGAIN;	\
			break;			\
		case NSS_NOTFOUND:		\
			(herr) = HOST_NOT_FOUND;\
			break;			\
		case NSS_UNAVAIL:		\
		default:			\
			(herr) = NO_RECOVERY;	\
			break;			\
		}				\
	} while (0)

#ifdef HAVE_IRS_H
#define MAP_ERRNO(nss_status, err)   do {	\
		switch ((nss_status)) {		\
		case NSS_SUCCESS:		\
			(err) = 0;		\
			break;			\
		case NSS_TRYAGAIN:		\
			(err) = ERANGE;		\
			break;			\
		case NSS_NOTFOUND:		\
			(err) = ENOENT;		\
			break;			\
		case NSS_UNAVAIL:		\
		default:			\
			(err) = EPERM;		\
			break;			\
		}				\
	} while (0)
#endif /* HAVE_IRS_H */

#if defined(RFC2307BIS) || defined(AT_OC_MAP)
struct ldap_datum
{
  void *data;
  size_t size;
};

typedef struct ldap_datum ldap_datum_t;

#define NSS_LDAP_DATUM_ZERO(d)	do { \
		(d)->data = NULL; \
		(d)->size = 0; \
	} while (0)

void *_nss_ldap_db_open (void);
void _nss_ldap_db_close (void *db);
NSS_STATUS _nss_ldap_db_put (void *db, const ldap_datum_t * key,
			     const ldap_datum_t * value);
NSS_STATUS _nss_ldap_db_get (void *db, const ldap_datum_t * key,
			     ldap_datum_t * value);
#endif /* RFC2307BIS || AT_OC_MAP */

#endif /* _LDAP_NSS_LDAP_UTIL_H */
