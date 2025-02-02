///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


#ifndef INCLUDED_IMF_ZIP_H
#define INCLUDED_IMF_ZIP_H

#include "ImfNamespace.h"
#include "ImfExport.h"

#include <cstddef>

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_ENTER

class Zip
{
    public:
        IMF_EXPORT
        explicit Zip(size_t rawMaxSize);
        IMF_EXPORT
        Zip(size_t maxScanlineSize, size_t numScanLines);
        IMF_EXPORT
        ~Zip();

#if __cplusplus >= 201103L
        Zip (const Zip& other) = delete;
        Zip& operator = (const Zip& other) = delete;
        Zip (Zip&& other) = delete;
        Zip& operator = (Zip&& other) = delete;
#else
      private:
        Zip (const Zip& other);
        Zip& operator = (const Zip& other);
      public:
#endif

        IMF_EXPORT
        size_t maxRawSize();
        IMF_EXPORT
        size_t maxCompressedSize();

        //
        // Compress the raw data into the provided buffer.
        // Returns the amount of compressed data.
        //
        IMF_EXPORT
        int compress(const char *raw, int rawSize, char *compressed);

        // 
        // Uncompress the compressed data into the provided
        // buffer. Returns the amount of raw data actually decoded.
        //
        IMF_EXPORT
        int uncompress(const char *compressed, int compressedSize,
                                                 char *raw);

    private:
        size_t _maxRawSize;
        char  *_tmpBuffer;
};

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_EXIT

#endif
