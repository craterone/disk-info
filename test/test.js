const disk = require('..');

const result = disk.info('/Users/congchen/workspace/self/disk-info');

console.log(result);

const n = 1 << 30
console.log(`size: ${Math.round(result.size / n)} GB`);
console.log(`available: ${Math.round(result.available / n)} GB`);
console.log(`used: ${Math.round(result.used / n)} GB`);
console.log(`free: ${Math.round(result.free / n)} GB`);