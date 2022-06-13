#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

/*
 * load file to buffer
 *          ||
 *          \/
 *     xor encrypt
 *          ||
 *          \/
 * write buffer to file
 */

int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./xorcrpt <filename> <key>\n");
        return 1;
    }

    setlocale(LC_ALL, "");

    FILE* srcf;       /* source file */
    char* buf;        /* buffer for file */
    size_t buf_size;  /* size of file */
    const char* key;  /* (en/de)cryption key */
    size_t i;

    srcf = fopen(argv[1], "rb");
    if (!srcf)
    {
        fprintf(stderr, "srcf rb is NULL\n");
        return 1;
    }

    /* loading file to buffer */
    fseek(srcf, 0, SEEK_END);
    buf_size = ftell(srcf);
    fseek(srcf, 0, SEEK_SET);
    buf = (char*)malloc(buf_size);
    if (!buf)
    {
        fprintf(stderr, "buf is NULL\n");
        fclose(srcf);
        return 1;
    }
    if (fread(buf, 1, buf_size, srcf) != buf_size)
    {
        fprintf(stderr, "fread != buf_size\n");
        fclose(srcf);
        free(buf);
        return 1;
    }
    fclose(srcf);

    key = argv[2];


    /* xor encryption alg */
    for (i = 0; i < buf_size; ++i)
    {
        buf[i] ^= *key++;
        if (!*key)
            key = argv[2];
    }

    /* writing buffer to file */
    srcf = fopen(argv[1], "wb");
    if (!srcf)
    {
        fprintf(stderr, "srcf wb is NULL\n");
        free(buf);
        return 1;
    }

    if (fwrite(buf, 1, buf_size, srcf) != buf_size)
    {
        fprintf(stderr, "fwrite != buf_size\n");
        fclose(srcf);
        free(buf);
        return 1;
    }
    fclose(srcf);
    free(buf);

    return 0;
}