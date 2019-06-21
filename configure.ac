AC_INIT(gambit, version-0.1)

AC_PROG_CC([gcc])

export LD_LIBRARY_PATH='/usr/local/include:/usr/local/lib:/c/MinGW/msys/1.0/local/include:/c/MinGW/msys/1.0/local/lib'
export CPPFLAGS='-I/usr/local/lib -I/usr/local/include -I/c/MinGW/msys/1.0/local/include'

AC_CHECK_PROG(GIT,[git],[yes],[no])
test "$GIT" == "no" && AC_MSG_ERROR([Required program 'git' not found.])

function require_cspec {
  local install_attempted=$1
  local libpresent=0

  AC_CHECK_HEADERS(cspec.h, [libpresent=1], [libpresent=0])
  AC_SEARCH_LIBS(cspec, [libpresent=1], [libpresent=0])

  if test "$libpresent" == 1
  then
    return
  fi

  if test "$libpresent" == 0
  then
    AC_MSG_NOTICE([CSpec not found. Attempting to install CSpec])
    cd .tmp/
    git clone https://github.com/arnaudbrejeon/cspec
    cd cspec && ./autogen.sh && ./configure && make && make install && cd ..
    cd ..
  fi
}

require_cspec 0

sh scripts/generate_tests

AC_OUTPUT(Makefile)