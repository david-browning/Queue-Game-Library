Floating Point Importers
========================

## Description:
Floats and doubles may be interpreted on different platforms. To keep floating
point numbers consistent across content files, they are encoded as a rational
number.

## Binary Layout:
Byte 0: Number of bytes large the numerator is.
Bytes 1 through N: Numerator encoded as a signed integer.
Bytes N + 1 though 2N: Numerator encoded as a signed integer.

## Importer:
Specialize qgl::content::loaders::rational_importer<T>.