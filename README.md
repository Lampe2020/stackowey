# stackowey
Stack-based 2D esolang

## Commands
|            ASCII Character             |                               Name                               | Description                                                                                                        |
| :------------------------------------: | :--------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------- |
|                  `9`                   |                            "tailring"                            | Halt                                                                                                               |
|           *all other bytes*            |                                                                  | No-op                                                                                                              |
|                  `/`                   |                             "Bounce"                             | Pop two values off the stack. Bounce off if the first one is greater than the second one.                          |
|                  `\`                   |                           "Backbounce"                           | Pop two values off the stack. Bounce off if the first one is lower than the second one.                            |
|                  `?`                   |                              "Huh"                               | Get input from the user. Each character is pushed to the stack as its unicode codepoint.                           |
|                  `!`                   |                              "Hah"                               | Output a value to the user. Outputs the UTF-8-encoded character according to its unicode value found on the stack. |
|                  `%`                   |                             "Splot"                              | Take the top two stack values as coordinates to jump to while not changing direction                               |
| `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7` | "Oh", "Itch", "Nigh", "Chan", "Fire", "Lamp", "Glorp", "Wa-ding" | Push the corresponding octal value to the stack.                                                                   |
|                  `+`                   |                             "Cross"                              | Pop two stack values and push their sum onto the stack (as two values if overflowing)                              |
|                  `#`                   |                             "Grille"                             | Pop one value off the stack, call it n and swap the n:th and n-1:st stack element                                  |
|                  `@`                   |                           "Whirlpool"                            | Pop one value off the stack, call it n and push a copy of the n:th stack element to the top of the stack           |
|                  `8`                   |                            "Dubring"                             | Push the current coordinates to the stack and continue straight ahead                                              |
|                  `9`                   |                            "Tailring"                            | Halt                                                                                                               |

> [!NOTE]
> - to multiply `a*b`, loop `b` times adding `a` to itself
> - to subtract `a-b`, subtract `b` 2 times from itself (effectively negating it), then add it to `a`
> - to divide `a/b`, loop subtracting `b` from `a` (or what's left of `a` after the previous iterations) and count how often you loop
>     - Conveniently, this also leaves you with `a (mod b)`
