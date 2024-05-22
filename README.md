# fasm-bcd-mul

this repo contains 2 little C programs to add or multiply 2 BCD integers (in array representation), that uses functions, written in FASM.

what is fasm? - https://flatassembler.net \
what is bcd number? - https://en.wikipedia.org/wiki/Binary-coded_decimal

in my case i using 8421 BCD code, i think

in fact therre also implementation of BCD multiplication in C, just to compare the speed and correctness of the algorithm written in FASM (spoiter: it slower, you cannot beat compiler)

# building

do
```bash
sudo apt install fasm build-essential git
git clone https://github.com/mettemfurfur000/fasm-bcd-mul.git
cd fasm-bcd-mul
make
```
and thats it

if things doesn compile u could also do `sudo apt installgcc-multilib`

# contributing

do pull request or whatever
