console.log("Shri Radha");
const canvas = document.getElementById("myCanvas");
import WasmModule from "./graphics.js";

(async () => {
	const moduleArgs = {
		print: (function () {})(),
		printErr: function (text) {
			console.log(text);
		},
		canvas: (function () {
			return document.getElementById("myCanvas");
		})(),
		setStatus: function (text) {
			console.log(text);
		},
		totalDependencies: 0,
	};

	const mod = await WasmModule(moduleArgs);

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
