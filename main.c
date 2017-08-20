#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <math.h>
#include <png.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>


typedef struct color color;
static const struct color {
    const int png_color_type;
    const int bytes_per_px;

} GRAY = { PNG_COLOR_TYPE_GRAY, 1 }
, GRAY_ALPHA = { PNG_COLOR_TYPE_GRAY_ALPHA, 2 }
, RGB = { PNG_COLOR_TYPE_RGB, 3 }
, RGBA = { PNG_COLOR_TYPE_RGBA, 4 };


void write_png_file(FILE *input, FILE *output, png_uint_32 width, png_uint_32 height, color color) {

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        exit(EXIT_FAILURE);
    }

    png_init_io(png, output);

    png_set_IHDR(
            png,
            info,
            width, height,
            8,
            color.png_color_type,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    png_size_t row_bytes = png_get_rowbytes(png, info);


    png_bytep *row_pointers = (png_bytep *) calloc(height, sizeof(png_bytep));
    for (unsigned long long y = 0; y < height; ++y) {
        row_pointers[y] = (png_byte *) malloc(row_bytes);
    }

    for (unsigned long long y = 0; y < height; ++y) {
        fread(row_pointers[y], row_bytes, 1, input);
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    for (int y = 0; y < height; ++y) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_write_struct(&png, &info);
}


png_uint_32 find_edge_px(FILE *fp, color color) {
    fseeko(fp, 0, SEEK_END);
    off_t input_size = ftello(fp);
    rewind(fp);

    double edge_px = sqrt(input_size / (double) color.bytes_per_px);

    if (edge_px > PNG_UINT_32_MAX || edge_px > 1 << 20) {
        errx(EXIT_FAILURE, "File too big to pang");
    }

    return (png_uint_32) edge_px;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: pang [-(g|ga|a)] INPUT OUTPUT\n"
                "       -a\tuse alpha"
                "       -g\tuse grayscale instead of default rgb\n");
        exit(EXIT_FAILURE);
    }

    int arg = 0;
    color color = (++arg, (argc > 3 && strcmp("-g", argv[arg]) == 0) ? GRAY
            : (strcmp("-ga", argv[arg]) == 0) ? GRAY_ALPHA
                    : (strcmp("-a", argv[arg]) == 0) ? RGBA
                            : (--arg, RGB));

    FILE *input = fopen(argv[++arg], "rb");
    if (input == NULL) {
        err(EXIT_FAILURE, argv[1]);
    }

    FILE *output = fopen(argv[++arg], "wb");
    if (output == NULL) {
        err(EXIT_FAILURE, argv[2]);
    }

    png_uint_32 edge_px = find_edge_px(input, color);

    write_png_file(input, output, edge_px, edge_px, color);

    fclose(output);
    fclose(input);

    return 0;
}
