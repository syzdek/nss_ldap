
/*
   Glue code to support AIX loadable authentication modules.

   Note: only information functions are supported, so you need to
   specify "options = dbonly" in /usr/lib/security/methods.cfg

   (Note: the is now experimental support for authentication
   functions - getpasswd/authenticate. This has not been tested
   as PADL do not have access to an AIX machine.)
 */
#include "config.h"

#ifdef _AIX

#include <stdlib.h>
#include <string.h>
#include <usersec.h>

#ifdef HAVE_LBER_H
#include <lber.h>
#endif
#ifdef HAVE_LDAP_H
#include <ldap.h>
#endif

#include "ldap-nss.h"
#include "ldap-grp.h"
#include "globals.h"
#include "util.h"

static struct irs_gr *grp_conn = NULL;
static struct irs_pw *pwd_conn = NULL;

/* Prototype definitions */
void *gr_pvtinit (void);
struct group *gr_byname (struct irs_gr *, const char *);
struct group *gr_bygid (struct irs_gr *, gid_t);
void gr_close (struct irs_gr *);

void *pw_pvtinit (void);
struct passwd *pw_byname (struct irs_pw *, const char *);
struct passwd *pw_byuid (struct irs_pw *, uid_t);
void pw_close (struct irs_pw *);

char *_nss_ldap_getgrset (char *user);

static void *
_nss_ldap_open (const char *name, const char *domain,
		const int mode, char *options)
{
  /* Currently we do not use the above parameters */

  grp_conn = (struct irs_gr *) gr_pvtinit ();
  pwd_conn = (struct irs_pw *) pw_pvtinit ();
  return NULL;
}

static int
_nss_ldap_close (void *token)
{
  gr_close (grp_conn);
  grp_conn = NULL;

  pw_close (pwd_conn);
  pwd_conn = NULL;

  return AUTH_SUCCESS;
}

static struct group *
_nss_ldap_getgrgid (gid_t gid)
{
  if (!grp_conn)
    return NULL;

  return gr_bygid (grp_conn, gid);
}

static struct group *
_nss_ldap_getgrnam (const char *name)
{
  if (!grp_conn)
    return NULL;

  return gr_byname (grp_conn, name);
}

static struct passwd *
_nss_ldap_getpwuid (uid_t uid)
{
  if (!pwd_conn)
    return NULL;

  return pw_byuid (pwd_conn, uid);
}

static struct passwd *
_nss_ldap_getpwnam (const char *name)
{
  if (!pwd_conn)
    return NULL;

  return pw_byname (pwd_conn, name);
}

static struct group *
_nss_ldap_getgracct (void *id, int type)
{
  if (type == SEC_INT)
    return _nss_ldap_getgrgid (*(gid_t *) id);
  else
    return _nss_ldap_getgrnam ((char *) id);
}

#ifdef PROXY_AUTH
int
_nss_ldap_authenticate (char *user, char *response, int **reenter, char **message)
{
    NSS_STATUS stat;
    int rc;

    *reenter = 0;
    *message = NULL;

    stat = _nss_ldap_proxy_bind(user, response);

    switch (stat) {
	case NSS_TRYAGAIN:
	    rc = AUTH_FAILURE;
	    break;
	case NSS_NOTFOUND:
	    rc = AUTH_NOTFOUND;
	    break;
	case NSS_SUCCESS:
	    rc = AUTH_SUCCESS;
	    break;
	default:
	case NSS_UNAVAIL:
	    rc = AUTH_UNAVAIL;
	    break;
    }

    return rc;
}
#endif /* PROXY_AUTH */

/*
 * Support this for when proxy authentication is disabled.
 * There may be some re-entrancy issues here; not sure
 * if we are supposed to return allocated memory or not,
 * this is not documented. I am assuming not in line with
 * the other APIs.
 */
char *
_nss_ldap_getpasswd (char *user)
{
    struct passwd *pw;
    static char pwdbuf[32];
    char *p = NULL;

    pw = _nss_ldap_getpwnam(user);
    if (pw != NULL) {
	if (strlen(pw->pw_passwd) > sizeof(pwdbuf) - 1) {
		errno = ERANGE;
	} else {
		strcpy(pwdbuf, pw->pw_passwd);
		p = pwdbuf;
	}
    }

    return p;
}

int
nss_ldap_initialize (struct secmethod_table *meths)
{
  bzero (meths, sizeof (*meths));

  /* Identification methods */
  meths->method_getpwnam = _nss_ldap_getpwnam;
  meths->method_getpwuid = _nss_ldap_getpwuid;
  meths->method_getgrnam = _nss_ldap_getgrnam;
  meths->method_getgrgid = _nss_ldap_getgrgid;
  meths->method_getgrset = _nss_ldap_getgrset;
  meths->method_getgracct = _nss_ldap_getgracct;

  /* Support methods */
  meths->method_open = _nss_ldap_open;
  meths->method_close = _nss_ldap_close;

  /* Authentication methods */
#ifdef PROXY_AUTH
  meths->method_authenticate = _nss_ldap_authenticate;
#else
  meths->method_authenticate = NULL;
#endif

  meths->method_getpasswd = _nss_ldap_getpasswd;
  meths->method_chpass = NULL;
  meths->method_passwordexpired = NULL;

  return AUTH_SUCCESS;
}

#endif /* _AIX */
