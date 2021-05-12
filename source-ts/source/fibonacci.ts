
// import { Foo } from './foo';
// import { Bar } from '../moep/bar';


/**
 * Fibonacci class for no reason, wrapping the Fibonacci number computation.
 */
export class Fibonacci {

    protected static data: any = require('../data/first500.json');

    /**
     * Tail recursive computation of the nth fibonacci number.
     *
     * @param i - The index of the Fibonacci number to look up.
     * @param val - Sum of value and previous value of caller.
     * @param prev - Value of caller passed to callee.
     */
    protected static compute(i: number, val: number = 1, prev: number = 0): number {
        switch (i) {
        case 0:
            return prev;
        case 1:
            return val;
        default:
            return Fibonacci.compute(i - 1, val + prev, val);
        }
    }

    /**
     * Instead of computation the ith fibonacci number is read from data.
     *
     * @param i - The index of the Fibonacci number to look up.
     */
    protected static lookup(i: number): number {
        return i > 500 ? undefined : parseInt(Fibonacci.data.fibonacci500[i], 10);
    }

    /**
     * Computes a specific Fibonacci number.
     *
     * @param i - The index of the Fibonacci number to compute.
     *
     * @returns the ith Fibonacci number as number.
     */
    static get(i: number): number {
        return i < 64 ? Fibonacci.compute(i) : Fibonacci.lookup(i);
    }

}
