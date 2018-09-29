#include <cmath>

static const int VX = 0, VY = 1, VZ = 2;

static const int M00 = 0, M01 = 4, M02 = 8, M03 = 12;
static const int M10 = 1, M11 = 5, M12 = 9, M13 = 13;
static const int M20 = 2, M21 = 6, M22 = 10, M23 = 14;
static const int M30 = 3, M31 = 7, M32 = 11, M33 = 15;

void setMatrix(float *m,
               float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33) {
    m[M00] = m00;
    m[M01] = m01;
    m[M02] = m02;
    m[M03] = m03;
    m[M10] = m10;
    m[M11] = m11;
    m[M12] = m12;
    m[M13] = m13;
    m[M20] = m20;
    m[M21] = m21;
    m[M22] = m22;
    m[M23] = m23;
    m[M30] = m30;
    m[M31] = m31;
    m[M32] = m32;
    m[M33] = m33;
}

void setIdentityMatrix(float *m) {
    return setMatrix(
            m,
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
}

void mulRotationMatrix(float *m, float x, float y, float z, float rad) {
    float cs = cos(rad);
    float sn = sin(rad);
    float len = x * x + y * y + z * z;
    if (0 < len) {
        len = sqrt(len);
        x /= len;
        y /= len;
        z /= len;
    }

    float cs1 = 1.0f - cs;
    float xcs1 = x * cs1, ycs1 = y * cs1;
    float xycs1 = y * xcs1, xzcs1 = z * xcs1, yzcs1 = z * ycs1;
    float xsn = x * sn, ysn = y * sn, zsn = z * sn;

    float a00 = m[M00], a01 = m[M01], a02 = m[M02];
    float a10 = m[M10], a11 = m[M11], a12 = m[M12];
    float a20 = m[M20], a21 = m[M21], a22 = m[M22];
    float a30 = m[M30], a31 = m[M31], a32 = m[M32];
    float b00 = cs + x * xcs1, b01 = xycs1 - zsn, b02 = xzcs1 + ysn;
    float b10 = xycs1 + zsn, b11 = cs + y * ycs1, b12 = yzcs1 - xsn;
    float b20 = xzcs1 - ysn, b21 = yzcs1 + xsn, b22 = cs + z * z * cs1;

    m[M00] = a00 * b00 + a01 * b10 + a02 * b20;
    m[M01] = a00 * b01 + a01 * b11 + a02 * b21;
    m[M02] = a00 * b02 + a01 * b12 + a02 * b22;
    m[M10] = a10 * b00 + a11 * b10 + a12 * b20;
    m[M11] = a10 * b01 + a11 * b11 + a12 * b21;
    m[M12] = a10 * b02 + a11 * b12 + a12 * b22;
    m[M20] = a20 * b00 + a21 * b10 + a22 * b20;
    m[M21] = a20 * b01 + a21 * b11 + a22 * b21;
    m[M22] = a20 * b02 + a21 * b12 + a22 * b22;
    m[M30] = a30 * b00 + a31 * b10 + a32 * b20;
    m[M31] = a30 * b01 + a31 * b11 + a32 * b21;
    m[M32] = a30 * b02 + a31 * b12 + a32 * b22;
}

void mulTranslateMatrix(float *m, float x, float y, float z) {
    m[M03] += m[M00] * x + m[M01] * y + m[M02] * z;
    m[M13] += m[M10] * x + m[M11] * y + m[M12] * z;
    m[M23] += m[M20] * x + m[M21] * y + m[M22] * z;
    m[M33] += m[M30] * x + m[M31] * y + m[M32] * z;
}
