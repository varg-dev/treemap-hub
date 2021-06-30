import { Renderer } from '../source/renderer';

describe('Renderer', () => {
    it('should initialize instance correctly', () => {
        const canvas = document.createElement('canvas');
        const renderer = new Renderer(canvas);

        expect(renderer).toBeInstanceOf(Renderer);
    });
});
