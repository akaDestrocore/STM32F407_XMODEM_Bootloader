# Get git revision.
#
# Defines the following variables:
#  GIT_SHA1 - Current git SHA1 hash

find_package(Git QUIET)
if(NOT GIT_FOUND)
    set(GIT_SHA1 "GIT-NOTFOUND")
    return()
endif()

include(GetGitRevisionDescription.cmake.in)
git_describe(GIT_SHA1 ${ARGN})

if(NOT GIT_SHA1)
    set(GIT_SHA1 "GIT-NOTFOUND")
endif()