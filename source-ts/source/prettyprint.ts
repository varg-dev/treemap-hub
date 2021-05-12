
export function pad(maxDigits: number, value: number): string {
    const v = Math.round(value);
    const digits = v < 1 ? 1 : Math.floor(Math.log10(v)) + 1;
    return ' '.repeat(maxDigits - digits) + value.toString();
}
