#include "testutils/FuseLstatReturnTest.h"

using ::testing::WithParamInterface;
using ::testing::Values;

class FuseLstatReturnNlinkTest: public FuseLstatReturnTest<nlink_t>, public WithParamInterface<nlink_t> {
private:
  void set(struct FUSE_STAT *stat, nlink_t value) override {
    stat->st_nlink = value;
  }
};
INSTANTIATE_TEST_CASE_P(FuseLstatReturnNlinkTest, FuseLstatReturnNlinkTest, Values(
    1,
    2,
    5,
    100
));

TEST_P(FuseLstatReturnNlinkTest, ReturnedFileNlinkIsCorrect) {
  struct FUSE_STAT result = CallDirLstatWithValue(GetParam());
  EXPECT_EQ(GetParam(), result.st_nlink);
}

TEST_P(FuseLstatReturnNlinkTest, ReturnedDirNlinkIsCorrect) {
  struct FUSE_STAT result = CallDirLstatWithValue(GetParam());
  EXPECT_EQ(GetParam(), result.st_nlink);
}

