#pragma once
#ifndef MESSMER_FSPP_TEST_FUSE_LSTAT_TESTUTILS_FUSELSTATRETURNTEST_H_
#define MESSMER_FSPP_TEST_FUSE_LSTAT_TESTUTILS_FUSELSTATRETURNTEST_H_

#include "FuseLstatTest.h"

// This class offers test helpers for testing (struct FUSE_STAT) entries. We return them from
// our mock filesystem, set up a temporary filesystem, call lstat syscall on it, and
// then check the return value.
template<typename Property>
class FuseLstatReturnTest: public FuseLstatTest {
public:
  // Set the specified (struct FUSE_STAT) entry to the given value, and test whether it is correctly returned from the syscall.
  // The CallFile[...] version tests it on a file node of the filesystem, the CallDir[...] version on a dir node.
  struct FUSE_STAT CallFileLstatWithValue(Property value);
  struct FUSE_STAT CallDirLstatWithValue(Property value);

private:
  std::function<void(struct FUSE_STAT*)> SetPropertyImpl(Property value);

  // Override this function to specify, how to set the specified (struct FUSE_STAT) entry on the passed (struct FUSE_STAT *) object.
  virtual void set(struct FUSE_STAT *stat, Property value) = 0;
};

template<typename Property>
FUSE_STAT FuseLstatReturnTest<Property>::CallFileLstatWithValue(Property value) {
  return CallFileLstatWithImpl(SetPropertyImpl(value));
}

template<typename Property>
FUSE_STAT FuseLstatReturnTest<Property>::CallDirLstatWithValue(Property value) {
  return CallDirLstatWithImpl(SetPropertyImpl(value));
}

template<typename Property>
std::function<void(struct FUSE_STAT*)> FuseLstatReturnTest<Property>::SetPropertyImpl(Property value) {
  return [this, value] (struct FUSE_STAT *stat) {
    set(stat, value);
  };
}


#endif
