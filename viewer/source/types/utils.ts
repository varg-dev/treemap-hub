export type TypedArray =
    | Float32Array
    | Float64Array
    | Int8Array
    | Int16Array
    | Int32Array
    | Uint8Array
    | Uint16Array
    | Uint32Array;

export type TypedArrayConstructor =
    | Float32ArrayConstructor
    | Float64ArrayConstructor
    | Int8ArrayConstructor
    | Int16ArrayConstructor
    | Int32ArrayConstructor
    | Uint8ArrayConstructor
    | Uint16ArrayConstructor
    | Uint32ArrayConstructor;

type Preset = [string, string]; // tuple semantic: [vertex shader, fragment shader]

export type RgbColor = [number, number, number];

/**
 * This is a constrained identity function (CIF) for the rendering presets.
 */
export const createPresets = <Presets extends Record<string, Preset>>(presets: Presets): Presets =>
    presets;
