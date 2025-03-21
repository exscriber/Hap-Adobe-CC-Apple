#pragma once

#include <stdint.h>

#include "codec.hpp"

inline int roundUpToMultipleOf4(int n) { return (n + 3) & ~3; }
inline int roundDownToMultipleOf4(int n) { return n & ~3; }

void convertHostFrameTo_RGBA_Top_Left_U8(const uint8_t* data, size_t stride, const FrameDef& frameDef, uint8_t* dest, size_t destStrideInBytes);
void convertRGBA_Top_Left_U8_ToHostFrame(const uint8_t* source, uint8_t* data, size_t stride, const FrameDef& frameDef);
void convertHostFrameTo_RGBA_Top_Left_U16(const uint8_t* data, size_t stride, const FrameDef& frameDef, uint16_t* dest, size_t destStrideInBytes);
void convertRGBA_Top_Left_U16_ToHostFrame(const uint16_t* source, uint8_t* data, size_t stride, const FrameDef& frameDef);
