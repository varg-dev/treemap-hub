import { Renderer } from "../source/renderer";

describe("Renderer", () => {
    it("should initialize instance correctly", () => {
        const canvas = jasmine.createSpyObj("canvas", []);
        const renderer = new Renderer(canvas);

        expect(renderer).toBeInstanceOf(Renderer);
    });
});
