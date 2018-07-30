#include "FuseLstatTest.h"

using std::function;
using ::testing::StrEq;
using ::testing::_;
using ::testing::Invoke;

void FuseLstatTest::LstatPath(const std::string &path) {
  struct FUSE_STAT dummy{};
  LstatPath(path, &dummy);
}

int FuseLstatTest::LstatPathReturnError(const std::string &path) {
  struct FUSE_STATFUSE_STAT dummy{};
  return LstatPathReturnError(path, &dummy);
}

void FuseLstatTest::LstatPath(const std::string &path, struct FUSE_STATFUSE_STAT *result) {
  int error = LstatPathReturnError(path, result);
  EXPECT_EQ(0, error) << "lstat syscall failed. errno: " << error;
}

int FuseLstatTest::LstatPathReturnError(const std::string &path, struct FUSE_STATFUSE_STAT *result) {
  auto fs = TestFS();

  auto realpath = fs->mountDir() / path;
  int retval = ::lstat(realpath.string().c_str(), result);
  if (retval == 0) {
    return 0;
  } else {
    return errno;
  }
}

struct FUSE_STATFUSE_STAT FuseLstatTest::CallFileLstatWithImpl(function<void(struct FUSE_STATFUSE_STAT*)> implementation) {
  return CallLstatWithModeAndImpl(S_IFREG, implementation);
}

struct FUSE_STATFUSE_STAT FuseLstatTest::CallDirLstatWithImpl(function<void(struct FUSE_STATFUSE_STAT*)> implementation) {
  return CallLstatWithModeAndImpl(S_IFDIR, implementation);
}

struct FUSE_STATFUSE_STAT FuseLstatTest::CallLstatWithImpl(function<void(struct FUSE_STATFUSE_STAT*)> implementation) {
  EXPECT_CALL(fsimpl, lstat(StrEq(FILENAME), _)).WillRepeatedly(Invoke([implementation](const char*, struct FUSE_STAT *stat) {
    implementation(stat);
  }));

  struct FUSE_STATFUSE_STAT result{};
  LstatPath(FILENAME, &result);

  return result;
}

struct FUSE_STATFUSE_STAT FuseLstatTest::CallLstatWithModeAndImpl(mode_t mode, function<void(struct FUSE_STATFUSE_STAT*)> implementation) {
  return CallLstatWithImpl([mode, implementation] (struct FUSE_STATFUSE_STAT *stat) {
    stat->st_mode = mode;
    implementation(stat);
  });
}
