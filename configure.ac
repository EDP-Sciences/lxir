AC_INIT([lxir], [1.0.1])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.6.1])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AM_PROG_CC_C_O
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB

AC_C_CONST

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_STAT
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([libintl.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([sys/param.h])

AC_TYPE_SIZE_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

AC_FUNC_ALLOCA
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([regcomp])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([strtol])
AC_CHECK_FUNCS([strtoul])
	

AC_ARG_WITH([kpse], [AS_HELP_STRING([--with-kpse], [support libkpathsea])])
	
AC_MSG_CHECKING([wether we will use kpathsea])
if test "$with_kpse" != "no" ; then
	AC_DEFINE(USE_KPSE, [1], [Use kpathsea library])
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_ARG_WITH([tools], [AS_HELP_STRING([--with-tools], [build helper tools])])

AC_MSG_CHECKING([wether we will build the utility tools])
if test "$with_tools" == "yes" ; then
	AC_DEFINE(BUILD_TOOLS, [1], [Build helper tools])
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_ARG_WITH(libxml-prefix,
        [AS_HELP_STRING([--with-libxml-prefix=[PFX]], [Specify location of libxml config])],
	LIBXML_CONFIG_PREFIX=$withval
)

AC_MSG_CHECKING([libxml2/libxslt config path])
if test "x$LIBXML_CONFIG_PREFIX" != "x"
then
	XML_CONFIG=${LIBXML_CONFIG_PREFIX}/bin/xml2-config
	XSLT_CONFIG=${LIBXML_CONFIG_PREFIX}/bin/xslt-config
else
	XML_CONFIG=xml2-config
	XSLT_CONFIG=xslt-config
fi
AC_MSG_RESULT($XML_CONFIG $XSLT_CONFIG)

LIBXML2_CFLAGS="$LIBXML2_CFLAGS `$XML_CONFIG --cflags`"
LIBXML2_LIBS="$LIBXML2_LIBS `$XML_CONFIG --libs`"

LIBXSLT_CFLAGS="$LIBXSLT_CFLAGS `$XSLT_CONFIG --cflags`"
LIBXSLT_LIBS="$LIBXSLT_LIBS `$XSLT_CONFIG --libs`"

AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXSLT_CFLAGS)
AC_SUBST(LIBXSLT_LIBS)

AM_CONDITIONAL([USE_KPSE], [test "x$with_kpse" != "xno"])
AM_CONDITIONAL([BUILD_TOOLS], [test "x$with_tools" == "xyes"])
AM_CONDITIONAL([KPSE_x64], [test "$target_cpu" == "x86_64"])

AC_ARG_WITH(texmf-prefix,
	[AS_HELP_STRING([--with-texmf-prefix=[path]], [Specify location of texmf destination])],
	TEXMF_DEST_DIR=$withval
)

AC_MSG_CHECKING([texmf destination path])
if test "x$TEXMF_DEST_DIR" == "x" -o "x$TEXMF_DEST_DIR" == "xNONE"
then
TEXMF_DEST_DIR="$prefix/texmf"
fi

AC_SUBST(TEXMF_DEST_DIR)

TEXMF_LXIR_DIR="$TEXMF_DEST_DIR/tex/lxir"
AC_SUBST(TEXMF_LXIR_DIR)

TEXMF_XML_DIR="$TEXMF_LXIR_DIR/xml"
TEXMF_XSLT_DIR="$TEXMF_LXIR_DIR/xslt"
AC_MSG_RESULT($TEXMF_XML_DIR $TEXMF_XSLT_DIR)

AC_SUBST(TEXMF_XML_DIR)
AC_SUBST(TEXMF_XSLT_DIR)
AC_DEFINE_UNQUOTED(TEXMF_XML_DIR, "${TEXMF_XML_DIR}", [texmf XML path])
AC_DEFINE_UNQUOTED(TEXMF_XSLT_DIR, "${TEXMF_XSLT_DIR}", [texmf XSLT path])

AC_ARG_WITH(tfm-search-path,
	[AS_HELP_STRING([--with-tfm-search-path=[path]], [Specify location to search font from])],
	TFM_SEARCH_PATH=$withval
)

AC_SUBST(TFM_SEARCH_PATH)
AC_DEFINE_UNQUOTED(TFM_SEARCH_PATH, "${TFM_SEARCH_PATH}", [tfm search path])

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([
	Makefile
	libkpathsea/Makefile
	libfontmap/Makefile
	libfontmap/tools/Makefile
	libfontmap/tools/tfmlist/Makefile
	libfontmap/tools/enc2tbl/Makefile
	libfontmap/tools/accents/Makefile
	libdvi/Makefile
	libtfm/Makefile
	src/Makefile
	texmf/Makefile
	texmf/default/Makefile
	texmf/tex/Makefile
	texmf/xslt/Makefile
	texmf/xslt/xlatex2xhtml.xsl
	texmf/css/Makefile
])
AC_OUTPUT
