const shm = require('../')
const fs = require('fs')


let fd = shm.shm_open("testfile", shm.O_CREAT | shm.O_RDWR, fs.S_IRWXU)

console.log("fd : %d", fd)

let ret = shm.shm_unlink("testfile")

console.log(ret)
