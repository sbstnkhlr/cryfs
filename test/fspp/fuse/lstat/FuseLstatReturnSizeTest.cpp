#include "testutils/FuseLstatReturnTest.h"

using ::testing::WithParamInterface;
using ::testing::Values;

class FuseLstatReturnSizeTest: public FuseLstatReturnTest<off_t>, public WithParamInterface<off_t> {
private:
  void set(struct FUSE_STAT *stat, off_t value) override {
    stat->st_size = value;
  }
};
INSTANTIATE_TEST_CASE_P(FuseLstatReturnSizeTest, FuseLstatReturnSizeTest, Values(
    0,
    1,
    4096,
    1024*1024*1024
));

TEST_P(FuseLstatReturnSizeTest, ReturnedFileSizeIsCorrect) {
  struct FUSE_STAT result = CallDirLstatWithValue(GetParam());
  EXPECT_EQ(GetParam(), result.st_size);
}

TEST_P(FuseLstatReturnSizeTest, ReturnedDirSizeIsCorrect) {
  struct FUSE_STAT result = CallDirLstatWithValue(GetParam());
  EXPECT_EQ(GetParam(), result.st_size);
}
