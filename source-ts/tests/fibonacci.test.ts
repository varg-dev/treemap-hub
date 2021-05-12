
import { Fibonacci } from '../source/fibonacci';


describe('Fibonacci', () => {

    it('should work for small values', () => {

        expect(Fibonacci.get(0)).toEqual(0);
        expect(Fibonacci.get(1)).toEqual(1);
        expect(Fibonacci.get(2)).toEqual(1);
        expect(Fibonacci.get(3)).toEqual(2);
        expect(Fibonacci.get(4)).toEqual(3);
        expect(Fibonacci.get(5)).toEqual(5);
        expect(Fibonacci.get(5)).toEqual(5);

    });

    it('should work for large values', () => {

        expect(Fibonacci.get(100)).toEqual(354224848179261915075);
        expect(Fibonacci.get(200)).toEqual(280571172992510140037611932413038677189525);

    });

    it('cannot retrieve the 501th fibonacci number', () => {

        expect(Fibonacci.get(501)).toBeUndefined();

    });

});
