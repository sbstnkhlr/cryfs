#pragma once
#ifndef MESSMER_FSPP_FSTEST_TESTUTILS_FSPPNODETEST_H_
#define MESSMER_FSPP_FSTEST_TESTUTILS_FSPPNODETEST_H_

#include "FileSystemTest.h"
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

class FsppNodeTestHelper {
public:
  void IN_STAT(fspp::Node *file, std::function<void (struct FUSE_STAT)> callback) {
    struct FUSE_STAT st{};
    file->stat(&st);
    callback(st);
  }

  void EXPECT_SIZE(uint64_t expectedSize, fspp::Node *node) {
    IN_STAT(node, [expectedSize] (struct FUSE_STAT st) {
      EXPECT_EQ(expectedSize, static_cast<uint64_t>(st.st_size));
    });
  }
};

/**
 * Inherit your fixture from this class to write a test case that is run on nodes, i.e. files, directories and symlinks.
 * You can use this->CreateNode() to create a node and then call fspp::Node functions on it.
 * Add your test cases as void Test_xxx() functions to your fixture and register/instantiate them using
 * REGISTER_NODE_TEST_CASE and INSTANTIATE_NODE_TEST_CASE.
 * It will then automatically create a test case for each node type (file, directory, symlink).
 * See FsppNodeTest_Rename for an example.
 */
template<class ConcreteFileSystemTestFixture>
class FsppNodeTest: public virtual FsppNodeTestHelper, public virtual FileSystemTest<ConcreteFileSystemTestFixture> {
public:
    virtual cpputils::unique_ref<fspp::Node> CreateNode(const boost::filesystem::path &path) = 0;
};

#define _REGISTER_SINGLE_NODE_TEST_CASE(r, Class, Name)                                                                 \
    TYPED_TEST_P(Class, Name) {                                                                                         \
      this->BOOST_PP_CAT(Test_,Name)();                                                                                 \
    }                                                                                                                   \

#define _REGISTER_NODE_TEST_CASES_FOR_CLASS(Class, ...)                                                                 \
    BOOST_PP_SEQ_FOR_EACH(_REGISTER_SINGLE_NODE_TEST_CASE, Class, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));               \
    REGISTER_TYPED_TEST_CASE_P(Class, __VA_ARGS__);                                                                     \

#define _REGISTER_FILE_TEST_CASE(Class, ...)                                                                            \
    template<class ConcreteFileSystemTestFixture>                                                                       \
    class Class##_FileNode: public Class<ConcreteFileSystemTestFixture>, public virtual FsppNodeTestHelper {            \
    public:                                                                                                             \
        cpputils::unique_ref<fspp::Node> CreateNode(const boost::filesystem::path &path) override {                     \
            this->CreateFile(path);                                                                                     \
            return this->Load(path);                                                                                    \
        }                                                                                                               \
    };                                                                                                                  \
    TYPED_TEST_CASE_P(Class##_FileNode);                                                                                \
    _REGISTER_NODE_TEST_CASES_FOR_CLASS(Class##_FileNode, __VA_ARGS__);                                                 \

#define _REGISTER_DIR_TEST_CASE(Class, ...)                                                                             \
    template<class ConcreteFileSystemTestFixture>                                                                       \
    class Class##_DirNode: public Class<ConcreteFileSystemTestFixture>, public virtual FsppNodeTestHelper {             \
    public:                                                                                                             \
        cpputils::unique_ref<fspp::Node> CreateNode(const boost::filesystem::path &path) override {                     \
            this->CreateDir(path);                                                                                      \
            return this->Load(path);                                                                                    \
        }                                                                                                               \
    };                                                                                                                  \
    TYPED_TEST_CASE_P(Class##_DirNode);                                                                                 \
    _REGISTER_NODE_TEST_CASES_FOR_CLASS(Class##_DirNode, __VA_ARGS__);                                                  \

#define _REGISTER_SYMLINK_TEST_CASE(Class, ...)                                                                         \
    template<class ConcreteFileSystemTestFixture>                                                                       \
    class Class##_SymlinkNode: public Class<ConcreteFileSystemTestFixture>, public virtual FsppNodeTestHelper {         \
    public:                                                                                                             \
        cpputils::unique_ref<fspp::Node> CreateNode(const boost::filesystem::path &path) override {                     \
            this->CreateSymlink(path);                                                                                  \
            return this->Load(path);                                                                                    \
        }                                                                                                               \
    };                                                                                                                  \
    TYPED_TEST_CASE_P(Class##_SymlinkNode);                                                                             \
    _REGISTER_NODE_TEST_CASES_FOR_CLASS(Class##_SymlinkNode, __VA_ARGS__);                                              \

#define REGISTER_NODE_TEST_CASE(Class, ...)                                                                             \
    _REGISTER_FILE_TEST_CASE(Class, __VA_ARGS__);                                                                       \
    _REGISTER_DIR_TEST_CASE(Class, __VA_ARGS__);                                                                        \
    _REGISTER_SYMLINK_TEST_CASE(Class, __VA_ARGS__);                                                                    \

#define INSTANTIATE_NODE_TEST_CASE(FS_NAME, Class, FIXTURE)                                                             \
    INSTANTIATE_TYPED_TEST_CASE_P(FS_NAME, Class##_FileNode, FIXTURE);                                                  \
    INSTANTIATE_TYPED_TEST_CASE_P(FS_NAME, Class##_DirNode, FIXTURE);                                                   \
    INSTANTIATE_TYPED_TEST_CASE_P(FS_NAME, Class##_SymlinkNode, FIXTURE);                                               \

#endif
