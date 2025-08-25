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

### How to play

- Start from a black (`X`), white (`O`) is next
- Position of valid moves are shown as `*`
- Input: coordinate in `[col. character (a-h)][row index (1-8)]` (`a1` - `h8`)
    - Capital letters (`A` - `H`) can be used
- Quit a game by `q`

### Command line optons

- `-b`: set a player to black (default)
- `-w`: set a player to white
- `-c`: play with COM vs COM mode
- `-r`: output a game record as `record.txt`

## License

Unlicense
