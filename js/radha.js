console.log("Shri Radha");
const canvas = document.getElementById("myCanvas");
import WasmModule from "./krsna.mjs";

(async () => {
	const moduleArgs = {
		printErr: (text) => {
			console.log(text);
		},
		canvas,
		setStatus: (text) => {
			console.log(text);
		},
	};

	const mod = await WasmModule(moduleArgs);

	window.addEventListener("resize", js_resizeCanvas);

	function js_resizeCanvas() {
		if (canvas) {
			canvas.width = window.innerWidth;
			canvas.height = window.innerHeight;
			mod.ccall(
				"canvas_resize", // name of C function
				"void", // return type
				["number", "number"], // argument types
				[canvas.width, canvas.height]
			);
		}
	}

	js_resizeCanvas();
})();
