# stackowey
Stack-based 2D esolang

## Commands
|            ASCII character             |                               Name                               | Description                                                                                                                                                                                  |
| :------------------------------------: | :--------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|                  `/`                   |                             "bounce"                             | Pops two values off the stack. Bounces the pointer off if the first one is greater than the second one                                                                                       |
|                  `\`                   |                           "backbounce"                           | Pops two values off the stack. Bounces the pointer off if the first one is lower than the second one                                                                                         |
|                  `?`                   |                              "huh"                               | Gets UTF-8-encoded input from the user. Pushes each character to the stack as its Unicode code point, followed by a 0 up top                                                                 |
|                  `!`                   |                              "hah"                               | Pops and outputs the UTF-8-encoded character according to its Unicode code point found on the stack                                                                                          |
|                  `%`                   |                             "splot"                              | Pops and takes the top two stack values as coordinates to jump to while not changing direction.  <br>_Note: the movement is done after the jump, so the command being jumped to is ignored!_ |
| `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7` | "oh", "itch", "nigh", "chan", "fire", "lamp", "glorp", "wa-ding" | Pushes the corresponding number to the stack                                                                                                                                                 |
|                  `+`                   |                             "cross"                              | Pops two stack values and pushes their sum onto the stack                                                                                                                                    |
|                  `_`                   |                            "stumble"                             | Inverts all bits of the top stack element                                                                                                                                                    |
|                  `#`                   |                             "grille"                             | Pops one value off the stack, calls it n and swaps the n:th and topmost stack element                                                                                                        |
|                  `@`                   |                           "whirlpool"                            | Pops one value off the stack, calls it n and pushes a copy of the n:th stack element to the top of the stack                                                                                 |
|                  `.`                   |                             "prick"                              | Pops one value off the stack and throws it into the void                                                                                                                                     |
|                  `8`                   |                            "dubring"                             | Pushes the current coordinates to the stack                                                                                                                                                  |
|                  `9`                   |                            "tailring"                            | Halts the program                                                                                                                                                                            |
|           _all other bytes_            |                            "impostor"                            | These are all no-ops                                                                                                                                                                         |

> [!TIP]
> - to multiply `a*b`, loop `b` times adding `a` to itself
>     - bonus tip: swap `a` and `b` if `b` is larger than `a` for faster computation
> - to subtract `a-b`, add (`(NOT b) + 1`) to `a`
> - to divide `a/b`, loop subtracting `b` from `a` (or what's left of `a` after the previous iterations) and count how often you loop until `a<b`
>     - conveniently, this also leaves you with `a (mod b)` as what's left in `a` after the loop finishes
