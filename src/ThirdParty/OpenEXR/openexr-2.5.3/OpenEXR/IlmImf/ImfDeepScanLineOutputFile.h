///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2011, Industrial Light & Magic, a division of Lucas
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



#ifndef INCLUDED_IMF_DEEP_SCAN_LINE_OUTPUT_FILE_H
#define INCLUDED_IMF_DEEP_SCAN_LINE_OUTPUT_FILE_H

//-----------------------------------------------------------------------------
//
//      class DeepScanLineOutputFile
//
//-----------------------------------------------------------------------------

#include "ImfHeader.h"
#include "ImfFrameBuffer.h"
#include "ImfThreading.h"
#include "ImfGenericOutputFile.h"
#include "ImfNamespace.h"
#include "ImfForward.h"
#include "ImfExport.h"

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_ENTER


struct PreviewRgba;

class DeepScanLineOutputFile : public GenericOutputFile
{
  public:

    //-----------------------------------------------------------
    // Constructor -- opens the file and writes the file header.
    // The file header is also copied into the DeepScanLineOutputFile
    // object, and can later be accessed via the header() method.
    // Destroying this DeepScanLineOutputFile object automatically closes
    // the file.
    //
    // numThreads determines the number of threads that will be
    // used to write the file (see ImfThreading.h).
    //-----------------------------------------------------------

    IMF_EXPORT
    DeepScanLineOutputFile (const char fileName[], const Header &header,
                int numThreads = globalThreadCount());


    //------------------------------------------------------------
    // Constructor -- attaches the new DeepScanLineOutputFile object
    // to a file that has already been opened, and writes the file header.
    // The file header is also copied into the DeepScanLineOutputFile
    // object, and can later be accessed via the header() method.
    // Destroying this DeepScanLineOutputFile object does not automatically
    // close the file.
    //
    // numThreads determines the number of threads that will be
    // used to write the file (see ImfThreading.h).
    //------------------------------------------------------------

    IMF_EXPORT
    DeepScanLineOutputFile (OPENEXR_IMF_INTERNAL_NAMESPACE::OStream &os, const Header &header,
                int numThreads = globalThreadCount());


    //-------------------------------------------------
    // Destructor
    //
    // Destroying the DeepScanLineOutputFile object
    // before writing all scan lines within the data
    // window results in an incomplete file.
    //-------------------------------------------------

    IMF_EXPORT
    virtual ~DeepScanLineOutputFile ();


    //------------------------
    // Access to the file name
    //------------------------

    IMF_EXPORT
    const char *        fileName () const;


    //--------------------------
    // Access to the file header
    //--------------------------

    IMF_EXPORT
    const Header &      header () const;


    //-------------------------------------------------------
    // Set the current frame buffer -- copies the FrameBuffer
    // object into the OutputFile object.
    //
    // The current frame buffer is the source of the pixel
    // data written to the file.  The current frame buffer
    // must be set at least once before writePixels() is
    // called.  The current frame buffer can be changed
    // after each call to writePixels.
    //-------------------------------------------------------

    IMF_EXPORT
    void                setFrameBuffer (const DeepFrameBuffer &frameBuffer);


    //-----------------------------------
    // Access to the current frame buffer
    //-----------------------------------

    IMF_EXPORT
    const DeepFrameBuffer & frameBuffer () const;


    //-------------------------------------------------------------------
    // Write pixel data:
    //
    // writePixels(n) retrieves the next n scan lines worth of data from
    // the current frame buffer, starting with the scan line indicated by
    // currentScanLine(), and stores the data in the output file, and
    // progressing in the direction indicated by header.lineOrder().
    //
    // To produce a complete and correct file, exactly m scan lines must
    // be written, where m is equal to
    // header().dataWindow().max.y - header().dataWindow().min.y + 1.
    //-------------------------------------------------------------------

    IMF_EXPORT
    void                writePixels (int numScanLines = 1);


    //------------------------------------------------------------------
    // Access to the current scan line:
    //
    // currentScanLine() returns the y coordinate of the first scan line
    // that will be read from the current frame buffer during the next
    // call to writePixels().
    //
    // If header.lineOrder() == INCREASING_Y:
    //
    //  The current scan line before the first call to writePixels()
    //  is header().dataWindow().min.y.  After writing each scan line,
    //  the current scan line is incremented by 1.
    //
    // If header.lineOrder() == DECREASING_Y:
    //
    //  The current scan line before the first call to writePixels()
    //  is header().dataWindow().max.y.  After writing each scan line,
    //  the current scan line is decremented by 1.
    //
    //------------------------------------------------------------------

    IMF_EXPORT
    int                 currentScanLine () const;


    //--------------------------------------------------------------
    // Shortcut to copy all pixels from an InputFile into this file,
    // without uncompressing and then recompressing the pixel data.
    // This file's header must be compatible with the InputFile's
    // header:  The two header's "dataWindow", "compression",
    // "lineOrder" and "channels" attributes must be the same.
    //--------------------------------------------------------------

    IMF_EXPORT
    void                copyPixels (DeepScanLineInputFile &in);
    
    // --------------------------------------------------------------
    // Shortcut to copy pixels from a given part of a multipart file
    // --------------------------------------------------------------
    IMF_EXPORT
    void                copyPixels (DeepScanLineInputPart &in);


    //--------------------------------------------------------------
    // Updating the preview image:
    //
    // updatePreviewImage() supplies a new set of pixels for the
    // preview image attribute in the file's header.  If the header
    // does not contain a preview image, updatePreviewImage() throws
    // an IEX_NAMESPACE::LogicExc.
    //
    // Note: updatePreviewImage() is necessary because images are
    // often stored in a file incrementally, a few scan lines at a
    // time, while the image is being generated.  Since the preview
    // image is an attribute in the file's header, it gets stored in
    // the file as soon as the file is opened, but we may not know
    // what the preview image should look like until we have written
    // the last scan line of the main image.
    //
    //--------------------------------------------------------------

    IMF_EXPORT
    void                updatePreviewImage (const PreviewRgba newPixels[]);


    struct Data;

  private:

    //------------------------------------------------------------
    // Constructor -- attaches the OutputStreamMutex to the
    // given one from MultiPartOutputFile. Set the previewPosition
    // and lineOffsetsPosition which have been acquired from
    // the constructor of MultiPartOutputFile as well.
    //------------------------------------------------------------
    DeepScanLineOutputFile (const OutputPartData* part);

#if __cplusplus >= 201103L
    DeepScanLineOutputFile (const DeepScanLineOutputFile &) = delete;
    DeepScanLineOutputFile & operator = (const DeepScanLineOutputFile &) = delete;
    DeepScanLineOutputFile (DeepScanLineOutputFile &&) = delete;
    DeepScanLineOutputFile & operator = (DeepScanLineOutputFile &&) = delete;
#else
    DeepScanLineOutputFile (const DeepScanLineOutputFile &);
    DeepScanLineOutputFile & operator = (const DeepScanLineOutputFile &);
#endif

    void                initialize (const Header &header);
    void                initializeLineBuffer();

    Data *              _data;



    friend class MultiPartOutputFile;
};

OPENEXR_IMF_INTERNAL_NAMESPACE_HEADER_EXIT

#endif
