/* Copyright (C) 2015 Hans-Kristian Arntzen <maister@archlinux.us>
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "glfft_common.hpp"

using namespace std;
using namespace GLFFT;

Texture::~Texture()
{
    if (name)
    {
        opengl_check(glDeleteTextures(1, &name));
    }
}

Texture::Texture(GLuint tex)
    : name(tex)
{}

void Texture::init(unsigned width, unsigned height, unsigned levels, GLenum internal_format,
    GLenum wrap_s, GLenum wrap_t, GLenum min_filter, GLenum mag_filter)
{
    if (name)
    {
        opengl_check(glDeleteTextures(1, &name));
    }
    opengl_check(glGenTextures(1, &name));
    opengl_check(glBindTexture(GL_TEXTURE_2D, name));
    opengl_check(glTexStorage2D(GL_TEXTURE_2D, levels, internal_format, width, height));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
    opengl_check(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
    opengl_check(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::upload(const void* data, GLenum format, GLenum type,
    unsigned x_off, unsigned y_off, unsigned width, unsigned height)
{
    if (!name)
    {
        throw logic_error("Cannot upload to null-texture.");
    }
    opengl_check(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));
    opengl_check(glBindTexture(GL_TEXTURE_2D, name));
    opengl_check(glTexSubImage2D(GL_TEXTURE_2D, 0, x_off, y_off, width, height, format, type, data));
    opengl_check(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture& Texture::operator=(Texture&& texture)
{
    if (this != &texture)
    {
        if (name)
        {
            opengl_check(glDeleteTextures(1, &name));
        }
        name = texture.name;
        texture.name = 0;
    }
    return *this;
}

Texture::Texture(Texture&& texture)
{
    *this = move(texture);
}

Buffer::~Buffer()
{
    if (name)
    {
        opengl_check(glDeleteBuffers(1, &name));
    }
}

Buffer::Buffer(GLuint buffer)
    : name(buffer)
{}

void Buffer::init(const void* data, size_t size, GLenum access)
{
    if (name)
    {
        opengl_check(glDeleteBuffers(1, &name));
    }
    opengl_check(glGenBuffers(1, &name));
    opengl_check(glBindBuffer(GL_SHADER_STORAGE_BUFFER, name));
    opengl_check(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, access));
}

Buffer& Buffer::operator=(Buffer&& buffer)
{
    if (this != &buffer)
    {
        if (name)
        {
            opengl_check(glDeleteBuffers(1, &name));
        }
        name = buffer.name;
        buffer.name = 0;
    }
    return *this;
}

Buffer::Buffer(Buffer&& buffer)
{
    *this = move(buffer);
}

Program::Program(GLuint prog)
    : name(prog)
{}

Program::~Program()
{
    if (name)
    {
        opengl_check(glDeleteProgram(name));
    }
}

Program& Program::operator=(Program&& prog)
{
    if (this != &prog)
    {
        if (name)
        {
            opengl_check(glDeleteProgram(name));
        }
        name = prog.name;
        prog.name = 0;
    }
    return *this;
}

Program::Program(Program&& prog)
{
    *this = move(prog);
}

