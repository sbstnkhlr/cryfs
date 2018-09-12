#pragma once
#ifndef MESSMER_CRYFS_FILESYSTEM_PARALLELACCESSFSBLOBSTORE_FILEBLOBREF_H
#define MESSMER_CRYFS_FILESYSTEM_PARALLELACCESSFSBLOBSTORE_FILEBLOBREF_H

#include "FsBlobRef.h"
#include "../cachingfsblobstore/FileBlobRef.h"

namespace cryfs {
namespace parallelaccessfsblobstore {

class FileBlobRef final: public FsBlobRef {
public:
    FileBlobRef(cachingfsblobstore::FileBlobRef *base) : _base(base) {}

    void resize(off_t size) {
        return _base->resize(size);
    }

    off_t size() const {
        return _base->size();
    }

    size_t read(void *target, uint64_t offset, uint64_t count) const {
        return _base->read(target, offset, count);
    }

    void write(const void *source, uint64_t offset, uint64_t count) {
        return _base->write(source, offset, count);
    }

    void flush() {
        return _base->flush();
    }

    const blockstore::BlockId &blockId() const override {
        return _base->blockId();
    }

    off_t lstat_size() const override {
        return _base->lstat_size();
    }

    const blockstore::BlockId &parentPointer() const override {
        return _base->parentPointer();
    }

    void setParentPointer(const blockstore::BlockId &parentId) override {
        return _base->setParentPointer(parentId);
    }

private:
    cachingfsblobstore::FileBlobRef *_base;

    DISALLOW_COPY_AND_ASSIGN(FileBlobRef);
};

}
}

#endif
