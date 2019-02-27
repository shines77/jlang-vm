
#include "jlang/fs/FileName.h"

using namespace jlang::fs;

FileName::FileName(const FilePath & src) :
    filename_(src.path())
{
    // Do nothing!!
}

void FileName::setPath(const FilePath & filename)
{
    this->filename_ = filename.path();
}

SourceFileName::SourceFileName(const FilePath & src)
    : filename_(src.path()), orginal_filename_(src.path()), is_same_(true)
{
    // Do nothing!!
}

void SourceFileName::setPath(const FilePath & filename)
{
    this->filename_ = filename.path();
    this->orginal_filename_ = filename.path();
    this->is_same_ = true;
}
