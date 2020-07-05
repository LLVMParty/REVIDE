#ifndef LZSTRING_H
#define LZSTRING_H

/*
 * Qt implementation of LZ-String, version 1.4.4.
 *
 * Based on the LZ-String compression algorithm found here:
 * http://pieroxy.net/blog/pages/lz-string/index.html
 *
 * WTFPL Licence
 *
 */

#include <QString>

class LZString
{
public:
    static QString compress(const QString &uncompressed);
    static QString compressToUTF16(const QString &uncompressed);
    static QString compressToBase64(const QString &uncompressed);

    static QString decompress(const QString &compressed);
    static QString decompressFromUTF16(const QString &compressed);
    static QString decompressFromBase64(const QString &compressed);

private:
    template <typename GetCharFromInt>
    static QString _compress(const QString &uncompressed, int bitsPerChar, GetCharFromInt getCharFromInt);

    template <typename GetNextValue>
    static QString _decompress(int length, int resetValue, GetNextValue getNextValue);
};

#endif // LZSTRING_H
