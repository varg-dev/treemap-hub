import type { TypedArray, TypedArrayConstructor } from '../types/utils';

export const chooseArray = (data: TypedArray): TypedArrayConstructor => {
    if (data instanceof Float32Array) {
        return Float32Array;
    } else if (data instanceof Float64Array) {
        return Float64Array;
    } else if (data instanceof Int8Array) {
        return Int8Array;
    } else if (data instanceof Int16Array) {
        return Int16Array;
    } else if (data instanceof Int32Array) {
        return Int32Array;
    } else if (data instanceof Uint8Array) {
        return Uint8Array;
    } else if (data instanceof Uint16Array) {
        return Uint16Array;
    } else if (data instanceof Uint32Array) {
        return Uint32Array;
    }
};
