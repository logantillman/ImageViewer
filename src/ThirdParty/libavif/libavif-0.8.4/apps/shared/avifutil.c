// Copyright 2019 Joe Drago. All rights reserved.
// SPDX-License-Identifier: BSD-2-Clause

#include "avifutil.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

void avifImageDump(avifImage * avif)
{
    printf(" * Resolution     : %dx%d\n", avif->width, avif->height);
    printf(" * Bit Depth      : %d\n", avif->depth);
    printf(" * Format         : %s\n", avifPixelFormatToString(avif->yuvFormat));
    printf(" * Alpha          : %s\n", (avif->alphaPlane && (avif->alphaRowBytes > 0)) ? "Present" : "Absent");
    printf(" * Range          : %s\n", (avif->yuvRange == AVIF_RANGE_FULL) ? "Full" : "Limited");

    printf(" * Color Primaries: %d\n", avif->colorPrimaries);
    printf(" * Transfer Char. : %d\n", avif->transferCharacteristics);
    printf(" * Matrix Coeffs. : %d\n", avif->matrixCoefficients);

    printf(" * ICC Profile    : %s (" AVIF_FMT_ZU " bytes)\n", (avif->icc.size > 0) ? "Present" : "Absent", avif->icc.size);
    printf(" * XMP Metadata   : %s (" AVIF_FMT_ZU " bytes)\n", (avif->xmp.size > 0) ? "Present" : "Absent", avif->xmp.size);
    printf(" * EXIF Metadata  : %s (" AVIF_FMT_ZU " bytes)\n", (avif->exif.size > 0) ? "Present" : "Absent", avif->exif.size);

    if (avif->transformFlags == AVIF_TRANSFORM_NONE) {
        printf(" * Transformations: None\n");
    } else {
        printf(" * Transformations:\n");

        if (avif->transformFlags & AVIF_TRANSFORM_PASP) {
            printf("    * pasp (Aspect Ratio)  : %d/%d\n", (int)avif->pasp.hSpacing, (int)avif->pasp.vSpacing);
        }
        if (avif->transformFlags & AVIF_TRANSFORM_CLAP) {
            printf("    * clap (Clean Aperture): W: %d/%d, H: %d/%d, hOff: %d/%d, vOff: %d/%d\n",
                   (int)avif->clap.widthN,
                   (int)avif->clap.widthD,
                   (int)avif->clap.heightN,
                   (int)avif->clap.heightD,
                   (int)avif->clap.horizOffN,
                   (int)avif->clap.horizOffD,
                   (int)avif->clap.vertOffN,
                   (int)avif->clap.vertOffD);
        }
        if (avif->transformFlags & AVIF_TRANSFORM_IROT) {
            printf("    * irot (Rotation)      : %u\n", avif->irot.angle);
        }
        if (avif->transformFlags & AVIF_TRANSFORM_IMIR) {
            printf("    * imir (Mirror)        : %u (%s)\n",
                   avif->imir.axis,
                   (avif->imir.axis == 0) ? "Vertical axis, \"left-to-right\"" : "Horizontal axis, \"top-to-bottom\"");
        }
    }
}

void avifPrintVersions(void)
{
    char codecVersions[256];
    avifCodecVersions(codecVersions);
    printf("Version: %s (%s)\n", avifVersion(), codecVersions);

    unsigned int libyuvVersion = avifLibYUVVersion();
    if (libyuvVersion == 0) {
        printf("libyuv : unavailable\n");
    } else {
        printf("libyuv : available (%u)\n", libyuvVersion);
    }

    printf("\n");
}

avifAppFileFormat avifGuessFileFormat(const char * filename)
{
    // Guess from the file header
    FILE * f = fopen(filename, "rb");
    if (f) {
        uint8_t headerBuffer[144];
        size_t bytesRead = fread(headerBuffer, 1, sizeof(headerBuffer), f);
        fclose(f);

        if (bytesRead > 0) {
            avifROData header;
            header.data = headerBuffer;
            header.size = bytesRead;

            if (avifPeekCompatibleFileType(&header)) {
                return AVIF_APP_FILE_FORMAT_AVIF;
            }

            static const uint8_t signatureJPEG[2] = { 0xFF, 0xD8 };
            static const uint8_t signaturePNG[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
            static const uint8_t signatureY4M[9] = { 0x59, 0x55, 0x56, 0x34, 0x4D, 0x50, 0x45, 0x47, 0x32 }; // "YUV4MPEG2"
            struct avifHeaderSignature
            {
                avifAppFileFormat format;
                const uint8_t * magic;
                size_t magicSize;
            } signatures[] = { { AVIF_APP_FILE_FORMAT_JPEG, signatureJPEG, sizeof(signatureJPEG) },
                               { AVIF_APP_FILE_FORMAT_PNG, signaturePNG, sizeof(signaturePNG) },
                               { AVIF_APP_FILE_FORMAT_Y4M, signatureY4M, sizeof(signatureY4M) } };
            const size_t signaturesCount = sizeof(signatures) / sizeof(signatures[0]);

            for (size_t signatureIndex = 0; signatureIndex < signaturesCount; ++signatureIndex) {
                struct avifHeaderSignature * signature = &signatures[signatureIndex];
                if (header.size < signature->magicSize) {
                    continue;
                }
                if (!memcmp(header.data, signature->magic, signature->magicSize)) {
                    return signature->format;
                }
            }

            // If none of these signatures match, bail out here. Guessing by extension won't help.
            return AVIF_APP_FILE_FORMAT_UNKNOWN;
        }
    }

    // If we get here, the file header couldn't be read for some reason. Guess from the extension.

    const char * fileExt = strrchr(filename, '.');
    if (!fileExt) {
        return AVIF_APP_FILE_FORMAT_UNKNOWN;
    }
    ++fileExt; // skip past the dot

    char lowercaseExt[8]; // This only needs to fit up to "jpeg", so this is plenty
    const size_t fileExtLen = strlen(fileExt);
    if (fileExtLen >= sizeof(lowercaseExt)) { // >= accounts for NULL terminator
        return AVIF_APP_FILE_FORMAT_UNKNOWN;
    }

    for (size_t i = 0; i < fileExtLen; ++i) {
        lowercaseExt[i] = (char)tolower((unsigned char)fileExt[i]);
    }
    lowercaseExt[fileExtLen] = 0;

    if (!strcmp(lowercaseExt, "avif")) {
        return AVIF_APP_FILE_FORMAT_AVIF;
    } else if (!strcmp(lowercaseExt, "y4m")) {
        return AVIF_APP_FILE_FORMAT_Y4M;
    } else if (!strcmp(lowercaseExt, "jpg") || !strcmp(lowercaseExt, "jpeg")) {
        return AVIF_APP_FILE_FORMAT_JPEG;
    } else if (!strcmp(lowercaseExt, "png")) {
        return AVIF_APP_FILE_FORMAT_PNG;
    }
    return AVIF_APP_FILE_FORMAT_UNKNOWN;
}