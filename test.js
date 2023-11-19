const { ptr } = require("./build/Release/ptr");
console.log(ptr);
const buffer = new ArrayBuffer(1024);
const view = new Int32Array(buffer);
view[0] = 1;
view[1] = 2;
view[2] = 3;
view[3] = 4;
ptr(buffer);
ptr(view);

