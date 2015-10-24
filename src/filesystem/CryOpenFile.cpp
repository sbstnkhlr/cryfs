#include "CryOpenFile.h"

#include <sys/types.h>
#include <fcntl.h>

#include "CryDevice.h"
#include "messmer/fspp/fuse/FuseErrnoException.h"

namespace bf = boost::filesystem;

using cpputils::unique_ref;
using cryfs::parallelaccessfsblobstore::FileBlobRef;

//TODO Get rid of this in favor of a exception hierarchy
using fspp::fuse::CHECK_RETVAL;
using fspp::fuse::FuseErrnoException;

namespace cryfs {

CryOpenFile::CryOpenFile(unique_ref<FileBlobRef> fileBlob)
: _fileBlob(std::move(fileBlob)) {
}

CryOpenFile::~CryOpenFile() {
  //TODO
}

void CryOpenFile::flush() {
  _fileBlob->flush();
}

void CryOpenFile::stat(struct ::stat *result) const {
  result->st_mode = S_IFREG | S_IRUSR | S_IXUSR | S_IWUSR;
  result->st_size = _fileBlob->size();
  return;
}

void CryOpenFile::truncate(off_t size) const {
  _fileBlob->resize(size);
}

ssize_t CryOpenFile::read(void *buf, size_t count, off_t offset) const {
  return _fileBlob->read(buf, offset, count);
}

void CryOpenFile::write(const void *buf, size_t count, off_t offset) {
  _fileBlob->write(buf, offset, count);
}

void CryOpenFile::fsync() {
  //throw FuseErrnoException(ENOTSUP);
}

void CryOpenFile::fdatasync() {
  //throw FuseErrnoException(ENOTSUP);
}

}