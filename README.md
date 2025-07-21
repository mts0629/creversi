# creversi

Reversi in C

## Requirements

- gcc 11.4.0
- GNU Make 4.3
- clang-format 14.0.0

## Build

```bash
$ make
```

Program will be built as `build/creversi`.

## Run

```
$ ./build/creversi
    a b c d e f g h
  +-----------------+
1 | - - - - - - - - |
2 | - - - - - - - - |
3 | - - - * - - - - |
4 | - - * O X - - - |
5 | - - - X O * - - |
6 | - - - - * - - - |
7 | - - - - - - - - |
8 | - - - - - - - - |
  +-----------------+
Black (X) turn > 
```

- Start from a black player
- Valid moves are shown as `*`
- Input is a coordinate in: `[col character][row index]` (`a0` - `h8`)
    - Capital letters (`A` - `H`) can be used
- Quit a game by `q`

## License

Unlicense
