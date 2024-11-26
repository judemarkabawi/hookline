/**
 * @file TextRenderer.hpp
 * @author Dante Liang, Jude Markabawi
 *
 * Most code adapted from Dante at
 * https://github.com/ZySerika/15-466-f24-base4/blob/main/hb-ftt.hpp.
 */

#pragma once

#include <GL/glew.h>
#include <hb-ft.h>
#include <hb.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

// A struct to hold glyph info and positions
struct GlyphData {
    hb_buffer_t* hb_buffer;
    hb_glyph_info_t* glyph_info;     // pointer into hb_buffer data
    hb_glyph_position_t* glyph_pos;  // pointer into hb_buffer data
    unsigned int glyph_count;

    GlyphData() = default;
    ~GlyphData();
};

struct Glyph {
    GLuint texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    int64_t advance;
};

// Class that handles the text rendering logic
class TextRenderer {
   public:
    // Constructor: loads the font file and sets up FreeType and Harfbuzz
    TextRenderer();

    // Destructor: cleans up FreeType and Harfbuzz resources
    ~TextRenderer();

    // Process the text and generate OpenGL textures for each glyph
    GlyphData shape_text(const std::string& text);
    std::vector<Glyph> generate_glyph_textures(const GlyphData& glyph_data);

    void print_bitmap_as_bytes(const FT_Bitmap& bitmap);

    FT_Library ft_library;  // FreeType library
    FT_Face ft_face;        // FreeType font face
    hb_font_t* hb_font;     // Harfbuzz font
};
