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



#ifndef INCLUDED_IMF_RLE_COMPRESSOR_H
#define INCLUDED_IMF_RLE_COMPRESSOR_H

//-----------------------------------------------------------------------------
//
//	class RleCompressor -- performs run-length encoding
//
//-----------------------------------------------------------------------------

#include "ImfCompressor.h"
#include "ImfNamespace.h"

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_ENTER


class RleCompressor: public Compressor
{
  public:

    IMF_EXPORT
    RleCompressor (const Header &hdr, size_t maxScanLineSize);
    IMF_EXPORT
    virtual ~RleCompressor ();

#if __cplusplus >= 201103L
    RleCompressor (const RleCompressor& other) = delete;
    RleCompressor& operator = (const RleCompressor& other) = delete;
    RleCompressor (RleCompressor&& other) = delete;
    RleCompressor& operator = (RleCompressor&& other) = delete;
#else
  private:
    RleCompressor (const RleCompressor& other);
    RleCompressor& operator = (const RleCompressor& other);
  public:
#endif

    IMF_EXPORT
    virtual int numScanLines () const;

    IMF_EXPORT
    virtual int	compress (const char *inPtr,
			  int inSize,
			  int minY,
			  const char *&outPtr);

    IMF_EXPORT
    virtual int	uncompress (const char *inPtr,
			    int inSize,
			    int minY,
			    const char *&outPtr);
  private:

    int		_maxScanLineSize;
    char *	_tmpBuffer;
    char *	_outBuffer;
};


OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_EXIT

#endif
