#include "TextRenderer.hpp"

#include <hb-ft.h>
#include <hb.h>

#include <iostream>

#include "util/misc.hpp"

#define FONT_SIZE 36
#define MARGIN (FONT_SIZE * .5)

GlyphData::~GlyphData() {}

void printBitmap(const FT_Bitmap& bitmap) {
    std::cout << "Glyph Bitmap (" << bitmap.width << "x" << bitmap.rows
              << "):" << std::endl;
    for (unsigned int y = 0; y < bitmap.rows; y++) {
        for (unsigned int x = 0; x < bitmap.width; x++) {
            std::cout << (bitmap.buffer[y * bitmap.pitch + x] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void TextRenderer::print_bitmap_as_bytes(const FT_Bitmap& bitmap) {
    std::cout << "Glyph Bitmap Bytes (" << bitmap.width << "x" << bitmap.rows
              << "):" << std::endl;
    for (unsigned int y = 0; y < bitmap.rows; y++) {
        for (unsigned int x = 0; x < bitmap.width; x++) {
            // Print the raw byte at position (y * bitmap.pitch + x)
            // You can print this as hex or decimal based on your preference.
            unsigned char byte = bitmap.buffer[y * bitmap.pitch + x];
            // Print as hexadecimal (two digits)
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::dec;  // Switch back to decimal format for other output
}

// Constructor: Initializes FreeType and Harfbuzz
TextRenderer::TextRenderer() {
    // adapted from
    // https://github.com/harfbuzz/harfbuzz-tutorial/blob/master/hello-harfbuzz-freetype.c

    if (FT_Init_FreeType(&ft_library)) {
        throw std::runtime_error("FreeType initialization error");
    }

    if (FT_New_Face(
            ft_library,
            hookline::data_path("../../assets/Montserrat-Regular.ttf").c_str(),
            0, &ft_face)) {
        throw std::runtime_error("Font loading error");
    }

    if (FT_Set_Char_Size(ft_face, FONT_SIZE * 64, FONT_SIZE * 64, 0, 0)) {
        throw std::runtime_error("Font size error");
    }

    hb_font = hb_ft_font_create(ft_face, NULL);

    if (!hb_font) {
        throw std::runtime_error("Harfbuzz font creation error");
    }
}

// Destructor: Cleans up FreeType and Harfbuzz resources
TextRenderer::~TextRenderer() {
    hb_font_destroy(hb_font);
    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_library);
}

// Renders text and generates glyph textures
GlyphData TextRenderer::shape_text(const std::string& text) {
    // Create Harfbuzz buffer and shape the text
    hb_buffer_t* hb_buffer = hb_buffer_create();
    if (!hb_buffer_allocation_successful(hb_buffer)) {
        std::cerr << "Failed to create Harfbuzz buffer!" << std::endl;
        throw std::runtime_error("Harfbuzz buffer creation error");
    }
    hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
    hb_buffer_guess_segment_properties(hb_buffer);

    hb_shape(hb_font, hb_buffer, NULL, 0);

    // Get glyph information and positions
    unsigned int glyph_count;
    hb_glyph_info_t* glyph_info =
        hb_buffer_get_glyph_infos(hb_buffer, &glyph_count);
    hb_glyph_position_t* glyph_pos =
        hb_buffer_get_glyph_positions(hb_buffer, &glyph_count);

    // Store the information in a GlyphData struct
    GlyphData glyph_data;
    glyph_data.hb_buffer = hb_buffer;
    glyph_data.glyph_info = glyph_info;
    glyph_data.glyph_pos = glyph_pos;
    glyph_data.glyph_count = glyph_count;

    return glyph_data;
}

/**
 * Build an array of Glyphs owning the generated texture for each glyph
 */
std::vector<Glyph> TextRenderer::generate_glyph_textures(
    const GlyphData& glyph_data) {
    std::vector<Glyph> result;
    for (unsigned int i = 0; i < glyph_data.glyph_count; ++i) {
        hb_codepoint_t glyph_index = glyph_data.glyph_info[i].codepoint;

        // Load the glyph into FreeType
        if (FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for index: "
                      << glyph_index << std::endl;
            continue;
        }

        // Generate texture for the glyph
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0,
            GL_RED,  // Single-channel (grayscale) texture
            ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows, 0,
            GL_RED, GL_UNSIGNED_BYTE,
            ft_face->glyph->bitmap.buffer  // Glyph bitmap data from FreeType
        );

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store the Glyph with its texture
        result.emplace_back(Glyph{
            texture,
            glm::ivec2(ft_face->glyph->bitmap.width,
                       ft_face->glyph->bitmap.rows),
            glm::ivec2(ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top),
            static_cast<int64_t>(ft_face->glyph->advance.x) /
                64  // Convert 1/64th pixel to pixel
        });
    }
    return result;
}