#include "testutils/FuseLstatReturnTest.h"

using ::testing::WithParamInterface;
using ::testing::Values;

class FuseLstatReturnUidTest: public FuseLstatReturnTest<uid_t>, public WithParamInterface<uid_t> {
private:
  void set(struct FUSE_STAT *stat, uid_t value) override {
    stat->st_uid = value;
  }
};
INSTANTIATE_TEST_CASE_P(FuseLstatReturnUidTest, FuseLstatReturnUidTest, Values(
    0,
    10
));

TEST_P(FuseLstatReturnUidTest, ReturnedFileUidIsCorrect) {
  struct FUSE_STAT result = CallFileLstatWithValue(GetParam());
  EXPECT_EQ(GetParam(), result.st_uid);
}

TEST_P(FuseLstatReturnUidTest, ReturnedDirUidIsCorrect) {
  struct FUSE_STAT result = CallDirLstatWithValue(GetParam());
  EXPECT_EQ(GetParam(), result.st_uid);
}
