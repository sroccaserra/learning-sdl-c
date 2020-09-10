Learning SDL2 in C (maybe).

## Whishlist

These seem like a reasonable plan, but might or might not come to existence.

### Images

- [x] 1 (or more ?) 256x256 pixels tile sheet with color key

### Backgrounds & Sprites

- [ ] 1 infinite tiled raster background (wraps around)
- [ ] 1 `h_blank()` function, called before rendering each line of the raster
      background (allows simple raster effects)

- [x] display part of the tile sheet
- [ ] n ordered tiled pannels, useable for n backgrounds or sprites

- [ ] Indefinite pannel scrolling in one direction (display only part of a big enough tilemap)
- [ ] Indefinite pannel scrolling in two directions (see above)

- [ ] pannel animation (swap tiles)

### Sounds & Musics

- [x] play simple wave forms
- [ ] play 8 bit sample
- [ ] trigger sound related to an action
- [ ] play a tune with simple wave forms

### Controls

- [x] move player with keyboard or joypad

## References

- <https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/les-premieres-fenetres/#1-initialiser-la-sdl>
- <https://stackoverflow.com/questions/12485190/calling-fsincos-instruction-in-llvm-slower-than-calling-libc-sin-cos-functions/24470801>
- <https://www.gnu.org/software/make/manual/html_node/Catalogue-of-Rules.html#Catalogue-of-Rules>
- <https://wiki.libsdl.org/MigrationGuide#Video>
- <https://wiki.libsdl.org/SDL_AudioSpec>
- <https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html>
